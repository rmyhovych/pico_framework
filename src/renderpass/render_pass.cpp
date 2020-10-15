//
// Created by ross on 2020-09-27.
//

#include "render_pass.h"

RenderPass::Builder::Builder(const Device &device) :
		deviceHandle_(device.handle_),

		attachmentDescriptions_(0),
		colorAttachments_(0),
		depthAttachment_({}),

		subpass_({})
{
	subpass_.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

	subpass_.colorAttachmentCount = 0;
	subpass_.pColorAttachments = nullptr;
	subpass_.pDepthStencilAttachment = nullptr;
}

RenderPass::Builder &RenderPass::Builder::pushBackColor(VkFormat format)
{
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

	subpass_.pColorAttachments = colorAttachments_.data();
	subpass_.colorAttachmentCount = static_cast<uint32_t>(colorAttachments_.size());

	return *this;
}

RenderPass::Builder &RenderPass::Builder::pushBackDepth(VkFormat format)
{
	if (subpass_.pDepthStencilAttachment != nullptr)
	{
		throw std::runtime_error("Depth already attached!");
	}

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

RenderPass::Builder &RenderPass::Builder::setPipelineBindPoint(VkPipelineBindPoint pipelineBindPoint)
{
	subpass_.pipelineBindPoint = pipelineBindPoint;
	return *this;
}


RenderPass RenderPass::Builder::build()
{
	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions_.size());
	renderPassCreateInfo.pAttachments = attachmentDescriptions_.data();
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass_;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	renderPassCreateInfo.dependencyCount = 1;
	renderPassCreateInfo.pDependencies = &dependency;

	VkRenderPass renderPassHandle;
	CALL_VK(vkCreateRenderPass(deviceHandle_, &renderPassCreateInfo, nullptr, &renderPassHandle))

	return RenderPass(renderPassHandle);
}


/*------------------------------------------------------------------------------------------------------------------------------*/


RenderPass::RenderPass(VkRenderPass handle) :
		handle_(handle)
{
}

RenderPass::~RenderPass()
{
	if (handle_ != VK_NULL_HANDLE)
		printf("WARNING : VkRenderPass handle was not explicitly destroyed.\n");
}


void RenderPass::destroy(VkDevice device)
{
	vkDestroyRenderPass(device, handle_, nullptr);
	handle_ = VK_NULL_HANDLE;
}


std::vector<VkFramebuffer> RenderPass::createFramebuffers(VkDevice deviceHandle, std::vector<std::vector<VkImageView>> &attachmentsList, VkExtent2D extent)
{
	std::vector<VkFramebuffer> framebuffers(attachmentsList.size());

	VkFramebufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	createInfo.renderPass = handle_;
	createInfo.width = extent.width;
	createInfo.height = extent.height;
	createInfo.layers = 1;

	int i = 0;
	for (std::vector<VkImageView> &attachments : attachmentsList)
	{
		createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		createInfo.pAttachments = attachments.data();

		CALL_VK(vkCreateFramebuffer(deviceHandle, &createInfo, nullptr, &framebuffers[i]))

		++i;
	}

	return framebuffers;
}

