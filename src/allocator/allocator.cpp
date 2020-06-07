//
// Created by rmyho on 6/6/2020.
//

#include "allocator.h"

Allocator::Allocator(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, uint32_t memoryTypeIndex, uint32_t pageSize) :
		logicalDevice_(logicalDevice),
		physicalDevice_(physicalDevice),

		memory_(VK_NULL_HANDLE),
		buddyTree_(pageSize, 0)
{
	VkMemoryAllocateInfo memoryAllocateInfo{};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;

	vkAllocateMemory(logicalDevice, )
}

void Allocator::copyData(const T &k, void* data, uint32_t size)
{
	const MemoryObject &info = memoryMap_[hash(k)];
	VkDeviceMemory m = memory_[info.index];

	void* dataPtr;
	vkMapMemory(logicalDevice_, m, info.offset, info.size, 0, &dataPtr);
	memcpy(dataPtr, data, size);
	vkUnmapMemory(logicalDevice_, m);
}


uint32_t Allocator::getMemoryType(uint32_t memoryTypeFilter, VkMemoryPropertyFlags memoryPropertyFlags)
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

	throw std::exception("Failed to find suitable memory type.");
}
