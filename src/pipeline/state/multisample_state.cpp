#include "multisample_state.h"

MultisampleState::MultisampleState()
{
	createInfo_.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	createInfo_.sampleShadingEnable = VK_FALSE;
	createInfo_.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	createInfo_.minSampleShading = 1.0f;
	createInfo_.pSampleMask = nullptr;
	createInfo_.alphaToCoverageEnable = VK_FALSE;
	createInfo_.alphaToOneEnable = VK_FALSE;
}
