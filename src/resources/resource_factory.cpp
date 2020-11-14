//
// Created by rmyho on 6/7/2020.
//

#include "resource_factory.h"

ResourceFactory::ResourceFactory(VkInstance hInstance, VkDevice hDevice, VkPhysicalDevice hPhysicalDevice, const DeviceQueue* pTransferDeviceQueue) :
		hDevice_(hDevice),

		allocator_(hInstance, hDevice, hPhysicalDevice),
		transferManager_(hDevice, pTransferDeviceQueue),


		createdBuffers_(),
		createdImages_(),
		createdImageViews_()
{
}


ResourceFactory::~ResourceFactory()
{
	for (auto bufferAlloc : createdBuffers_)
	{
		allocator_.free(bufferAlloc.first, bufferAlloc.second);
	}
	createdBuffers_.clear();

	for (auto imageAlloc : createdImages_)
	{
		allocator_.free(imageAlloc.first, imageAlloc.second);
	}
	createdImages_.clear();

	for (auto imageView : createdImageViews_)
	{
		vkDestroyImageView(hDevice_, imageView, nullptr);
	}
	createdImageViews_.clear();
}


VkBuffer ResourceFactory::createBuffer(VkDeviceSize size, VkBufferUsageFlags usageFlags, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags)
{
	VkBufferCreateInfo bufferCreateInfo{};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = size;
	bufferCreateInfo.usage = usageFlags;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferCreateInfo.queueFamilyIndexCount = 0;
	bufferCreateInfo.pQueueFamilyIndices = nullptr;
	bufferCreateInfo.flags = 0;

	std::pair<VkBuffer, VmaAllocation> bufferAllocation = allocator_.createBuffer(bufferCreateInfo, memoryUsage, flags);
	createdBuffers_.insert(bufferAllocation);

	return bufferAllocation.first;
}

void ResourceFactory::destroyBuffer(VkBuffer buffer)
{
	allocator_.free(buffer, createdBuffers_.at(buffer));
	createdBuffers_.erase(buffer);
}


VkImage ResourceFactory::createImage(VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlags, VmaMemoryUsage memoryUsage)
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

	std::pair<VkImage, VmaAllocation> imageAllocation = allocator_.createImage(imageCreateInfo, memoryUsage, 0);
	createdImages_.insert(imageAllocation);
	return imageAllocation.first;
}

void ResourceFactory::destroyImage(VkImage image)
{
	allocator_.free(image, createdImages_.at(image));
	createdImages_.erase(image);
}


VkImageView ResourceFactory::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
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
	CALL_VK(vkCreateImageView(hDevice_, &viewCreateInfo, nullptr, &imageView))
	createdImageViews_.insert(imageView);

	return imageView;
}

void ResourceFactory::destroyImageView(VkImageView imageView)
{
	vkDestroyImageView(hDevice_, imageView, nullptr);
	createdImageViews_.erase(imageView);
}


void* ResourceFactory::map(VkBuffer buffer) const
{
	return allocator_.map(createdBuffers_.at(buffer));
}


void ResourceFactory::unmap(VkBuffer buffer) const
{
	allocator_.unmap(createdBuffers_.at(buffer));
}


