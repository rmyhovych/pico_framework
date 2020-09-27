//
// Created by ross on 2020-09-27.
//

#include "render_pass.h"

RenderPass::RenderPass(VkRenderPass handle) :
		handle_(handle)
{
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

		CALL_VK(vkCreateFramebuffer(deviceHandle, &createInfo, nullptr, &framebuffers[i++]))
	}

	return framebuffers;
}
