#ifndef VERTEXDESCRIPTION_H
#define VERTEXDESCRIPTION_H

#include "pfvk.h"
#include <vector>

class VertexInputState
{
public:
	VertexInputState(uint32_t stride, VkVertexInputRate inputRate=VK_VERTEX_INPUT_RATE_VERTEX);
	VertexInputState& pushAttribute(VkFormat format, uint32_t offset);

	const VkPipelineVertexInputStateCreateInfo* getCreateInfoPtr();

private:
	VkVertexInputBindingDescription bindingDescription_;
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions_;

	VkPipelineVertexInputStateCreateInfo createInfo_;
};

#endif // VERTEXDESCRIPTION_H
