#include "viewport_state.h"

ViewportState::ViewportState(VkExtent2D extent) :
		viewport_({}),
		scissor_({})
{
	viewport_.x = 0.0f;
	viewport_.y = 0.0f;
	viewport_.width = (float) extent.width;
	viewport_.height = (float) extent.height;
	viewport_.minDepth = 0.0f;
	viewport_.maxDepth = 1.0f;

	scissor_.offset = {0, 0};
	scissor_.extent = extent;

	createInfo_.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	createInfo_.viewportCount = 1;
	createInfo_.pViewports = &viewport_;
	createInfo_.scissorCount = 1;
	createInfo_.pScissors = &scissor_;
}

