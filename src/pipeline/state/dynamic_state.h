#ifndef DYNAMICSTATE_H
#define DYNAMICSTATE_H

#include <vector>

#include "pipeline_state.h"


class DynamicState : public PipelineState<VkPipelineDynamicStateCreateInfo>
{
public:
	DynamicState();

	DynamicState& addState(VkDynamicState state);

private:
	std::vector<VkDynamicState> states_;
};

#endif // DYNAMICSTATE_H
