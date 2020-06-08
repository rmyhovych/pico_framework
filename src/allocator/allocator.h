//
// Created by rmyho on 6/7/2020.
//

#ifndef PICOFRAMEWORK_ALLOCATOR_H
#define PICOFRAMEWORK_ALLOCATOR_H

#include <vk_mem_alloc.h>
#include "pfvk.h"

class Device;

struct ImageAllocation
{
	VkImage image;
	VmaAllocation allocation;
};

struct BufferAllocation
{
	VkBuffer buffer;
	VmaAllocation allocation;
};

class Allocator
{
public:
	class StagingCommand
	{
	public:

		BufferAllocation stageBuffer(VkBuffer buffer, VkBufferUsageFlags usage, VkBufferCopy bufferCopy);

		ImageAllocation stageImage(VkBuffer buffer, uint32_t size);

		void execute();

	private:
		StagingCommand(VkDevice device, VmaAllocator allocator, VkCommandPool commandPool, VmaMemoryUsage finalUsage);

		VkDevice deviceHandle_;
		VmaAllocator allocator_;
		VkCommandPool commandPool_;

		VmaAllocationCreateInfo allocationCreateInfo_;

		VkCommandBuffer commandBuffer_;
	};

	Allocator();

	void init(VkInstance instance, Device* pDevice);

	void destroy();

	BufferAllocation createBuffer(VkBufferCreateInfo &bufferCreateInfo, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags = 0) const;

	ImageAllocation createImage(VkImageCreateInfo &imageCreateInfo, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags = 0) const;

	void* map(VmaAllocation allocation);

	void unmap(VmaAllocation allocation);

private:
	Device* pDevice_;
	VkCommandPool commandPool_;

	VmaAllocator vmaAllocator_;
};


#endif //PICOFRAMEWORK_ALLOCATOR_H
