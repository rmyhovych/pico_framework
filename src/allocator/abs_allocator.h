//
// Created by rmyho on 5/30/2020.
//

#ifndef PICOFRAMEWORK_ABS_ALLOCATOR_H
#define PICOFRAMEWORK_ABS_ALLOCATOR_H

#include "pfvk.h"
#include <map>
#include <unordered_map>
#include <utility>

#include "buddy_tree.h"

struct MemoryInfo
{
	uint32_t index;
	uint32_t offset;
	uint32_t size;
};

template<class T>
class AbsAllocator
{
public:
	AbsAllocator(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, int pageSize, int orderSize);

	virtual void allocate(const T &key) = 0;

	virtual void free(const T &key) = 0;

	void copyData(const T &k, void* data, uint32_t size);

private:
	virtual int hash(const T &key) = 0;

	virtual void getMemoryRequirements(const T &key, VkMemoryRequirements* memoryRequirements) = 0;

	uint32_t getMemoryType(uint32_t memoryTypeFilter, VkMemoryPropertyFlags memoryPropertyFlags);

	std::pair<VkDeviceMemory, BuddyTree> &createNewMemorySlot();

private:
	VkDevice logicalDevice_;
	VkPhysicalDevice physicalDevice_;

	const int PAGE_SIZE_;
	const int ORDER_SIZE_;

	std::vector<std::pair<VkDeviceMemory, BuddyTree>> memory_;
	std::map<int, MemoryInfo> memoryMap_;
};

/**-----------------------------------------------------------------------------------------------------------------------------*/

template<class T>
AbsAllocator<T>::AbsAllocator(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, int pageSize, int orderSize) :
		logicalDevice_(logicalDevice),
		physicalDevice_(physicalDevice),

		PAGE_SIZE_(pageSize),
		ORDER_SIZE_(orderSize),

		allocationTree_(pageSize, orderSize),

		memory_(0),
{
	createNewMemorySlot();
}

template<class T>
void AbsAllocator<T>::copyData(const T &k, void* data, uint32_t size)
{
	const MemoryInfo &info = memoryMap_[hash(k)];
	VkDeviceMemory m = memory_[info.index];

	void* dataPtr;
	vkMapMemory(logicalDevice_, m, info.offset, info.size, 0, &dataPtr);
	memcpy(dataPtr, data, size);
	vkUnmapMemory(logicalDevice_, m);
}


template<class T>
uint32_t AbsAllocator<T>::getMemoryType(uint32_t memoryTypeFilter, VkMemoryPropertyFlags memoryPropertyFlags)
{
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice_, &memoryProperties);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
	{
		if (memoryTypeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags)
		{
			return i;
		}
	}

	throw VulkanException("Failed to find suitable memory type.");
}

template<class T>
std::pair<VkDeviceMemory, BuddyTree> &AbsAllocator<T>::createNewMemorySlot()
{
	memory_.push_back(std::pair<VkDeviceMemory, BuddyTree>(VK_NULL_HANDLE, BuddyTree(PAGE_SIZE_, ORDER_SIZE_)));
	std::pair<VkDeviceMemory, BuddyTree> &newMemory = memory_.back();

	VkMemoryRequirements memoryRequirements;
	getMemoryRequirements()

	VkMemoryAllocateInfo allocateInfo{};
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.

			vkAllo
	newMemory.first
}


#endif //PICOFRAMEWORK_ABS_ALLOCATOR_H
