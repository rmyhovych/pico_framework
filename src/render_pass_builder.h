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

	RenderPassBuilder& pushBackColor(VkFormat format);

	RenderPassBuilder& pushBackDepth(VkFormat format);

	VkRenderPass build();

private:
	VkDevice deviceHandle_;

	std::vector<VkAttachmentDescription> attachmentDescriptions_;

	std::vector<VkAttachmentReference> colorAttachments_;
	VkAttachmentReference depthAttachment_;

	VkSubpassDescription subpass_;
};


#endif //PICOFRAMEWORK_RENDER_PASS_BUILDER_H
