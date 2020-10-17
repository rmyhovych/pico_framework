#ifndef VIEWPORTSTATE_H
#define VIEWPORTSTATE_H

#include "pipeline_state.h"

class ViewportState : public PipelineState<VkPipelineViewportStateCreateInfo>
{
public:
	explicit ViewportState(VkExtent2D extent);
};

#endif // VIEWPORTSTATE_H
