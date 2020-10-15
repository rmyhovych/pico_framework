#include "vertex_binding.h"

VertexBinding::VertexBinding(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate) :
	bindingDescription_({}),
	attributeDescriptions_(0)
{
	bindingDescription_.binding = binding;
	bindingDescription_.stride = stride;
	bindingDescription_.inputRate = inputRate;
}

const VkVertexInputBindingDescription &VertexBinding::getBindingDescription() const
{
	return bindingDescription_;
}

const std::vector<VkVertexInputAttributeDescription> &VertexBinding::getAttributeDescriptions() const
{
	return attributeDescriptions_;
}

VertexBinding &VertexBinding::pushAttribute(VkFormat format, uint32_t offset)
{
	attributeDescriptions_.push_back(VkVertexInputAttributeDescription{});

	VkVertexInputAttributeDescription& attribute = attributeDescriptions_.back();
	attribute.binding = bindingDescription_.binding;
	attribute.location = attributeDescriptions_.size() - 1;
	attribute.format = format;
	attribute.offset = offset;

	return *this;
}
