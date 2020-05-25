//
// Created by rmyho on 5/24/2020.
//

#include "image_factory.h"


ImageFactory::ImageFactory(VkDevice deviceHandle) :
		deviceHandle_(deviceHandle)
{
}


void ImageFactory::createImages(std::vector<VkImage>& destination, VkSwapchainKHR swapchainHandle)
{
	uint32_t nImages = 0;
	vkGetSwapchainImagesKHR(deviceHandle_, swapchainHandle, &nImages, nullptr);
	destination.resize(nImages);
	vkGetSwapchainImagesKHR(deviceHandle_, swapchainHandle, &nImages, destination.data());
}

void ImageFactory::createImageViews(std::vector<VkImageView> &destination, const std::vector<VkImage> &images, VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo viewCreateInfo = {};
	viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewCreateInfo.image = image;
	viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewCreateInfo.format = format;

	VkImageSubresourceRange& subresourceRange = viewCreateInfo.subresourceRange;
	subresourceRange.aspectMask = aspectFlags;
	subresourceRange.baseMipLevel = 0;
	subresourceRange.levelCount = 1;
	subresourceRange.baseArrayLayer = 0;
	subresourceRange.layerCount = 1;

	VkImageView imageView;
	CALL_VK(vkCreateImageView(deviceHandle_, &viewCreateInfo, nullptr, &imageView))

	return imageView;
}
