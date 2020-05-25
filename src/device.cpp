//
// Created by rmyho on 5/19/2020.
//

#include "device.h"

#include <set>
#include <string>

Device::Device(const Device::Properties &properties, VkInstance instance, const Surface &surface) :
		instance_(instance),

		physicalDevice_(VK_NULL_HANDLE),
		logicalDevice_(VK_NULL_HANDLE)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);

	if (deviceCount < 1)
	{
		throw std::exception("No devices supporting Vulkan found!");
	}

	std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
	vkEnumeratePhysicalDevices(instance_, &deviceCount, physicalDevices.data());

	VkPhysicalDeviceProperties deviceProperties;
	for (VkPhysicalDevice pd : physicalDevices)
	{
		vkGetPhysicalDeviceProperties(pd, &deviceProperties);
		if (deviceProperties.deviceType == properties.type)
		{
			if (isSuitable(pd, surface, properties.extensions))
			{
				physicalDevice_ = pd;
				break;
			}
		}
	}

	if (physicalDevice_ == VK_NULL_HANDLE)
	{
		physicalDevice_ = physicalDevices[0];

		if (!isSuitable(physicalDevice_, surface, properties.extensions))
		{
			throw std::exception("No device is suitable!");
		}
	}

	// Device
	queueFamilyIndexes_ = getAvailableQueueFamilyIndexes(physicalDevice_, surface);
	logicalDevice_ = createLogicalDevice(physicalDevice_, queueFamilyIndexes_, properties.extensions);

	// Queues
	vkGetDeviceQueue(logicalDevice_, queueFamilyIndexes_.graphical, 0, &queueGraphics_);
	vkGetDeviceQueue(logicalDevice_, queueFamilyIndexes_.present, 0, &queuePresent_);
}

VkDevice Device::getHandle() const
{
	return logicalDevice_;
}

VkPhysicalDevice Device::getPhysiacalDevice() const
{
	return physicalDevice_;
}


const QueueFamilyIndexes &Device::getQueueFamilyIndexes() const
{
	return queueFamilyIndexes_;
}



void Device::destroy()
{
	vkDestroyDevice(logicalDevice_, nullptr);
}

// TODO: improve suitable check by validating surface swapchain configurations
bool Device::isSuitable(VkPhysicalDevice physicalDevice, const Surface &surface, const std::vector<const char*> &extensions)
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
		isPhysicalDeviceSuitable = getQueueGraphicsFamilyIndex(physicalDevice, nullptr) &&
		                           getQueuePresentFamilyIndex(physicalDevice, surface.getHandle(), nullptr, 0);
	}

	if (isPhysicalDeviceSuitable)
	{
		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

		isPhysicalDeviceSuitable = supportedFeatures.samplerAnisotropy;
	}

	return isPhysicalDeviceSuitable;
}


QueueFamilyIndexes Device::getAvailableQueueFamilyIndexes(VkPhysicalDevice physicalDevice, const Surface &surface)
{
	QueueFamilyIndexes familyIndexes = {0, 0};
	if (!getQueueGraphicsFamilyIndex(physicalDevice, &familyIndexes.graphical))
	{
		throw std::exception("Device not suitable.");
	}

	if (!getQueuePresentFamilyIndex(physicalDevice, surface.getHandle(), &familyIndexes.present, familyIndexes.graphical))
	{
		throw std::exception("Device not suitable.");
	}

	return familyIndexes;
}


bool Device::getQueueGraphicsFamilyIndex(VkPhysicalDevice physicalDevice, uint32_t* index)
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

// TODO: improve priority selection
bool Device::getQueuePresentFamilyIndex(VkPhysicalDevice physicalDevice, VkSurfaceKHR surfaceHandle, uint32_t* index, uint32_t priorityIndex)
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

VkDevice Device::createLogicalDevice(VkPhysicalDevice physicalDevice, const QueueFamilyIndexes &familyIndexes, const std::vector<const char*> &extensions)
{
	std::set<uint32_t> uniqueQueueFamilies = {familyIndexes.graphical, familyIndexes.present};
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	for (uint32_t queueFamilyIndex : uniqueQueueFamilies)
	{
		queueCreateInfos.emplace_back();
		setQueueCreateInfo(queueCreateInfos[queueCreateInfos.size() - 1], queueFamilyIndex, 1.0f);
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
	CALL_VK(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device))

	return device;
}

void Device::setQueueCreateInfo(VkDeviceQueueCreateInfo &queueCreateInfo, uint32_t index, float priority)
{
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = index;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &priority;
}
