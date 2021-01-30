#include "dynamic_state.h"

DynamicState::DynamicState()
{
	createInfo_.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

	createInfo_.dynamicStateCount = 0;
	createInfo_.pDynamicStates = nullptr;
}

DynamicState& DynamicState::addState(VkDynamicState state)
{
	states_.push_back(state);

	createInfo_.dynamicStateCount = static_cast<uint32_t>(states_.size());
	createInfo_.pDynamicStates = states_.data();

	return *this;
}
