//
// Created by rmyho on 6/7/2020.
//

#ifndef PICOFRAMEWORK_STAGING_ALLOCATOR_H
#define PICOFRAMEWORK_STAGING_ALLOCATOR_H

#include "device.h"

class StagingAllocator
{
public:
	StagingAllocator(Device* pDevice, VmaMemoryUsage finalUsage);

private:
	Allocator* pAllocator_;
	VmaMemoryUsage finalUsage_;

	VkCommandPool commandPool_;
	VkCommandBuffer commandBuffer_;
};


#endif //PICOFRAMEWORK_STAGING_ALLOCATOR_H
