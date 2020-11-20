#ifndef PIPELINELAYOUT_H
#define PIPELINELAYOUT_H

#include "descriptor_set_layout.h"


class PipelineLayout
{
public:
	PipelineLayout(VkDevice hDevice, const DescriptorSetLayout &descriptorSetLayout);

	~PipelineLayout();

public:
	VkPipelineLayout handle_;

private:
	VkDevice hDevice_;
};

#endif // PIPELINELAYOUT_H
