//
// Created by rmyho on 5/30/2020.
//

#include "physical_device.h"

PhysicalDevice::PhysicalDevice(VkPhysicalDevice handle) :
		handle_(handle)
{
}


VkPhysicalDevice PhysicalDevice::getHandle() const
{
	return handle_;
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


bool PhysicalDevice::getQueueFamilyIndex(VkPhysicalDevice physicalDevice, VkQueueFlags flags, uint32_t* index)
{
	uint32_t nQueueFamilies;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nQueueFamilies, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyProperties(nQueueFamilies);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nQueueFamilies, queueFamilyProperties.data());

	bool found = false;
	for (uint32_t i = 0; i < nQueueFamilies; ++i)
	{
		if ((queueFamilyProperties[i].queueFlags & flags) == flags)
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
