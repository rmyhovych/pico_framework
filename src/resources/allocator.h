//
// Created by rmyho on 6/7/2020.
//

#ifndef PICOFRAMEWORK_ALLOCATOR_H
#define PICOFRAMEWORK_ALLOCATOR_H

#include "alloc_objects.h"

class Instance;

class Device;


class Allocator
{
public:
	Allocator(const Instance &instance, const Device &device);

	~Allocator();

	void destroy();

	BufferAllocation createBuffer(VkBufferCreateInfo &bufferCreateInfo, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags = 0) const;

	ImageAllocation createImage(VkImageCreateInfo &imageCreateInfo, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags = 0) const;

	void free(BufferAllocation &obj) const;

	void free(ImageAllocation &obj) const;

	template<typename T>
	void* map(ObjectAllocation<T> &obj) const;

	template<typename T>
	void unmap(ObjectAllocation<T> &obj) const;

private:
	VmaAllocator handle_;
};

template<typename T>
void* Allocator::map(ObjectAllocation<T> &obj) const
{
	void* data;
	CALL_VK(vmaMapMemory(handle_, obj.allocation, &data))
	return data;
}

template<typename T>
void Allocator::unmap(ObjectAllocation<T> &obj) const
{
	vmaUnmapMemory(handle_, obj.allocation);
}

#endif //PICOFRAMEWORK_ALLOCATOR_H
