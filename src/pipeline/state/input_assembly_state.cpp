#include "input_assembly_state.h"

InputAssemblyState::InputAssemblyState(VkPrimitiveTopology topology) :
    createInfo_({})
{
	createInfo_.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	createInfo_.flags = 0;
	createInfo_.topology = topology;
	createInfo_.primitiveRestartEnable = VK_FALSE;
}

const VkPipelineInputAssemblyStateCreateInfo *InputAssemblyState::getCreateInfoPtr() const
{
	return &createInfo_;
}
