//
// Created by ross on 2020-09-27.
//

#ifndef PICOFRAMEWORK_RENDER_PASS_H
#define PICOFRAMEWORK_RENDER_PASS_H

#include <vector>
#include "pfvk.h"

class RenderPass
{
public:
	class Builder
	{
	public:
		explicit Builder(VkDevice hDevice);

		Builder& pushBackColor(VkFormat format);

		Builder& pushBackDepth(VkFormat format);

		Builder& setPipelineBindPoint(VkPipelineBindPoint pipelineBindPoint);

		RenderPass build();

	private:
		VkDevice hDevice_;

		std::vector<VkAttachmentDescription> attachmentDescriptions_;
		std::vector<VkAttachmentReference> colorAttachments_;
		VkAttachmentReference depthAttachment_;

		VkSubpassDescription subpass_;
	};

	explicit RenderPass(VkDevice hDevice, VkRenderPass handle);

	~RenderPass();

	std::vector<VkFramebuffer> createFramebuffers(std::vector<std::vector<VkImageView>>& attachmentsList, VkExtent2D extent);

public:
	VkRenderPass handle_;

private:
	VkDevice hDevice_;
};


#endif //PICOFRAMEWORK_RENDER_PASS_H
