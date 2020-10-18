//
// Created by ross on 2020-10-05.
//

#include "swapchain.h"

#include <algorithm>
#include <utility>

Swapchain::Builder::Builder(const Surface &surface, const Device &device, const ResourceFactory &resourceFactory) :
		surface_(surface),
		device_(device),
		resourceFactory_(resourceFactory)
{

}

Swapchain Swapchain::Builder::build(const SwapchainConfigurations &configurations) const
{
	VkSwapchainKHR handle = createHandle(configurations);
	std::vector<VkImage> swapchainImages = getImages(handle);

	VkFormat imageFormat = configurations.surfaceFormat.format;
	std::vector<VkImageView> swapchainImageViews(swapchainImages.size());
	for (VkImage image : swapchainImages)
	{
		resourceFactory_.createImageView(image, imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	return Swapchain(handle, swapchainImages, swapchainImageViews);
}

VkSwapchainKHR Swapchain::Builder::createHandle(const SwapchainConfigurations &configurations) const
{
	auto graphicsQueue = std::find_if(device_.deviceQueues_.begin(), device_.deviceQueues_.end(), [](const DeviceQueue &queue) { return queue.isType(VK_QUEUE_GRAPHICS_BIT); });

	if (graphicsQueue == device_.deviceQueues_.end())
		throw std::runtime_error("Missing VK_QUEUE_GRAPHICS_BIT queue in device!");

	if (!surface_.isQueueFamilySupported(*device_.pPhysicalDevice_, graphicsQueue->family))
		throw std::runtime_error("Graphics queue family swapchain not supported!");

	uint32_t nImages = configurations.surfaceCapabilities.minImageCount + 1;
	if (configurations.surfaceCapabilities.maxImageCount > 0 && nImages > configurations.surfaceCapabilities.maxImageCount)
		nImages = configurations.surfaceCapabilities.maxImageCount;

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface_.handle_;
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
	CALL_VK(vkCreateSwapchainKHR(device_.handle_, &createInfo, nullptr, &swapchainHandle))

	return swapchainHandle;
}

std::vector<VkImage> Swapchain::Builder::getImages(VkSwapchainKHR handle) const
{
	uint32_t nImages;
	vkGetSwapchainImagesKHR(device_.handle_, handle, &nImages, nullptr);
	std::vector<VkImage> images(nImages);
	vkGetSwapchainImagesKHR(device_.handle_, handle, &nImages, images.data());
	return images;
}

/*------------------------------------------------------------------------------------------------------------------------------*/

Swapchain::Swapchain(VkSwapchainKHR handle, std::vector<VkImage>& images, std::vector<VkImageView> imageViews) :
		handle_(handle),
		images_(std::move(images)),
		imageViews_(std::move(imageViews))
{
}

Swapchain::~Swapchain()
{
	CHECK_NULL_HANDLE(handle_)
}

void Swapchain::destroy(const Device &device, const ResourceFactory &resourceFactory)
{

}

std::vector<VkImageView> &Swapchain::getImageViews()
{
	return imageViews_;
}

