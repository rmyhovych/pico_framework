//
// Created by rmyho on 5/30/2020.
//

#ifndef PICOFRAMEWORK_PHYSICAL_DEVICE_H
#define PICOFRAMEWORK_PHYSICAL_DEVICE_H

#include "pfvk.h"
#include "surface.h"

class PhysicalDevice
{
public:
	PhysicalDevice(VkPhysicalDevice handle);

	VkPhysicalDevice getHandle() const;

	VkFormat getSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags featureFlags) const;

	bool getQueueFamilyIndex(VkPhysicalDevice physicalDevice, VkQueueFlags flags, uint32_t* index);

private:
	VkPhysicalDevice handle_;
};


#endif //PICOFRAMEWORK_PHYSICAL_DEVICE_H
