//
// Created by rmyho on 5/30/2020.
//

#ifndef PICOFRAMEWORK_ALLOCATOR_H
#define PICOFRAMEWORK_ALLOCATOR_H

#include "pfvk.h"
#include <map>
#include <unordered_map>


struct MemoryInfo
{
	uint32_t index;
	uint32_t offset;
	uint32_t size;
};

template<class T>
class Allocator
{
public:
	Allocator(VkDevice logicalDevice, VkDevice physicalDevice, uint32_t pageSize);

	void allocate(const T &key);

	void free(const T &key);

	void copyData(const T &k, void* data, uint32_t size);

private:
	int hash(const T& key);

private:
	const uint32_t PAGE_SIZE;

	VkDevice logicalDevice_;

	std::vector<VkDeviceMemory> memory_;
	std::map<int, MemoryInfo> memoryMap_;


};


#endif //PICOFRAMEWORK_ALLOCATOR_H
