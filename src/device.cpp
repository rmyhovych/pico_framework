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

	allocator_.init(instance_, this);

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
	allocator_.destroy();
	vkDestroyDevice(handle_, nullptr);
}


Allocator* Device::getAllocator()
{
	return &allocator_;
}

VkCommandPool Device::createCommandPool(VkCommandPoolCreateFlags flags) const
{
	VkCommandPoolCreateInfo commandPoolCreateInfo{};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.queueFamilyIndex = physicalDevice_.getQueueFamilyIndexes().graphical;
	commandPoolCreateInfo.flags = flags;
	commandPoolCreateInfo.pNext = nullptr;

	VkCommandPool commandPool;
	CALL_VK(vkCreateCommandPool(handle_, &commandPoolCreateInfo, nullptr, &commandPool))
	return commandPool;
}
