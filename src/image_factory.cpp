//
// Created by rmyho on 5/24/2020.
//

#include "image_factory.h"


ImageFactory::ImageFactory(const Device* pDevice) :
		pDevice_(pDevice)
{
}


void ImageFactory::createImages(std::vector<VkImage> &destination, VkSwapchainKHR swapchainHandle)
{
	VkDevice deviceHandle = pDevice_->getHandle();

	uint32_t nImages = 0;
	vkGetSwapchainImagesKHR(deviceHandle, swapchainHandle, &nImages, nullptr);
	destination.resize(nImages);
	vkGetSwapchainImagesKHR(deviceHandle, swapchainHandle, &nImages, destination.data());
}

void ImageFactory::createImageViews(std::vector<VkImageView> &destination, const std::vector<VkImage> &images, VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkDevice deviceHandle = pDevice_->getHandle();

	uint32_t nImages = images.size();
	destination.resize(nImages);
	for (uint32_t i = 0; i < nImages; ++i)
	{
		VkImageViewCreateInfo viewCreateInfo = {};
		viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewCreateInfo.image = images[i];
		viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewCreateInfo.format = format;

		VkImageSubresourceRange &subresourceRange = viewCreateInfo.subresourceRange;
		subresourceRange.aspectMask = aspectFlags;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;

		CALL_VK(vkCreateImageView(deviceHandle, &viewCreateInfo, nullptr, &destination[i]))
	}
}
