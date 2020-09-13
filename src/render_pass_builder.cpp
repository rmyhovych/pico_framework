//
// Created by rmyho on 5/25/2020.
//

#include "render_pass_builder.h"

RenderPassBuilder::RenderPassBuilder(VkDevice deviceHandle) :
		deviceHandle_(deviceHandle),

		attachmentDescriptions_(0),
		colorAttachments_(0),
		depthAttachment_({}),
		subpass_({}),

		renderPass_(VK_NULL_HANDLE),
		wasModified_(true)
{
	subpass_.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

	subpass_.colorAttachmentCount = 0;
	subpass_.pColorAttachments = nullptr;
	subpass_.pDepthStencilAttachment = nullptr;
}

RenderPassBuilder &RenderPassBuilder::pushBackColor(VkFormat format)
{
	wasModified_ = true;

	attachmentDescriptions_.push_back({});
	VkAttachmentDescription &attachmentDescription = attachmentDescriptions_.back();

	attachmentDescription.format = format;
	attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	colorAttachments_.push_back({});
	VkAttachmentReference &attachmentReference = colorAttachments_.back();
	attachmentReference.attachment = attachmentDescriptions_.size() - 1;
	attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	return *this;
}

RenderPassBuilder &RenderPassBuilder::pushBackDepth(VkFormat format)
{
	if (subpass_.pDepthStencilAttachment != nullptr)
	{
		throw std::runtime_error("Depth already attached!");
	}

	wasModified_ = true;

	attachmentDescriptions_.push_back({});
	VkAttachmentDescription &attachmentDescription = attachmentDescriptions_.back();

	attachmentDescription.format = format;
	attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	depthAttachment_.attachment = attachmentDescriptions_.size() - 1;
	depthAttachment_.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	subpass_.pDepthStencilAttachment = &depthAttachment_;

	return *this;
}

VkRenderPass RenderPassBuilder::getRenderPass()
{
	updateRenderPass();
	return renderPass_;
}

void RenderPassBuilder::createFramebuffers(std::vector<VkFramebuffer> &framebuffers, std::vector<std::vector<VkImageView>> &attachmentsList, VkExtent2D extent)
{
	updateRenderPass();

	framebuffers.resize(attachmentsList.size());

	VkFramebufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	createInfo.renderPass = renderPass_;
	createInfo.width = extent.width;
	createInfo.height = extent.height;
	createInfo.layers = 1;

	int i = 0;
	for (std::vector<VkImageView> &attachments : attachmentsList)
	{
		if (attachments.size() != attachmentDescriptions_.size())
			throw std::runtime_error("Framebuffer attachments count incompatible with render pass attachments!");

		createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		createInfo.pAttachments = attachments.data();

		CALL_VK(vkCreateFramebuffer(deviceHandle_, &createInfo, nullptr, &framebuffers[i++]))
	}
}

void RenderPassBuilder::updateRenderPass()
{
	if (!wasModified_)
	{
		wasModified_ = false;
		return;
	}

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = static_cast<uint32_t>(colorAttachments_.size());
	subpass.pColorAttachments = colorAttachments_.data();
	// subpass.pDepthStencilAttachment = ALREADY SET;


	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions_.size());
	renderPassCreateInfo.pAttachments = attachmentDescriptions_.data();
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	renderPassCreateInfo.dependencyCount = 1;
	renderPassCreateInfo.pDependencies = &dependency;

	CALL_VK(vkCreateRenderPass(deviceHandle_, &renderPassCreateInfo, nullptr, &renderPass_))
}
