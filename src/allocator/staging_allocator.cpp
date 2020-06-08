//
// Created by rmyho on 6/7/2020.
//

#include "staging_allocator.h"

StagingAllocator::StagingAllocator(Device* pDevice, VmaMemoryUsage finalUsage) :
		pAllocator_(pAllocator),
		finalUsage_(finalUsage)
{

}


void StagingAllocator::stage(BufferAllocation* finalDestination, VkBuffer buffer, uint32_t size)
{

}


void StagingAllocator::stage(ImageAllocation* finalDestination, VkImage image, uint32_t size)
{

}
