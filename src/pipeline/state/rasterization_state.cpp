#include "rasterization_state.h"

RasterizationState::RasterizationState(VkFrontFace frontFace, VkCullModeFlags cullMode)
{
	createInfo_.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	createInfo_.depthClampEnable = VK_FALSE;
	createInfo_.rasterizerDiscardEnable = VK_FALSE;

	// POLYGON MODE
	createInfo_.polygonMode = VK_POLYGON_MODE_FILL;
	createInfo_.lineWidth = 1.0f;

	// CULLING
	createInfo_.cullMode = cullMode;
	createInfo_.frontFace = frontFace;

	// DEPTH MAPPING
	createInfo_.depthBiasEnable = VK_FALSE;
	createInfo_.depthBiasConstantFactor = 0.0f;
	createInfo_.depthBiasClamp = 0.0f;
	createInfo_.depthBiasSlopeFactor = 0.0f;
}
