//
// Created by rmyho on 6/7/2020.
//

#define VMA_IMPLEMENTATION

#include "allocator.h"
#include "device.h"


Allocator::StagingCommand::StagingCommand(VkDevice device, VmaAllocator allocator, VkCommandBuffer transferCommandBuffer, VkQueue transferQueue, VmaMemoryUsage finalUsage) :
		deviceHandle_(device),
		allocator_(allocator),
		allocationCreateInfo_({}),

		transferCommandBuffer_(transferCommandBuffer),
		transferQueue_(transferQueue)
{
	allocationCreateInfo_.usage = finalUsage;
	allocationCreateInfo_.flags = 0;

	VkCommandBufferBeginInfo cmdBufferBeginInfo = {};
	cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	CALL_VK(vkBeginCommandBuffer(transferCommandBuffer_, &cmdBufferBeginInfo))
}

BufferAllocation Allocator::StagingCommand::stageBuffer(BufferAllocation &source, VkBufferUsageFlags usage, VkDeviceSize size)
{
	VkBufferCreateInfo bufferCreateInfo{};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage;
	bufferCreateInfo.flags = 0;
	bufferCreateInfo.size = size;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	BufferAllocation allocation{};
	CALL_VK(vmaCreateBuffer(allocator_, &bufferCreateInfo, &allocationCreateInfo_, &allocation.buffer, &allocation.allocation, nullptr))

	VkBufferCopy bufferCopy{};
	bufferCopy.dstOffset = 0;
	bufferCopy.srcOffset = 0;
	bufferCopy.size = size;
	vkCmdCopyBuffer(transferCommandBuffer_, source.buffer, allocation.buffer, 1, &bufferCopy);

	return allocation;
}

ImageAllocation Allocator::StagingCommand::stageImage(VkBuffer buffer, uint32_t size)
{
	return ImageAllocation();
}


/**------------------------------------------------------------------------------------------------------------------------*/


Allocator::Builder::Builder()
{

}

Allocator::Builder &Allocator::Builder::setInstance(VkInstance instance)
{
	instance_ = instance;
	return *this;
}

Allocator::Builder &Allocator::Builder::setDevices(VkDevice device, VkPhysicalDevice physicalDevice)
{
	device_ = device;
	physicalDevice_ = physicalDevice;
	return *this;
}

Allocator::Builder &Allocator::Builder::setTransferData(VkCommandPool transferCommandPool, VkQueue transferQueue)
{
	transferCommandPool_ = transferCommandPool;
	transferQueue_ = transferQueue;
	return *this;
}

Allocator* Allocator::Builder::build() const
{
	if (instance_ == VK_NULL_HANDLE ||
	    device_ == VK_NULL_HANDLE ||
	    physicalDevice_ == VK_NULL_HANDLE ||
	    transferCommandPool_ == VK_NULL_HANDLE ||
	    transferQueue_ == VK_NULL_HANDLE)
	{
		throw std::exception("Missing parameter for allocator building!");
	}

	return new Allocator(instance_, device_, physicalDevice_, transferCommandPool_, transferQueue_);
}

/**------------------------------------------------------------------------------------------------------------------------*/

Allocator::Allocator(VkInstance instance, VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool transferCommandPool, VkQueue transferQueue) :
		device_(device),

		transferCommandPool_(transferCommandPool),
		transferQueue_(transferQueue),

		vmaAllocator_(VMA_NULL)
{
	transferCommandPool_ = transferCommandPool;

	VmaAllocatorCreateInfo allocatorCreateInfo{};
	allocatorCreateInfo.instance = instance;
	allocatorCreateInfo.device = device_;
	allocatorCreateInfo.physicalDevice = physicalDevice;

	allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXTERNALLY_SYNCHRONIZED_BIT;
	allocatorCreateInfo.pAllocationCallbacks = nullptr;
	allocatorCreateInfo.pDeviceMemoryCallbacks = nullptr;
	allocatorCreateInfo.frameInUseCount = 0;
	allocatorCreateInfo.pHeapSizeLimit = nullptr;
	allocatorCreateInfo.pVulkanFunctions = nullptr;
	allocatorCreateInfo.pRecordSettings = nullptr;
	allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_0;

	CALL_VK(vmaCreateAllocator(&allocatorCreateInfo, &vmaAllocator_))
}

Allocator::~Allocator()
{
	vmaDestroyAllocator(vmaAllocator_);
	vkDestroyCommandPool(device_, transferCommandPool_, nullptr);
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


void Allocator::free(BufferAllocation &allocation) const
{
	vmaDestroyBuffer(vmaAllocator_, allocation.buffer, allocation.allocation);
}

void Allocator::free(ImageAllocation &allocation) const
{
	vmaDestroyImage(vmaAllocator_, allocation.image, allocation.allocation);
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

Allocator::StagingCommand* Allocator::createStagingCommand(VmaMemoryUsage finalUsage) const
{
	VkCommandBufferAllocateInfo cmdBufferAllocateInfo = {};
	cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufferAllocateInfo.commandPool = transferCommandPool_;
	cmdBufferAllocateInfo.commandBufferCount = 1;

	VkCommandBuffer transferCommandBuffer;
	CALL_VK(vkAllocateCommandBuffers(device_, &cmdBufferAllocateInfo, &transferCommandBuffer))

	return new StagingCommand(device_, vmaAllocator_, transferCommandBuffer, transferQueue_, finalUsage);
}

void Allocator::executeStagingCommand(StagingCommand* stagingCommand) const
{
	CALL_VK(vkEndCommandBuffer(stagingCommand->transferCommandBuffer_))

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &stagingCommand->transferCommandBuffer_;

	vkQueueSubmit(transferQueue_, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(transferQueue_);

	vkFreeCommandBuffers(device_, transferCommandPool_, 1, &stagingCommand->transferCommandBuffer_);

	delete stagingCommand;
}
