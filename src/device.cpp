//
// Created by rmyho on 5/19/2020.
//

#include "device.h"

Device::Device(const Device::Properties &properties, VkInstance instance, const Surface &surface) :
		instance_(instance),
		handle_(VK_NULL_HANDLE),

		physicalDevice_(instance, surface.getHandle(), properties.type, properties.extensions),

		pAllocator_(nullptr)
{
	handle_ = physicalDevice_.createLogicalDevice(properties.extensions);
	const QueueFamilyIndexes &queueFamilyIndexes = physicalDevice_.getQueueFamilyIndexes();

	// Queues
	vkGetDeviceQueue(handle_, queueFamilyIndexes.graphical, 0, &queueGraphics_);
	vkGetDeviceQueue(handle_, queueFamilyIndexes.present, 0, &queuePresent_);

	pAllocator_ = Allocator::Builder()
			.setInstance(instance_)
			.setDevices(handle_, physicalDevice_.getHandle())
			.setTransferData(createCommandPool(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT, TRANSFER), queueGraphics_)
			.build();
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
	delete pAllocator_;
	vkDestroyDevice(handle_, nullptr);
}


Allocator* Device::getAllocator()
{
	return pAllocator_;
}

VkCommandPool Device::createCommandPool(VkCommandPoolCreateFlags flags, FamilyIndexType familyIndexType) const
{
	QueueFamilyIndexes queueFamilyIndexes = physicalDevice_.getQueueFamilyIndexes();

	VkCommandPoolCreateInfo commandPoolCreateInfo{};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	switch (familyIndexType)
	{
		case TRANSFER:
		case GRAPHICAL:
			commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndexes.graphical;
			break;

		case PRESENT:
			commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndexes.present;
			break;
	}
	commandPoolCreateInfo.flags = flags;
	commandPoolCreateInfo.pNext = nullptr;

	VkCommandPool commandPool;
	CALL_VK(vkCreateCommandPool(handle_, &commandPoolCreateInfo, nullptr, &commandPool))
	return commandPool;
}
