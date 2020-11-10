#ifndef PIPELINELAYOUT_H
#define PIPELINELAYOUT_H

#include "device/device.h"
#include "descriptor_set_layout.h"


class PipelineLayout
{
public:
	PipelineLayout(const Device& device, const DescriptorSetLayout& descriptorSetLayout);
	~PipelineLayout();

	void destroy(const Device& device);

public:
	VkPipelineLayout handle_;
};

#endif // PIPELINELAYOUT_H
