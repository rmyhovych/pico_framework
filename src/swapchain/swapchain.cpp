//
// Created by ross on 2020-10-05.
//

#include "swapchain.h"

#include <algorithm>
#include <utility>

Swapchain::Builder::Builder(VkDevice hDevice, const DeviceQueue* pDeviceQueue, const Surface* pSurface, ResourceFactory* pResourceFactory) :
		hDevice_(hDevice),
		pSurface_(pSurface),
		pResourceFactory_(pResourceFactory)
{
	if (!pSurface->isQueueFamilySupported(pDeviceQueue->family))
		throw std::runtime_error("Graphics queue family swapchain not supported!");
}

Swapchain Swapchain::Builder::build(const SwapchainConfigurations& configurations) const
{
	VkSwapchainKHR handle = createHandle(configurations);
	std::vector<VkImage> swapchainImages = getImages(handle);

	VkFormat imageFormat = configurations.surfaceFormat.format;
	std::vector<VkImageView> swapchainImageViews(swapchainImages.size());
	for (uint32_t i = 0; i < swapchainImages.size(); ++i)
	{
		swapchainImageViews[i] = pResourceFactory_->createImageView(swapchainImages[i], imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	return Swapchain(hDevice_, pResourceFactory_, handle, swapchainImageViews);
}

VkSwapchainKHR Swapchain::Builder::createHandle(const SwapchainConfigurations& configurations) const
{
	uint32_t nImages = configurations.surfaceCapabilities.minImageCount + 1;
	if (configurations.surfaceCapabilities.maxImageCount > 0 && nImages > configurations.surfaceCapabilities.maxImageCount)
		nImages = configurations.surfaceCapabilities.maxImageCount;

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = pSurface_->handle_;
	createInfo.minImageCount = nImages;
	createInfo.imageFormat = configurations.surfaceFormat.format;
	createInfo.imageColorSpace = configurations.surfaceFormat.colorSpace;
	createInfo.imageExtent = configurations.extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices = nullptr;
	createInfo.preTransform = configurations.surfaceCapabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = configurations.presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = nullptr;

	VkSwapchainKHR swapchainHandle;
	CALL_VK(vkCreateSwapchainKHR(hDevice_, &createInfo, nullptr, &swapchainHandle))

	return swapchainHandle;
}

std::vector<VkImage> Swapchain::Builder::getImages(VkSwapchainKHR handle) const
{
	uint32_t nImages;
	vkGetSwapchainImagesKHR(hDevice_, handle, &nImages, nullptr);
	std::vector<VkImage> images(nImages);
	vkGetSwapchainImagesKHR(hDevice_, handle, &nImages, images.data());
	return images;
}

/*------------------------------------------------------------------------------------------------------------------------------*/

Swapchain::Swapchain(VkDevice hDevice, ResourceFactory* pResourceFactory, VkSwapchainKHR handle, std::vector<VkImageView> imageViews) :
		hDevice_(hDevice),
		pResourceFactory_(pResourceFactory),

		handle_(handle),
		imageViews_(std::move(imageViews)),

		depthImage_(VK_NULL_HANDLE),
		depthImageView_(VK_NULL_HANDLE)
{
}

Swapchain::~Swapchain()
{
	for (VkImageView imageView : imageViews_)
		pResourceFactory_->destroyImageView(imageView);
	imageViews_.clear();

	vkDestroySwapchainKHR(hDevice_, handle_, nullptr);
	handle_ = VK_NULL_HANDLE;
}

std::vector<std::vector<VkImageView>> Swapchain::getAttachments() const
{
	std::vector<std::vector<VkImageView>> attachments(imageViews_.size());
	std::transform(imageViews_.begin(), imageViews_.end(), attachments.begin(), [](VkImageView imageView) -> std::vector<VkImageView>
	{
		std::vector<VkImageView> attachment;
		attachment.push_back(imageView);
		return attachment;
	});

	return attachments;
}

