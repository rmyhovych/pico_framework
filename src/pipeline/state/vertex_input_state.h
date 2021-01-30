#ifndef VERTEXDESCRIPTION_H
#define VERTEXDESCRIPTION_H

#include "pipeline_state.h"
#include <vector>

class VertexInputState : public PipelineState<VkPipelineVertexInputStateCreateInfo>
{
public:
	VertexInputState(uint32_t stride, VkVertexInputRate inputRate);

	VertexInputState& pushAttribute(VkFormat format, uint32_t offset);

private:
	VkVertexInputBindingDescription bindingDescription_;
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions_;
};

#endif // VERTEXDESCRIPTION_H
