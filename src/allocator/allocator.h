//
// Created by rmyho on 5/30/2020.
//

#ifndef PICOFRAMEWORK_ALLOCATOR_H
#define PICOFRAMEWORK_ALLOCATOR_H

#include "pfvk.h"
#include <map>
#include <unordered_map>

#include "allocator/tree/buddy_tree.h"

struct MemoryObject
{
	uint32_t index;
	uint32_t offset;
	uint32_t size;
};


class Allocator
{
public:
	Allocator(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, uint32_t memoryTypeIndex, uint32_t pageSize);

	BuddyNode* pickSpot(uint32_t size);

	void freeSpot(BuddyNode* node);

	void copyData(const MemoryObject &k, void* data, uint32_t size);

private:
	virtual void getMemoryRequirements(const T &key, VkMemoryRequirements* memoryRequirements) = 0;

	uint32_t getMemoryType(uint32_t memoryTypeFilter, VkMemoryPropertyFlags memoryPropertyFlags);

private:
	VkDevice logicalDevice_;
	VkPhysicalDevice physicalDevice_;

	VkDeviceMemory memory_;
	BuddyTree buddyTree_;
};

#endif //PICOFRAMEWORK_ALLOCATOR_H
