//
// Created by rmyho on 6/7/2020.
//

#ifndef PICOFRAMEWORK_ALLOCATOR_H
#define PICOFRAMEWORK_ALLOCATOR_H

#include <vk_mem_alloc.h>
#include "pfvk.h"

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
	void init(VkInstance instance, VkDevice device, VkPhysicalDevice physicalDevice);

	void destroy();

	BufferAllocation createBuffer(VkBufferCreateInfo &bufferCreateInfo, VmaMemoryUsage memoryUsage);

	ImageAllocation createImage(VkImageCreateInfo &imageCreateInfo, VmaMemoryUsage memoryUsage);

	void* map(VmaAllocation allocation);

	void unmap(VmaAllocation allocation);

private:
	VmaAllocator vmaAllocator_;
};


#endif //PICOFRAMEWORK_ALLOCATOR_H
