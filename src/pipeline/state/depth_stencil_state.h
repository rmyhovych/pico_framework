#ifndef DEPTHSTENCILSTATE_H
#define DEPTHSTENCILSTATE_H

#include "pipeline_state.h"


class DepthStencilState : public PipelineState<VkPipelineDepthStencilStateCreateInfo>
{
public:
	DepthStencilState(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp=VK_COMPARE_OP_LESS);
};

#endif // DEPTHSTENCILSTATE_H
