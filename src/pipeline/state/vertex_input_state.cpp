#include "vertex_input_state.h"

VertexInputState::VertexInputState(uint32_t stride, VkVertexInputRate inputRate) :
	bindingDescription_({}),
	attributeDescriptions_(0),

	createInfo_({})
{
	bindingDescription_.binding = 0;
	bindingDescription_.stride = stride;
	bindingDescription_.inputRate = inputRate;

	createInfo_.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	createInfo_.vertexBindingDescriptionCount = 1;
}

VertexInputState &VertexInputState::pushAttribute(VkFormat format, uint32_t offset)
{
	attributeDescriptions_.push_back(VkVertexInputAttributeDescription{});

	VkVertexInputAttributeDescription& attribute = attributeDescriptions_.back();
	attribute.binding = bindingDescription_.binding;
	attribute.location = attributeDescriptions_.size() - 1;
	attribute.format = format;
	attribute.offset = offset;

	return *this;
}


const VkPipelineVertexInputStateCreateInfo* VertexInputState::getCreateInfoPtr()
{
	createInfo_.pVertexBindingDescriptions = &bindingDescription_;
	createInfo_.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions_.size());
	createInfo_.pVertexAttributeDescriptions = attributeDescriptions_.data();
	return &createInfo_;
}
