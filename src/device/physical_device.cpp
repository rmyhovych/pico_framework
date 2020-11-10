//
// Created by rmyho on 5/30/2020.
//

#include "physical_device.h"


PhysicalDevice::PhysicalDevice(VkInstance hInstance, VkPhysicalDevice handle) :
		hInstance_(hInstance),
		handle_(handle)
{
}

Device PhysicalDevice::createDevice() const
{
	std::vector<DeviceQueue> deviceQueues;
	for (VkQueueFlags queueFlag : {VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_TRANSFER_BIT})
		deviceQueues.push_back({VK_NULL_HANDLE, 0, queueFlag});

	pickQueueFamilies(deviceQueues);

	return Device(hInstance_, handle_, deviceQueues, {VK_KHR_SWAPCHAIN_EXTENSION_NAME});
}

VkFormat PhysicalDevice::pickSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags featureFlags) const
{
	for (VkFormat format : candidates)
	{
		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(handle_, format, &properties);

		VkFormatFeatureFlags candidateFeatureFlags;
		switch (tiling)
		{
			case VK_IMAGE_TILING_LINEAR:
				candidateFeatureFlags = properties.linearTilingFeatures;
				break;

			case VK_IMAGE_TILING_OPTIMAL:
				candidateFeatureFlags = properties.optimalTilingFeatures;
				break;

			default:
				continue;
		}

		if ((candidateFeatureFlags & featureFlags) == featureFlags)
		{
			return format;
		}
	}

	return VK_FORMAT_UNDEFINED;
}

VkFormat PhysicalDevice::pickSupportedDepthFormat() const
{
	return pickSupportedFormat(
			{VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT},
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

std::vector<uint32_t> PhysicalDevice::getQueueFamilyIndexes(VkQueueFlags flags) const
{
	uint32_t nQueueFamilies;
	vkGetPhysicalDeviceQueueFamilyProperties(handle_, &nQueueFamilies, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyProperties(nQueueFamilies);
	vkGetPhysicalDeviceQueueFamilyProperties(handle_, &nQueueFamilies, queueFamilyProperties.data());

	std::vector<uint32_t> indexes(0);

	for (uint32_t i = 0; i < nQueueFamilies; ++i)
	{
		if ((queueFamilyProperties[i].queueFlags & flags) == flags)
		{
			indexes.push_back(i);
		}
	}

	return indexes;
}

void PhysicalDevice::pickQueueFamilies(std::vector<DeviceQueue> &queueInfos) const
{
	VkQueueFlags commonType = 0;
	for (DeviceQueue &queue : queueInfos)
	{
		commonType |= queue.type;
	}

	std::vector<uint32_t> commonFamilies = getQueueFamilyIndexes(commonType);
	if (!commonFamilies.empty())
	{
		for (DeviceQueue &queue : queueInfos)
		{
			queue.family = commonFamilies[0];
		}
	}
	else
	{
		for (DeviceQueue &queue : queueInfos)
		{
			std::vector<uint32_t> families = getQueueFamilyIndexes(queue.type);
			queue.family = families[0];
		}
	}
}
