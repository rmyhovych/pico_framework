#ifndef VIEWPORTSTATE_H
#define VIEWPORTSTATE_H

#include "pfvk.h"

class ViewportState
{
public:
	explicit ViewportState(VkExtent2D extent);

	const VkPipelineViewportStateCreateInfo* getCreateInfoPtr() const;

private:
	VkPipelineViewportStateCreateInfo createInfo_;
};

#endif // VIEWPORTSTATE_H
