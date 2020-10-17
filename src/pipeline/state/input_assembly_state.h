#ifndef INPUTASSEMBLYSTATE_H
#define INPUTASSEMBLYSTATE_H

#include "pipeline_state.h"

class InputAssemblyState : public PipelineState<VkPipelineInputAssemblyStateCreateInfo>
{
public:
	explicit InputAssemblyState(VkPrimitiveTopology topology);
};

#endif // INPUTASSEMBLYSTATE_H
