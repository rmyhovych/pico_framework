#ifndef COLORBLENDSTATE_H
#define COLORBLENDSTATE_H

#include <vector>

#include "pipeline_state.h"


class ColorBlendState : public PipelineState<VkPipelineColorBlendStateCreateInfo>
{
public:
	ColorBlendState();

	ColorBlendState& addAttachment(const VkPipelineColorBlendAttachmentState& attachment);

private:
	std::vector<VkPipelineColorBlendAttachmentState> attachments_;
};

#endif // COLORBLENDSTATE_H
