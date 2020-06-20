//
// Created by rmyho on 6/7/2020.
//

#ifndef PICOFRAMEWORK_ALLOCATOR_H
#define PICOFRAMEWORK_ALLOCATOR_H

#include <vk_mem_alloc.h>
#include "pfvk.h"

class Device;

struct ImageAllocation
{
	VkImage image;
	VmaAllocation allocation;
};

struct BufferAllocation
{
	VkBuffer buffer;
	VmaAllocation allocation;
};

class Allocator
{
public:
	class StagingCommand
	{
	public:

		BufferAllocation stageBuffer(BufferAllocation &source, VkBufferUsageFlags usage, VkDeviceSize size);

		ImageAllocation stageImage(VkBuffer buffer, uint32_t size);

	private:
		friend Allocator;

		StagingCommand(VkDevice device, VmaAllocator allocator, VkCommandBuffer transferCommandBuffer, VkQueue transferQueue, VmaMemoryUsage finalUsage);

		VkDevice deviceHandle_;
		VmaAllocator allocator_;

		VmaAllocationCreateInfo allocationCreateInfo_;

		VkCommandBuffer transferCommandBuffer_;

		VkQueue transferQueue_;
	};

	class Builder
	{
	public:
		Builder();

		Builder &setInstance(VkInstance instance);

		Builder &setDevices(VkDevice device, VkPhysicalDevice physicalDevice);

		Builder &setTransferData(VkCommandPool transferCommandPool, VkQueue transferQueue);

		Allocator* build() const;

	private:
		VkInstance instance_;
		VkDevice device_;
		VkPhysicalDevice physicalDevice_;
		VkCommandPool transferCommandPool_;
		VkQueue transferQueue_;
	};

	~Allocator();

	BufferAllocation createBuffer(VkBufferCreateInfo &bufferCreateInfo, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags = 0) const;

	ImageAllocation createImage(VkImageCreateInfo &imageCreateInfo, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags = 0) const;

	void free(BufferAllocation &allocation) const;

	void free(ImageAllocation &allocation) const;

	void* map(VmaAllocation allocation);

	void unmap(VmaAllocation allocation);

	StagingCommand* createStagingCommand(VmaMemoryUsage finalUsage) const;

	void executeStagingCommand(StagingCommand* stagingCommand) const;

private:
	Allocator(VkInstance instance, VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool transferCommandPool, VkQueue transferQueue);

	VkDevice device_;

	VkCommandPool transferCommandPool_;
	VkQueue transferQueue_;

	VmaAllocator vmaAllocator_;
};


#endif //PICOFRAMEWORK_ALLOCATOR_H
