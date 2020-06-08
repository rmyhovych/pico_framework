//
// Created by rmyho on 6/7/2020.
//

#define VMA_IMPLEMENTATION

#include "allocator.h"
#include "device.h"


Allocator::StagingCommand::StagingCommand(VkDevice deviceHandle, VmaAllocator allocator, VkCommandPool commandPool, VmaMemoryUsage finalUsage) :
		deviceHandle_(deviceHandle),
		allocator_(allocator),
		commandPool_(commandPool),
		allocationCreateInfo_({}),

		commandBuffer_(VK_NULL_HANDLE)
{
	allocationCreateInfo_.usage = finalUsage;
	allocationCreateInfo_.flags = 0;

	VkCommandBufferAllocateInfo  cmdBufferAllocateInfo = {};
	cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufferAllocateInfo.commandPool = commandPool;
	cmdBufferAllocateInfo.commandBufferCount = 1;

	CALL_VK(vkAllocateCommandBuffers(deviceHandle, &cmdBufferAllocateInfo, &commandBuffer_))

	VkCommandBufferBeginInfo cmdBufferBeginInfo = {};
	cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	CALL_VK(vkBeginCommandBuffer(commandBuffer_, &cmdBufferBeginInfo))
}

BufferAllocation Allocator::StagingCommand::stageBuffer(VkBuffer buffer, VkBufferUsageFlags usage, VkBufferCopy bufferCopy)
{
	VkBufferCreateInfo bufferCreateInfo{};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage;
	bufferCreateInfo.flags = 0;

	BufferAllocation allocation{};
	CALL_VK(vmaCreateBuffer(allocator_, &bufferCreateInfo, &allocationCreateInfo_, &allocation.buffer, &allocation.allocation, nullptr))



	return allocation;
}

ImageAllocation Allocator::StagingCommand::stageImage(VkBuffer buffer, uint32_t size)
{
	return ImageAllocation();
}

void Allocator::StagingCommand::execute()
{
	CALL_VK(vkEndCommandBuffer(commandBuffer_));

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer_;

	vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(m_graphicsQueue);

	vkFreeCommandBuffers(m_logicalDevice, m_commandPool, 1, &commandBuffer);
}


/**------------------------------------------------------------------------------------------------------------------------*/


Allocator::Allocator() :
		pDevice_(nullptr),
		vmaAllocator_(VMA_NULL),
		commandPool_(VK_NULL_HANDLE)
{

}

void Allocator::init(VkInstance instance, Device* pDevice)
{
	pDevice_ = pDevice;

	VmaAllocatorCreateInfo allocatorCreateInfo{};
	allocatorCreateInfo.instance = instance;
	allocatorCreateInfo.device = pDevice->getHandle();
	allocatorCreateInfo.physicalDevice = pDevice->getPhysicalDevice().getHandle();

	allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXTERNALLY_SYNCHRONIZED_BIT;
	allocatorCreateInfo.pAllocationCallbacks = nullptr;
	allocatorCreateInfo.pDeviceMemoryCallbacks = nullptr;
	allocatorCreateInfo.frameInUseCount = 0;
	allocatorCreateInfo.pHeapSizeLimit = nullptr;
	allocatorCreateInfo.pVulkanFunctions = nullptr;
	allocatorCreateInfo.pRecordSettings = nullptr;
	allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_0;

	CALL_VK(vmaCreateAllocator(&allocatorCreateInfo, &vmaAllocator_))


	commandPool_ = pDevice->createCommandPool(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
}

void Allocator::destroy()
{
	vmaDestroyAllocator(vmaAllocator_);
	vkDestroyCommandPool(pDevice_->getHandle(), commandPool_, nullptr);
}


BufferAllocation Allocator::createBuffer(VkBufferCreateInfo &bufferCreateInfo, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags) const
{
	VmaAllocationCreateInfo allocationCreateInfo{};
	allocationCreateInfo.usage = memoryUsage;
	allocationCreateInfo.flags = flags;

	BufferAllocation allocation{};
	CALL_VK(vmaCreateBuffer(vmaAllocator_, &bufferCreateInfo, &allocationCreateInfo, &allocation.buffer, &allocation.allocation, nullptr))

	return allocation;
}

ImageAllocation Allocator::createImage(VkImageCreateInfo &imageCreateInfo, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags) const
{
	VmaAllocationCreateInfo allocationCreateInfo{};
	allocationCreateInfo.usage = memoryUsage;
	allocationCreateInfo.flags = flags;

	ImageAllocation allocation{};
	CALL_VK(vmaCreateImage(vmaAllocator_, &imageCreateInfo, &allocationCreateInfo, &allocation.image, &allocation.allocation, nullptr))

	return allocation;
}

void* Allocator::map(VmaAllocation allocation)
{
	void* data;
	CALL_VK(vmaMapMemory(vmaAllocator_, allocation, &data))
	return data;
}

void Allocator::unmap(VmaAllocation allocation)
{
	vmaUnmapMemory(vmaAllocator_, allocation);
}
