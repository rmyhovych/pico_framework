//
// Created by rmyho on 5/24/2020.
//

#include "swapchain.h"
#include "resource_factory.h"
#include "render_pass_builder.h"

Swapchain::Swapchain(const Swapchain::Properties &properties, VkSurfaceKHR surfaceHandle, Device* pDevice) :
		properties_(properties),
		surfaceHandle_(surfaceHandle),
		pDevice_(pDevice),

		handle_(VK_NULL_HANDLE),

		images_(),
		imageViews_(),

		depthImageAllocation_({VK_NULL_HANDLE, VK_NULL_HANDLE}),
		depthImageView_(VK_NULL_HANDLE),

		renderPass_(VK_NULL_HANDLE),
		framebuffers_(),

		commandPool_(VK_NULL_HANDLE),
		commandBuffers_(),

		pipelines_()
{
	commandPool_ = pDevice_->createCommandPool(0, GRAPHICAL);
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
	getImages(images_, handle_);

	ResourceFactory resourceFactory(pDevice_);

	imageViews_.resize(images_.size());
	for (uint32_t i = 0; i < imageViews_.size(); ++i)
		imageViews_[i] = resourceFactory.createImageView(images_[i], swapchainConfigurations.surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);

	RenderPassBuilder renderPassBuilder(pDevice_->getHandle());
	renderPassBuilder.pushBackColor(swapchainConfigurations.surfaceFormat.format);

	const PhysicalDevice &physicalDevice = pDevice_->getPhysicalDevice();
	VkFormat depthFormat = physicalDevice.getSupportedFormat(
			{VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT},
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);

	renderPassBuilder.pushBackDepth(depthFormat);
	renderPass_ = renderPassBuilder.getRenderPass();

	if (properties_.isDepth)
		createDepthResources(swapchainConfigurations.extent);

	createFramebuffers(swapchainConfigurations.extent);

	createCommandBuffers();
}

void Swapchain::destroy()
{
	VkDevice deviceHandle = pDevice_->getHandle();

	for (VkImageView imageView : imageViews_)
	{
		vkDestroyImageView(deviceHandle, imageView, nullptr);
	}
	imageViews_.clear();

	vkDestroySwapchainKHR(deviceHandle, handle_, nullptr);
	handle_ = VK_NULL_HANDLE;
}

void Swapchain::reset(const SwapchainConfigurations &swapchainConfigurations)
{
	destroy();
	init(swapchainConfigurations);
}


VkSwapchainKHR Swapchain::createHandle(const SwapchainConfigurations &swapchainConfigurations, const Device* pDevice, VkSurfaceKHR surfaceHandle)
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

	const QueueFamilyIndexes &deviceQueueFamilyIndexes = pDevice->getPhysicalDevice().getQueueFamilyIndexes();
	if (deviceQueueFamilyIndexes.graphical != deviceQueueFamilyIndexes.present)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = (uint32_t*) &deviceQueueFamilyIndexes;
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

void Swapchain::getImages(std::vector<VkImage> &destination, VkSwapchainKHR swapchainHandle)
{
	VkDevice deviceHandle = pDevice_->getHandle();

	uint32_t nImages = 0;
	vkGetSwapchainImagesKHR(deviceHandle, swapchainHandle, &nImages, nullptr);
	destination.resize(nImages);
	vkGetSwapchainImagesKHR(deviceHandle, swapchainHandle, &nImages, destination.data());
}


void Swapchain::createDepthResources(VkExtent2D extent)
{
	Allocator* pAllocator = pDevice_->getAllocator();
	VkFormat depthFormat = pDevice_->getPhysicalDevice().getSupportedFormat(
			{VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT},
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);

	VkImageCreateInfo imageCreateInfo{};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;

	imageCreateInfo.extent.width = extent.width;
	imageCreateInfo.extent.height = extent.height;
	imageCreateInfo.extent.depth = 1;

	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.format = depthFormat;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCreateInfo.flags = 0;

	pAllocator->createImage(&depthImageAllocation_, imageCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY);

}

void Swapchain::createFramebuffers(VkExtent2D extent)
{
	framebuffers_.resize(imageViews_.size());
	std::vector<VkImageView> attachments;
	attachments.push_back(VK_NULL_HANDLE);

	if (properties_.isDepth)
		attachments.push_back(depthImageView_);

	VkFramebufferCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	createInfo.renderPass = renderPass_;
	createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	createInfo.pAttachments = attachments.data();
	createInfo.width = extent.width;
	createInfo.height = extent.height;
	createInfo.layers = 1;

	VkDevice deviceHandle = pDevice_->getHandle();

	for (int i = framebuffers_.size() - 1; i >= 0; --i)
	{
		attachments[0] = imageViews_[i];
		CALL_VK(vkCreateFramebuffer(deviceHandle, &createInfo, nullptr, &framebuffers_[i]))
	}
}

void Swapchain::createCommandBuffers()
{
	commandBuffers_.resize(framebuffers_.size());
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = commandPool_;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers_.size());

	CALL_VK(vkAllocateCommandBuffers(pDevice_->getHandle(), &commandBufferAllocateInfo, commandBuffers_.data()))
}
