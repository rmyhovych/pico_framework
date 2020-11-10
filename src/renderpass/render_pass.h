//
// Created by ross on 2020-09-27.
//

#ifndef PICOFRAMEWORK_RENDER_PASS_H
#define PICOFRAMEWORK_RENDER_PASS_H

#include <vector>
#include <surface.h>
#include <device/device.h>
#include <resources/resource_factory.h>
#include "pfvk.h"

class RenderPass
{
public:
	class Builder
	{
	public:
		Builder(const Device &device);

		Builder &pushBackColor(VkFormat format);

		Builder &pushBackDepth(VkFormat format);

		Builder &setPipelineBindPoint(VkPipelineBindPoint pipelineBindPoint);

		RenderPass build();

	private:
		VkDevice deviceHandle_;

		std::vector<VkAttachmentDescription> attachmentDescriptions_;
		std::vector<VkAttachmentReference> colorAttachments_;
		VkAttachmentReference depthAttachment_;

		VkSubpassDescription subpass_;
	};

	explicit RenderPass(VkRenderPass handle);

	~RenderPass();

	void destroy(VkDevice device);

	std::vector<VkFramebuffer> createFramebuffers(VkDevice deviceHandle, std::vector<std::vector<VkImageView>> &attachmentsList, VkExtent2D extent);

public:
	VkRenderPass handle_;
};


#endif //PICOFRAMEWORK_RENDER_PASS_H
