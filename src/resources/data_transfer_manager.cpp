//
// Created by ross on 2020-11-10.
//

#include "data_transfer_manager.h"


DataTransferManager::DataTransferManager(VkDevice hDevice, const DeviceQueue* pDeviceQueue) :
		hDevice_(hDevice),
		hQueue_(pDeviceQueue->queue),

		transferCmdPool_(VK_NULL_HANDLE),
		transferCmdBuffer_(VK_NULL_HANDLE)
{
	VkCommandPoolCreateInfo commandPoolCreateInfo{};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
	commandPoolCreateInfo.pNext = nullptr;
	commandPoolCreateInfo.queueFamilyIndex = pDeviceQueue->family;

	CALL_VK(vkCreateCommandPool(hDevice_, &commandPoolCreateInfo, nullptr, &transferCmdPool_))

}

DataTransferManager::~DataTransferManager()
{
	if (transferCmdBuffer_ != VK_NULL_HANDLE)
	{
		vkFreeCommandBuffers(hDevice_, transferCmdPool_, 1, &transferCmdBuffer_);
		transferCmdBuffer_ = VK_NULL_HANDLE;
	}

	if (transferCmdPool_ != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(hDevice_, transferCmdPool_, nullptr);
		transferCmdPool_ = VK_NULL_HANDLE;
	}
}


DataTransferManager& DataTransferManager::begin()
{
	VkCommandBufferAllocateInfo cmdBufferAllocateInfo{};
	cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufferAllocateInfo.commandPool = transferCmdPool_;
	cmdBufferAllocateInfo.commandBufferCount = 1;
	CALL_VK(vkAllocateCommandBuffers(hDevice_, &cmdBufferAllocateInfo, &transferCmdBuffer_))

	VkCommandBufferBeginInfo cmdBufferBeginInfo{};
	cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	CALL_VK(vkBeginCommandBuffer(transferCmdBuffer_, &cmdBufferBeginInfo))

	return *this;
}

void DataTransferManager::submit()
{
	CALL_VK(vkEndCommandBuffer(transferCmdBuffer_))

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &transferCmdBuffer_;

	vkQueueSubmit(hQueue_, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(hQueue_);

	vkFreeCommandBuffers(hDevice_, transferCmdPool_, 1, &transferCmdBuffer_);
	transferCmdBuffer_ = VK_NULL_HANDLE;
}

DataTransferManager& DataTransferManager::transfer(VkBuffer src, VkBuffer dst, VkDeviceSize size)
{
	VkBufferCopy bufferCopy{};
	bufferCopy.size = size;
	bufferCopy.srcOffset = 0;
	bufferCopy.dstOffset = 0;
	vkCmdCopyBuffer(transferCmdBuffer_, src, dst, 1, &bufferCopy);

	return *this;
}
