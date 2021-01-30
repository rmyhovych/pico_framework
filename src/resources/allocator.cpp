//
// Created by rmyho on 6/7/2020.
//

#define VMA_IMPLEMENTATION

#include "allocator.h"


Allocator::Allocator(VkInstance hInstance, VkDevice hDevice, VkPhysicalDevice hPhysicalDevice) :
		handle_(VK_NULL_HANDLE)
{
	VmaAllocatorCreateInfo allocatorCreateInfo{};
	allocatorCreateInfo.instance = hInstance;
	allocatorCreateInfo.device = hDevice;
	allocatorCreateInfo.physicalDevice = hPhysicalDevice;

	allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXTERNALLY_SYNCHRONIZED_BIT;
	allocatorCreateInfo.pAllocationCallbacks = nullptr;
	allocatorCreateInfo.pDeviceMemoryCallbacks = nullptr;
	allocatorCreateInfo.frameInUseCount = 0;
	allocatorCreateInfo.pHeapSizeLimit = nullptr;
	allocatorCreateInfo.pVulkanFunctions = nullptr;
	allocatorCreateInfo.pRecordSettings = nullptr;
	allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_0;

	CALL_VK(vmaCreateAllocator(&allocatorCreateInfo, &handle_))
}

Allocator::~Allocator()
{
	vmaDestroyAllocator(handle_);
	handle_ = VK_NULL_HANDLE;
}

std::pair<VkBuffer, VmaAllocation> Allocator::createBuffer(VkBufferCreateInfo& bufferCreateInfo, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags) const
{
	VmaAllocationCreateInfo allocationCreateInfo{};
	allocationCreateInfo.usage = memoryUsage;
	allocationCreateInfo.flags = flags;

	std::pair<VkBuffer, VmaAllocation> bufferAllocation;
	CALL_VK(vmaCreateBuffer(handle_, &bufferCreateInfo, &allocationCreateInfo, &bufferAllocation.first, &bufferAllocation.second, nullptr))
	return bufferAllocation;
}

std::pair<VkImage, VmaAllocation> Allocator::createImage(VkImageCreateInfo& imageCreateInfo, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags) const
{
	VmaAllocationCreateInfo allocationCreateInfo{};
	allocationCreateInfo.usage = memoryUsage;
	allocationCreateInfo.flags = flags;

	std::pair<VkImage, VmaAllocation> imageAllocation;
	CALL_VK(vmaCreateImage(handle_, &imageCreateInfo, &allocationCreateInfo, &imageAllocation.first, &imageAllocation.second, nullptr))
	return imageAllocation;
}

void Allocator::free(VkBuffer buffer, VmaAllocation allocation) const
{
	vmaDestroyBuffer(handle_, buffer, allocation);
}

void Allocator::free(VkImage image, VmaAllocation allocation) const
{
	vmaDestroyImage(handle_, image, allocation);
}


void* Allocator::map(VmaAllocation allocation) const
{
	void* data;
	CALL_VK(vmaMapMemory(handle_, allocation, &data))
	return data;
}

void Allocator::unmap(VmaAllocation allocation) const
{
	vmaUnmapMemory(handle_, allocation);
}