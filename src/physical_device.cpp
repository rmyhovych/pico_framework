//
// Created by rmyho on 5/30/2020.
//

#include "physical_device.h"
#include <set>

PhysicalDevice::PhysicalDevice(VkInstance instanceHandle, VkSurfaceKHR surfaceHandle, VkPhysicalDeviceType deviceType, const std::vector<const char*> &neededExtensions) :
		handle_(VK_NULL_HANDLE),
		queueFamilyIndexes_({0, 0})
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instanceHandle, &deviceCount, nullptr);

	if (deviceCount < 1)
	{
		throw std::exception("No devices supporting Vulkan found!");
	}

	std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
	vkEnumeratePhysicalDevices(instanceHandle, &deviceCount, physicalDevices.data());

	VkPhysicalDeviceProperties deviceProperties;
	for (VkPhysicalDevice pd : physicalDevices)
	{
		vkGetPhysicalDeviceProperties(pd, &deviceProperties);
		if (deviceProperties.deviceType == deviceType)
		{
			if (isDeviceSuitable(pd, surfaceHandle, neededExtensions))
			{
				handle_ = pd;
				break;
			}
		}
	}

	if (handle_ == VK_NULL_HANDLE)
	{
		handle_ = physicalDevices[0];

		if (!isDeviceSuitable(handle_, surfaceHandle, neededExtensions))
		{
			throw std::exception("No device is suitable!");
		}
	}

	queueFamilyIndexes_ = getAvailableQueueFamilyIndexes(handle_, surfaceHandle);
}


VkPhysicalDevice PhysicalDevice::getHandle() const
{
	return handle_;
}

VkDevice PhysicalDevice::createLogicalDevice(const std::vector<const char*> &extensions) const
{
	std::set<uint32_t> uniqueQueueFamilies = {queueFamilyIndexes_.graphical, queueFamilyIndexes_.present};
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	float priority = 1.0f;
	for (uint32_t queueFamilyIndex : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &priority;

		queueCreateInfos.push_back(queueCreateInfo);
	}


	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

	VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
	physicalDeviceFeatures.samplerAnisotropy = VK_TRUE;

	deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

	deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
	deviceCreateInfo.enabledExtensionCount = (uint32_t) extensions.size();

	deviceCreateInfo.enabledLayerCount = 0;
	deviceCreateInfo.ppEnabledLayerNames = nullptr;

	VkDevice device;
	CALL_VK(vkCreateDevice(handle_, &deviceCreateInfo, nullptr, &device))

	return device;
}

const QueueFamilyIndexes &PhysicalDevice::getQueueFamilyIndexes() const
{
	return queueFamilyIndexes_;
}


VkFormat PhysicalDevice::getSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags featureFlags) const
{
	for (VkFormat format : candidates)
	{
		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(handle_, format, &properties);

		if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & featureFlags) == featureFlags)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & featureFlags) == featureFlags)
		{
			return format;
		}

		throw std::exception("Failed to find supported format.");
	}
}


// TODO: improve suitable check by validating surface swapchain configurations
bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surfaceHandle, const std::vector<const char*> &extensions)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensionProperties(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensionProperties.data());

	std::set<std::string> extensionsLeft(extensions.begin(), extensions.end());
	for (VkExtensionProperties &ep : extensionProperties)
	{
		extensionsLeft.erase(ep.extensionName);
	}

	bool isPhysicalDeviceSuitable = extensionsLeft.empty();
	if (isPhysicalDeviceSuitable)
	{
		isPhysicalDeviceSuitable = getQueueFamilyIndexGraphics(physicalDevice, nullptr) &&
		                           getQueueFamilyIndexPresent(physicalDevice, surfaceHandle, nullptr, 0);
	}

	if (isPhysicalDeviceSuitable)
	{
		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

		isPhysicalDeviceSuitable = supportedFeatures.samplerAnisotropy;
	}

	return isPhysicalDeviceSuitable;
}

QueueFamilyIndexes PhysicalDevice::getAvailableQueueFamilyIndexes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surfaceHandle)
{
	QueueFamilyIndexes familyIndexes = {0, 0};
	if (!getQueueFamilyIndexGraphics(physicalDevice, &familyIndexes.graphical))
	{
		throw std::exception("Device not suitable.");
	}

	if (!getQueueFamilyIndexPresent(physicalDevice, surfaceHandle, &familyIndexes.present, familyIndexes.graphical))
	{
		throw std::exception("Device not suitable.");
	}

	return familyIndexes;
}

bool PhysicalDevice::getQueueFamilyIndexGraphics(VkPhysicalDevice physicalDevice, uint32_t* index)
{
	uint32_t nQueueFamilies;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nQueueFamilies, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyProperties(nQueueFamilies);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nQueueFamilies, queueFamilyProperties.data());

	bool found = false;
	for (uint32_t i = 0; i < nQueueFamilies; ++i)
	{
		if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			if (index != nullptr)
			{
				*index = i;
			}

			return true;
		}
	}

	return found;
}

bool PhysicalDevice::getQueueFamilyIndexPresent(VkPhysicalDevice physicalDevice, VkSurfaceKHR surfaceHandle, uint32_t* index, uint32_t priorityIndex)
{
	uint32_t nQueueFamilies;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nQueueFamilies, nullptr);

	VkBool32 supported;
	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, priorityIndex, surfaceHandle, &supported);
	if (supported)
	{
		if (index != nullptr)
		{
			*index = priorityIndex;
		}

		return true;
	}

	for (uint32_t i = 0; i < nQueueFamilies; ++i)
	{
		if (i == priorityIndex)
			continue;

		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surfaceHandle, &supported);
		if (supported)
		{
			if (index != nullptr)
			{
				*index = i;
			}

			return true;
		}
	}

	return false;
}
