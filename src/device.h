//
// Created by rmyho on 5/19/2020.
//

#ifndef PICOFRAMEWORK_DEVICE_H
#define PICOFRAMEWORK_DEVICE_H

#include <pfvk.h>
#include <vector>

#include "surface.h"
#include "physical_device.h"
#include "allocator/allocator.h"


class Device
{
public:
	Device(const PhysicalDevice* pPhysicalDevice, const std::vector<VkQueueFlags> &queueFlags, const std::vector<const char*> &deviceExtensions);

	~Device();

	void destroy();

	VkCommandPool createCommandPool(VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex) const;

private:
	static VkDevice createHandle(VkPhysicalDevice physicalDevice, const std::vector<DeviceQueue> &queues, const std::vector<const char*> &deviceExtensions);

public:
	VkDevice handle_;
	const PhysicalDevice* pPhysicalDevice_;
	std::vector<DeviceQueue> deviceQueues_;
};


#endif //PICOFRAMEWORK_DEVICE_H
