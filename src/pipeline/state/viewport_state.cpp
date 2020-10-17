#include "viewport_state.h"

ViewportState::ViewportState(VkExtent2D extent) :
    createInfo_({})
{
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) extent.width;
	viewport.height = (float) extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = extent;

	createInfo_.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	createInfo_.viewportCount = 1;
	createInfo_.pViewports = &viewport;
	createInfo_.scissorCount = 1;
	createInfo_.pScissors = &scissor;
}


const VkPipelineViewportStateCreateInfo *ViewportState::getCreateInfoPtr() const
{
	return &createInfo_;
}
