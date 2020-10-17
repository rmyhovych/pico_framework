#include "color_blend_state.h"

ColorBlendState::ColorBlendState()
{
	createInfo_.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	createInfo_.logicOpEnable = VK_FALSE;
	createInfo_.logicOp = VK_LOGIC_OP_COPY;
	createInfo_.blendConstants[0] = 0.0f;
	createInfo_.blendConstants[1] = 0.0f;
	createInfo_.blendConstants[2] = 0.0f;
	createInfo_.blendConstants[3] = 0.0f;

	createInfo_.attachmentCount = 0;
	createInfo_.pAttachments = nullptr;
}

ColorBlendState &ColorBlendState::addAttachment(const VkPipelineColorBlendAttachmentState &attachment)
{
	attachments_.push_back(attachment);

	createInfo_.attachmentCount = static_cast<uint32_t>(attachments_.size());
	createInfo_.pAttachments = attachments_.data();

	return *this;
}
