//
// Created by rmyho on 5/30/2020.
//

#include "physical_device.h"

#include <set>


PhysicalDevice::PhysicalDevice(VkPhysicalDevice handle) :
		handle_(handle)
{
}


VkFormat PhysicalDevice::getSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags featureFlags) const
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
