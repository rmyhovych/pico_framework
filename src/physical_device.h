//
// Created by rmyho on 5/30/2020.
//

#ifndef PICOFRAMEWORK_PHYSICAL_DEVICE_H
#define PICOFRAMEWORK_PHYSICAL_DEVICE_H

#include "pfvk.h"
#include "surface.h"

struct QueueFamilyIndexes
{
	uint32_t graphical;
	uint32_t present;
};

class PhysicalDevice
{
public:
	PhysicalDevice(VkInstance instanceHandle, VkSurfaceKHR surfaceHandle, VkPhysicalDeviceType deviceType, const std::vector<const char*> &neededExtensions);

	VkPhysicalDevice getHandle() const;

	VkDevice createLogicalDevice(const std::vector<const char*> &extensions) const;

	const QueueFamilyIndexes &getQueueFamilyIndexes() const;

	VkFormat getSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags featureFlags) const;

private:
	static bool isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surfaceHandle, const std::vector<const char*> &extensions);

	static QueueFamilyIndexes getAvailableQueueFamilyIndexes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surfaceHandle);

	static bool getQueueFamilyIndexGraphics(VkPhysicalDevice physicalDevice, uint32_t* index);

	static bool getQueueFamilyIndexPresent(VkPhysicalDevice physicalDevice, VkSurfaceKHR surfaceHandle, uint32_t* index, uint32_t priorityIndex);

private:
	VkPhysicalDevice handle_;
	QueueFamilyIndexes queueFamilyIndexes_;
};


#endif //PICOFRAMEWORK_PHYSICAL_DEVICE_H
