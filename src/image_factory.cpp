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

VkFormat ImageFactory::getSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags featureFlags)
{
	VkPhysicalDevice physicalDeviceHandle = pDevice_->getPhysiacalDevice();

	for (VkFormat format : candidates)
	{
		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(physicalDeviceHandle, format, &properties);

		if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & featureFlags) == featureFlags)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & featureFlags) == featureFlags)
		{
			return format;
		}

		throw std::exception("Failed to find supported format.");
	}
}

VkFormat ImageFactory::getDepthFormat()
{
	return getSupportedFormat(
			{VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT},
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}
