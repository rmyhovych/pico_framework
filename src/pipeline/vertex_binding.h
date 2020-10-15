#ifndef VERTEXDESCRIPTION_H
#define VERTEXDESCRIPTION_H

#include "pfvk.h"
#include <vector>

class VertexBinding
{
public:
	VertexBinding(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate=VK_VERTEX_INPUT_RATE_VERTEX);

	const VkVertexInputBindingDescription& getBindingDescription() const;
	const std::vector<VkVertexInputAttributeDescription>& getAttributeDescriptions() const;

	VertexBinding& pushAttribute(VkFormat format, uint32_t offset);

private:
	VkVertexInputBindingDescription bindingDescription_;
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions_;
};

#endif // VERTEXDESCRIPTION_H
