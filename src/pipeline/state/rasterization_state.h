#ifndef RASTERIZATIONSTATE_H
#define RASTERIZATIONSTATE_H

#include "pipeline_state.h"

class RasterizationState:
		public PipelineState<VkPipelineRasterizationStateCreateInfo>
{
public:
	RasterizationState();

};

#endif // RASTERIZATIONSTATE_H
