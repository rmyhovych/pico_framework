//
// Created by rmyho on 5/30/2020.
//

#ifndef PICOFRAMEWORK_PHYSICAL_DEVICE_H
#define PICOFRAMEWORK_PHYSICAL_DEVICE_H

#include "pfvk.h"
#include "device_queue.h"

#include "device.h"

#include <vector>

class PhysicalDevice
{
public:
	explicit PhysicalDevice(VkInstance hInstance, VkPhysicalDevice handle);

	Device createDevice() const;

	VkFormat pickSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags featureFlags) const;

	VkFormat pickSupportedDepthFormat() const;

	std::vector<uint32_t> getQueueFamilyIndexes(VkQueueFlags flags) const;

	void pickQueueFamilies(std::vector<DeviceQueue>& queueInfos) const;

public:
	VkPhysicalDevice handle_;

private:
	VkInstance hInstance_;
};


#endif //PICOFRAMEWORK_PHYSICAL_DEVICE_H
