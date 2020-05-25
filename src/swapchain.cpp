//
// Created by rmyho on 5/24/2020.
//

#include "swapchain.h"
#include "image_factory.h"

Swapchain::Swapchain(const Swapchain::Properties &properties, VkSurfaceKHR surfaceHandle, Device* pDevice) :
		surfaceHandle_(surfaceHandle),
		pDevice_(pDevice),

		handle_(VK_NULL_HANDLE),

		isDepth_(properties.isDepth)
{
}

Swapchain::~Swapchain()
{
	if (handle_ != VK_NULL_HANDLE)
	{
		destroy();
	}
}

void Swapchain::init(const SwapchainConfigurations &swapchainConfigurations)
{
	handle_ = createHandle(swapchainConfigurations, pDevice_, surfaceHandle_);

	ImageFactory imageFactory(pDevice_->getHandle());
	imageFactory.createImages(images_, handle_);
}

void Swapchain::reset(const SwapchainConfigurations &swapchainConfigurations)
{
	destroy();
	init(swapchainConfigurations);
}

void Swapchain::destroy()
{
	vkDestroySwapchainKHR(pDevice_->getHandle(), handle_, nullptr);
	handle_ = VK_NULL_HANDLE;
}

VkSwapchainKHR Swapchain::createHandle(const SwapchainConfigurations& swapchainConfigurations, const Device* pDevice, VkSurfaceKHR surfaceHandle)
{
	// minImageCount + 1 to avoid waiting
	uint32_t nImages = swapchainConfigurations.surfaceCapabilities.minImageCount + 1;

	// VkSurfaceCapabilitiesKHR::maxImageCount == 0 --> No max
	if (swapchainConfigurations.surfaceCapabilities.maxImageCount > 0 && nImages > swapchainConfigurations.surfaceCapabilities.maxImageCount)
	{
		nImages = swapchainConfigurations.surfaceCapabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surfaceHandle;
	createInfo.minImageCount = nImages;
	createInfo.imageFormat = swapchainConfigurations.surfaceFormat.format;
	createInfo.imageColorSpace = swapchainConfigurations.surfaceFormat.colorSpace;
	createInfo.imageExtent = swapchainConfigurations.extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	const QueueFamilyIndexes& deviceQueueFamilyIndexes = pDevice->getQueueFamilyIndexes();
	if (deviceQueueFamilyIndexes.graphical != deviceQueueFamilyIndexes.present)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = (uint32_t*)&deviceQueueFamilyIndexes;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	createInfo.preTransform = swapchainConfigurations.surfaceCapabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = swapchainConfigurations.presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = nullptr;

	VkSwapchainKHR swapchainHandle;
	CALL_VK(vkCreateSwapchainKHR(pDevice->getHandle(), &createInfo, nullptr, &swapchainHandle))

	return swapchainHandle;
}
