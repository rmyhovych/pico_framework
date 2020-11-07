#ifndef VIEWPORTSTATE_H
#define VIEWPORTSTATE_H

#include "pipeline_state.h"

class ViewportState : public PipelineState<VkPipelineViewportStateCreateInfo>
{
public:
	explicit ViewportState(VkExtent2D extent);

private:
	VkViewport viewport_;
	VkRect2D scissor_;
};

#endif // VIEWPORTSTATE_H
