#include "depth_stencil_state.h"

DepthStencilState::DepthStencilState(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp)
{
	createInfo_.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	createInfo_.depthTestEnable = depthTestEnable;
	createInfo_.depthWriteEnable = depthWriteEnable;

	createInfo_.depthCompareOp = depthCompareOp;

	// Unused
	createInfo_.depthBoundsTestEnable = VK_FALSE;
	createInfo_.minDepthBounds = 0.0f;
	createInfo_.maxDepthBounds = 1.0f;

	// Unused
	createInfo_.stencilTestEnable = VK_FALSE;
	createInfo_.front = {};
	createInfo_.back = {};
}
