//
// Created by rmyho on 5/31/2020.
//

#include "buffer_allocator.h"

BufferAllocator::BufferAllocator(VkDevice logicalDevice, VkPhysicalDevice physicalDevice) :
		Allocator<VkImage>(logicalDevice, physicalDevice, 4, 10)
{

}

void BufferAllocator::allocate(VkImage const &key)
{

}

void BufferAllocator::free(VkImage const &key)
{

}

int BufferAllocator::hash(VkImage const &key)
{
	return 0;
}

