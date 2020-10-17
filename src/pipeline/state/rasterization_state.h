#ifndef RASTERIZATIONSTATE_H
#define RASTERIZATIONSTATE_H

#include "pipeline_state.h"

class RasterizationState : public PipelineState<VkPipelineRasterizationStateCreateInfo>
{
public:
	RasterizationState(VkFrontFace frontFace=VK_FRONT_FACE_COUNTER_CLOCKWISE, VkCullModeFlags cullMode=VK_CULL_MODE_BACK_BIT);
};

#endif // RASTERIZATIONSTATE_H
