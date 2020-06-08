//
// Created by rmyho on 5/19/2020.
//

#ifndef PICOFRAMEWORK_DEVICE_H
#define PICOFRAMEWORK_DEVICE_H

#include <pfvk.h>
#include <vector>

#include "surface.h"
#include "physical_device.h"
#include "allocator.h"


class Device
{
public:
	struct Properties
	{
		VkPhysicalDeviceType type;
		std::vector<const char*> extensions;
	};

	Device(const Device::Properties &properties, VkInstance instance, const Surface &surface);

	Allocator* getAllocator();

	VkDevice getHandle() const;
	const PhysicalDevice& getPhysicalDevice() const;

	void destroy();

private:
	VkInstance instance_;
	VkDevice handle_;

	PhysicalDevice physicalDevice_;

	Allocator allocator_;

	VkQueue queueGraphics_;
	VkQueue queuePresent_;
};


#endif //PICOFRAMEWORK_DEVICE_H
