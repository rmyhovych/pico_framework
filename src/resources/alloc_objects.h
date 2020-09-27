//
// Created by ross on 2020-09-27.
//

#ifndef PICOFRAMEWORK_ALLOC_OBJECTS_H
#define PICOFRAMEWORK_ALLOC_OBJECTS_H

#include "pfvk.h"
#include <vk_mem_alloc.h>

template<class T>
struct ObjectAllocation
{
	T handle;
	VmaAllocation allocation;
};

struct ImageAllocation : public ObjectAllocation<VkImage>
{
};

struct BufferAllocation : public ObjectAllocation<VkBuffer>
{
};


#endif //PICOFRAMEWORK_ALLOC_OBJECTS_H
