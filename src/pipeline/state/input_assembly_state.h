#ifndef INPUTASSEMBLYSTATE_H
#define INPUTASSEMBLYSTATE_H

#include "pfvk.h"

class InputAssemblyState
{
public:
	explicit InputAssemblyState(VkPrimitiveTopology topology);

	const VkPipelineInputAssemblyStateCreateInfo* getCreateInfoPtr() const;

private:
	VkPipelineInputAssemblyStateCreateInfo createInfo_;
};

#endif // INPUTASSEMBLYSTATE_H
