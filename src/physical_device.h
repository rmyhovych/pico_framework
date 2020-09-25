//
// Created by rmyho on 5/30/2020.
//

#ifndef PICOFRAMEWORK_PHYSICAL_DEVICE_H
#define PICOFRAMEWORK_PHYSICAL_DEVICE_H

#include "pfvk.h"
#include "surface.h"

struct DeviceQueue
{
	VkQueue queue = VK_NULL_HANDLE;
	uint32_t family = 0;
	VkQueueFlags type = 0;
};


class PhysicalDevice
{
public:
	explicit PhysicalDevice(VkPhysicalDevice handle);

	VkFormat getSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags featureFlags) const;

	std::vector<uint32_t> getQueueFamilyIndexes(VkQueueFlags flags) const;

	void pickQueueFamilies(std::vector<DeviceQueue> &queueInfos) const;

public:
	const VkPhysicalDevice handle_;
};


#endif //PICOFRAMEWORK_PHYSICAL_DEVICE_H
