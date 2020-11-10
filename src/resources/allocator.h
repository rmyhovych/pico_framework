//
// Created by rmyho on 6/7/2020.
//

#ifndef PICOFRAMEWORK_ALLOCATOR_H
#define PICOFRAMEWORK_ALLOCATOR_H


#include <utility>
#include <vk_mem_alloc.h>

#include "pfvk.h"

class Allocator
{
public:
	Allocator(VkInstance hInstance, VkDevice hDevice, VkPhysicalDevice hPhysicalDevice);

	~Allocator();

	std::pair<VkBuffer, VmaAllocation> createBuffer(VkBufferCreateInfo &bufferCreateInfo, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags = 0) const;

	std::pair<VkImage, VmaAllocation> createImage(VkImageCreateInfo &imageCreateInfo, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags = 0) const;

	void free(VkBuffer buffer, VmaAllocation allocation) const;

	void free(VkImage image, VmaAllocation allocation) const;

	void* map(VmaAllocation allocation) const;

	void unmap(VmaAllocation allocation) const;

private:
	VmaAllocator handle_;
};


#endif //PICOFRAMEWORK_ALLOCATOR_H
