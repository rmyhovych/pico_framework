//
// Created by rmyho on 5/30/2020.
//

#include "allocator.h"

template<class T>
Allocator<T>::Allocator(VkDevice logicalDevice, VkDevice physicalDevice, uint32_t pageSize) :
		PAGE_SIZE(pageSize),
		logicalDevice_(logicalDevice),

		memory_(0),
		memoryMap_(0)
{
}


template<class T>
void Allocator<T>::copyData(const T &k, void* data, uint32_t size)
{
	const MemoryInfo &info = memoryMap_[hash(k)];
	VkDeviceMemory m = memory_[info.index];

	void* dataPtr;
	vkMapMemory(logicalDevice_, m, info.offset, info.size, 0, &dataPtr);
	memcpy(dataPtr, data, size);
	vkUnmapMemory(logicalDevice_, m);
}
