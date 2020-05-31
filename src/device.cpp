//
// Created by rmyho on 5/19/2020.
//

#include "device.h"

Device::Device(const Device::Properties &properties, VkInstance instance, const Surface &surface) :
		instance_(instance),
		handle_(VK_NULL_HANDLE),

		physicalDevice_(instance, surface.getHandle(), properties.type, properties.extensions)
{
	handle_ = physicalDevice_.createLogicalDevice(properties.extensions);
	const QueueFamilyIndexes &queueFamilyIndexes = physicalDevice_.getQueueFamilyIndexes();

	// Queues
	vkGetDeviceQueue(handle_, queueFamilyIndexes.graphical, 0, &queueGraphics_);
	vkGetDeviceQueue(handle_, queueFamilyIndexes.present, 0, &queuePresent_);
}

VkDevice Device::getHandle() const
{
	return handle_;
}

const PhysicalDevice &Device::getPhysicalDevice() const
{
	return physicalDevice_;
}

void Device::destroy()
{
	vkDestroyDevice(handle_, nullptr);
}
