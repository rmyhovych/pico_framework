//
// Created by ross on 2020-11-06.
//

#include "renderer.h"

Renderer::Renderer(const Device* pDevice, const Swapchain* pSwapchain, const RenderPass* pRenderPass, const Pipeline* pPipeline, ResourceFactory* pResourceFactory, const SwapchainConfigurations& swapchainConfigurations) :
		pDevice_(pDevice),
		pSwapchain_(pSwapchain),
		pRenderPass_(pRenderPass),
		pPipeline_(pPipeline),
		pResourceFactory_(pResourceFactory),
		swapchainConfigurations_(swapchainConfigurations),

		frameManager_(*pDevice, pSwapchain->getAttachments().size()),

		graphicsQueue_(pDevice->getQueue(VK_QUEUE_GRAPHICS_BIT)->queue),

		objectDescriptors_(),

		framebuffers_(),
		commandPool_(VK_NULL_HANDLE),
		commandBuffers_()
{
	const std::vector<std::vector<VkImageView>> attachmentList = pSwapchain->getAttachments();
	uint32_t nImages = attachmentList.size();

	framebuffers_.resize(nImages);

	VkFramebufferCreateInfo framebufferCreateInfo{};
	framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferCreateInfo.renderPass = pRenderPass->handle_;
	framebufferCreateInfo.width = swapchainConfigurations.extent.width;
	framebufferCreateInfo.height = swapchainConfigurations.extent.height;
	framebufferCreateInfo.layers = 1;

	for (uint32_t i = 0; i < nImages; ++i)
	{
		const std::vector<VkImageView>& attachments = attachmentList[i];
		framebufferCreateInfo.attachmentCount = attachments.size();
		framebufferCreateInfo.pAttachments = attachments.data();
		CALL_VK(vkCreateFramebuffer(pDevice_->handle_, &framebufferCreateInfo, nullptr, &framebuffers_[i]))
	}


	commandPool_ = createCommandPool(pDevice_);

	commandBuffers_.resize(nImages);
	VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = commandPool_;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = nImages;
	CALL_VK(vkAllocateCommandBuffers(pDevice_->handle_, &commandBufferAllocateInfo, commandBuffers_.data()))
}


Renderer::~Renderer()
{
	CHECK_NULL_HANDLE(commandPool_)
}

void Renderer::destroy()
{
	vkDeviceWaitIdle(pDevice_->handle_);

	for (VkFramebuffer framebuffer : framebuffers_)
		vkDestroyFramebuffer(pDevice_->handle_, framebuffer, nullptr);
	framebuffers_.clear();

	objectDescriptors_.clear();

	vkDestroyCommandPool(pDevice_->handle_, commandPool_, nullptr);
	commandPool_ = VK_NULL_HANDLE;

	frameManager_.destroy();
}


void Renderer::recordCommands()
{
	VkCommandBufferBeginInfo commandBufferBeginInfo{};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.flags = 0;
	commandBufferBeginInfo.pInheritanceInfo = nullptr;

	VkRenderPassBeginInfo renderPassBeginInfo{};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = pRenderPass_->handle_;
	renderPassBeginInfo.renderArea.offset = {0, 0};
	renderPassBeginInfo.renderArea.extent = swapchainConfigurations_.extent;

	// ATTACHMENT ORDER
	std::vector<VkClearValue> clearValues({{0.0f, 0.6f, 0.1f, 1.0f}});
	renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassBeginInfo.pClearValues = clearValues.data();

	for (int32_t i = commandBuffers_.size() - 1; i >= 0; --i)
	{
		const VkCommandBuffer& commandBuffer = commandBuffers_[i];
		renderPassBeginInfo.framebuffer = framebuffers_[i];

		CALL_VK(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo))
		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pPipeline_->handle_);

		for (auto& objectDescriptor : objectDescriptors_)
			objectDescriptor.recordCommands(commandBuffer);

		vkCmdEndRenderPass(commandBuffer);
		CALL_VK(vkEndCommandBuffer(commandBuffer))
	}
}

void Renderer::render()
{
	uint32_t imageIndex = frameManager_.waitForImage(*pSwapchain_);
	for (BindingResource* resourcePtr : bindingResources_)
	{
		resourcePtr->update(*pResourceFactory_, imageIndex);
	}

	frameManager_.submit(graphicsQueue_, commandBuffers_);
	frameManager_.present(graphicsQueue_, *pSwapchain_);
}


VkCommandPool Renderer::createCommandPool(const Device* pDevice)
{
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.queueFamilyIndex = pDevice->getQueue(VK_QUEUE_GRAPHICS_BIT)->family;
	commandPoolCreateInfo.flags = 0;

	VkCommandPool commandPool;
	CALL_VK(vkCreateCommandPool(pDevice->handle_, &commandPoolCreateInfo, nullptr, &commandPool))
	return commandPool;
}
