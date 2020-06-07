//
// Created by rmyho on 5/25/2020.
//

#ifndef PICOFRAMEWORK_RENDER_PASS_BUILDER_H
#define PICOFRAMEWORK_RENDER_PASS_BUILDER_H

#include <vector>

#include "pfvk.h"

/**
 * Supports 1 subpass (for now?)
 */
class RenderPassBuilder
{
public:
	explicit RenderPassBuilder(VkDevice deviceHandle);

	RenderPassBuilder &pushBackColor(VkFormat format);

	RenderPassBuilder &pushBackDepth(VkFormat format);

	VkRenderPass getRenderPass();

	void createFramebuffers(std::vector<VkFramebuffer> &framebuffers,
	                        std::vector<std::vector<VkImageView>> &attachmentsList, VkExtent2D extent);

private:
	void updateRenderPass();

private:
	VkDevice deviceHandle_;

	std::vector<VkAttachmentDescription> attachmentDescriptions_;

	std::vector<VkAttachmentReference> colorAttachments_;
	VkAttachmentReference depthAttachment_;

	VkSubpassDescription subpass_;


	VkRenderPass renderPass_;
	bool wasModified_;
};


#endif //PICOFRAMEWORK_RENDER_PASS_BUILDER_H
