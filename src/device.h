//
// Created by rmyho on 5/19/2020.
//

#ifndef PICOFRAMEWORK_DEVICE_H
#define PICOFRAMEWORK_DEVICE_H

#include <pfvk.h>
#include <vector>

#include "surface.h"

struct QueueFamilyIndexes
{
	uint32_t graphical;
	uint32_t present;
};

// TODO: separate queue management from device
class Device
{
public:
	struct Properties
	{
		VkPhysicalDeviceType type;
		std::vector<const char*> extensions;
	};

	Device(const Device::Properties &properties, VkInstance instance, const Surface &surface);

	VkDevice getHandle() const;
	VkPhysicalDevice getPhysiacalDevice() const;

	const QueueFamilyIndexes& getQueueFamilyIndexes() const;

	void destroy();

private:
	static bool isSuitable(VkPhysicalDevice physicalDevice, const Surface &surface, const std::vector<const char*> &extensions);

	static QueueFamilyIndexes getAvailableQueueFamilyIndexes(VkPhysicalDevice physicalDevice, const Surface &surface);

	static bool getQueueGraphicsFamilyIndex(VkPhysicalDevice physicalDevice, uint32_t* index);

	static bool getQueuePresentFamilyIndex(VkPhysicalDevice physicalDevice, VkSurfaceKHR surfaceHandle, uint32_t* index, uint32_t priorityIndex);

	static VkDevice createLogicalDevice(VkPhysicalDevice physicalDevice, const QueueFamilyIndexes &familyIndexes, const std::vector<const char*> &extensions);


	static void setQueueCreateInfo(VkDeviceQueueCreateInfo &queueCreateInfo, uint32_t index, float priority);

private:
	VkInstance instance_;

	VkPhysicalDevice physicalDevice_;
	VkDevice logicalDevice_;

	QueueFamilyIndexes queueFamilyIndexes_;
	VkQueue queueGraphics_;
	VkQueue queuePresent_;
};


#endif //PICOFRAMEWORK_DEVICE_H
