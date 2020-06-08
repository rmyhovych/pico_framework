//
// Created by rmyho on 6/7/2020.
//

#define VMA_IMPLEMENTATION

#include "allocator.h"


void Allocator::init(VkInstance instance, VkDevice device, VkPhysicalDevice physicalDevice)
{
	VmaAllocatorCreateInfo createInfo{};
	createInfo.instance = instance;
	createInfo.device = device;
	createInfo.physicalDevice = physicalDevice;

	createInfo.flags = VMA_ALLOCATOR_CREATE_EXTERNALLY_SYNCHRONIZED_BIT;
	createInfo.pAllocationCallbacks = nullptr;
	createInfo.pDeviceMemoryCallbacks = nullptr;
	createInfo.frameInUseCount = 0;
	createInfo.pHeapSizeLimit = nullptr;
	createInfo.pVulkanFunctions = nullptr;
	createInfo.pRecordSettings = nullptr;
	createInfo.vulkanApiVersion = VK_API_VERSION_1_0;

	CALL_VK(vmaCreateAllocator(&createInfo, &vmaAllocator_))
}

void Allocator::destroy()
{
	vmaDestroyAllocator(vmaAllocator_);
}


BufferAllocation Allocator::createBuffer(VkBufferCreateInfo &bufferCreateInfo, VmaMemoryUsage memoryUsage)
{
	VmaAllocationCreateInfo allocationCreateInfo{};
	allocationCreateInfo.usage = memoryUsage;

	BufferAllocation allocation{};
	CALL_VK(vmaCreateBuffer(vmaAllocator_, &bufferCreateInfo, &allocationCreateInfo, &allocation.buffer, &allocation.allocation, nullptr))

	return allocation;
}

ImageAllocation Allocator::createImage(VkImageCreateInfo &imageCreateInfo, VmaMemoryUsage memoryUsage)
{
	VmaAllocationCreateInfo allocationCreateInfo{};
	allocationCreateInfo.usage = memoryUsage;

	ImageAllocation allocation{};
	CALL_VK(vmaCreateImage(vmaAllocator_, &imageCreateInfo, &allocationCreateInfo, &allocation.image, &allocation.allocation, nullptr))

	return allocation;
}

void* Allocator::map(VmaAllocation allocation)
{
	void* data;
	CALL_VK(vmaMapMemory(vmaAllocator_, allocation, &data))
	return data;
}

void Allocator::unmap(VmaAllocation allocation)
{
	vmaUnmapMemory(vmaAllocator_, allocation);
}

