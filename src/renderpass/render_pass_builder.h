//
// Created by ross on 2020-09-27.
//

#ifndef PICOFRAMEWORK_RENDER_PASS_BUILDER_H
#define PICOFRAMEWORK_RENDER_PASS_BUILDER_H

#include <vector>
#include "pfvk.h"
#include "render_pass.h"

class RenderPassBuilder
{
public:
	RenderPassBuilder();

	RenderPassBuilder &pushBackColor(VkFormat format);

	RenderPassBuilder &pushBackDepth(VkFormat format);

	RenderPassBuilder &setPipelineBindPoint(VkPipelineBindPoint pipelineBindPoint);

	RenderPass build(VkDevice device);

private:
	std::vector<VkAttachmentDescription> attachmentDescriptions_;
	std::vector<VkAttachmentReference> colorAttachments_;
	VkAttachmentReference depthAttachment_;

	VkSubpassDescription subpass_;
};


#endif //PICOFRAMEWORK_RENDER_PASS_BUILDER_H
