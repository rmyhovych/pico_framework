//
// Created by rmyho on 6/7/2020.
//

#include "resource_factory.h"

ResourceFactory::ResourceFactory(const Device* pDevice, const Allocator* pAllocator) :
		pDevice_(pDevice),
		pAllocator_(pAllocator),
		pTransferQueue_(pDevice->getQueue(VK_QUEUE_TRANSFER_BIT))
{
	if (pTransferQueue_ == nullptr)
		throw std::runtime_error("Missing transfer queue in Device!");

	VkCommandPoolCreateInfo commandPoolCreateInfo{};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
	commandPoolCreateInfo.pNext = nullptr;
	commandPoolCreateInfo.queueFamilyIndex = pTransferQueue_->family;

	CALL_VK(vkCreateCommandPool(pDevice->handle_, &commandPoolCreateInfo, nullptr, &transferCommandPool_))
}


ResourceFactory::~ResourceFactory()
{
	CHECK_NULL_HANDLE(transferCommandPool_)
}

void ResourceFactory::destroy()
{
	vkDestroyCommandPool(pDevice_->handle_, transferCommandPool_, nullptr);
	transferCommandPool_ = VK_NULL_HANDLE;
}


BufferAllocation ResourceFactory::createBuffer(VkDeviceSize size, VkBufferUsageFlags usageFlags, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags) const
{
	VkBufferCreateInfo bufferCreateInfo{};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = size;
	bufferCreateInfo.usage = usageFlags;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferCreateInfo.queueFamilyIndexCount = 0;
	bufferCreateInfo.pQueueFamilyIndices = nullptr;
	bufferCreateInfo.flags = 0;

	return pAllocator_->createBuffer(bufferCreateInfo, memoryUsage, flags);
}

void ResourceFactory::destroyBuffer(BufferAllocation &buffer) const
{
	pAllocator_->free(buffer);
}


ImageAllocation ResourceFactory::createImage(VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlags, VmaMemoryUsage memoryUsage) const
{
	VkImageCreateInfo imageCreateInfo{};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.extent = VkExtent3D{extent.width, extent.height, 1};
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.format = format;
	imageCreateInfo.tiling = tiling;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageCreateInfo.usage = usageFlags;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCreateInfo.flags = 0;

	return pAllocator_->createImage(imageCreateInfo, memoryUsage, 0);
}

void ResourceFactory::destroyImage(ImageAllocation &image) const
{
	pAllocator_->free(image);
}


VkImageView ResourceFactory::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) const
{
	VkImageViewCreateInfo viewCreateInfo = {};
	viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewCreateInfo.image = image;
	viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewCreateInfo.format = format;

	VkImageSubresourceRange &subresourceRange = viewCreateInfo.subresourceRange;
	subresourceRange.aspectMask = aspectFlags;
	subresourceRange.baseMipLevel = 0;
	subresourceRange.levelCount = 1;
	subresourceRange.baseArrayLayer = 0;
	subresourceRange.layerCount = 1;

	VkImageView imageView;
	CALL_VK(vkCreateImageView(pDevice_->handle_, &viewCreateInfo, nullptr, &imageView))
	return imageView;
}

void ResourceFactory::destroyImageView(VkImageView imageView) const
{
	vkDestroyImageView(pDevice_->handle_, imageView, nullptr);
}

VkCommandBuffer ResourceFactory::createTransferCommandBuffer() const
{
	VkCommandBufferAllocateInfo cmdBufferAllocateInfo = {};
	cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufferAllocateInfo.commandPool = transferCommandPool_;
	cmdBufferAllocateInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	CALL_VK(vkAllocateCommandBuffers(pDevice_->handle_, &cmdBufferAllocateInfo, &commandBuffer))

	VkCommandBufferBeginInfo cmdBufferBeginInfo = {};
	cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	CALL_VK(vkBeginCommandBuffer(commandBuffer, &cmdBufferBeginInfo))

	return commandBuffer;
}

void ResourceFactory::submitTransferCommandBuffer(VkCommandBuffer transferCommandBuffer) const
{
	CALL_VK(vkEndCommandBuffer(transferCommandBuffer))

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &transferCommandBuffer;

	vkQueueSubmit(pTransferQueue_->queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(pTransferQueue_->queue);

	vkFreeCommandBuffers(pDevice_->handle_, transferCommandPool_, 1, &transferCommandBuffer);
}
