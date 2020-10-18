//
// Created by rmyho on 6/7/2020.
//

#define VMA_IMPLEMENTATION

#include "allocator.h"

#include "instance.h"
#include "device.h"


Allocator::Allocator(const Instance &instance, const Device &device) :
		handle_(VK_NULL_HANDLE)
{

	VmaAllocatorCreateInfo allocatorCreateInfo{};
	allocatorCreateInfo.instance = instance.handle_;
	allocatorCreateInfo.device = device.handle_;
	allocatorCreateInfo.physicalDevice = device.pPhysicalDevice_->handle_;

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
	CHECK_NULL_HANDLE(handle_)
}

void Allocator::destroy()
{
	vmaDestroyAllocator(handle_);
	handle_ = VK_NULL_HANDLE;
}

BufferAllocation Allocator::createBuffer(VkBufferCreateInfo &bufferCreateInfo, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags) const
{
	VmaAllocationCreateInfo allocationCreateInfo{};
	allocationCreateInfo.usage = memoryUsage;
	allocationCreateInfo.flags = flags;

	BufferAllocation bufferAllocation;
	CALL_VK(vmaCreateBuffer(handle_, &bufferCreateInfo, &allocationCreateInfo, &bufferAllocation.handle, &bufferAllocation.allocation, nullptr));
	return bufferAllocation;
}

ImageAllocation Allocator::createImage(VkImageCreateInfo &imageCreateInfo, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags) const
{
	VmaAllocationCreateInfo allocationCreateInfo{};
	allocationCreateInfo.usage = memoryUsage;
	allocationCreateInfo.flags = flags;

	ImageAllocation imageAllocation;
	CALL_VK(vmaCreateImage(handle_, &imageCreateInfo, &allocationCreateInfo, &imageAllocation.handle, &imageAllocation.allocation, nullptr))
	return imageAllocation;
}

void Allocator::free(BufferAllocation &obj) const
{
	vmaDestroyBuffer(handle_, obj.handle, obj.allocation);
}

void Allocator::free(ImageAllocation &obj) const
{
	vmaDestroyImage(handle_, obj.handle, obj.allocation);
}
