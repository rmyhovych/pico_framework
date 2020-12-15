#include "layout.h"


Layout::Builder::Builder(VkDevice hDevice) :
		hDevice_(hDevice),
		bindings_(0)
{
}

Layout::Builder &Layout::Builder::pushBinding(VkDescriptorType type, VkShaderStageFlags stage)
{
	bindings_.push_back(VkDescriptorSetLayoutBinding{});

	VkDescriptorSetLayoutBinding &binding = bindings_.back();
	binding.binding = static_cast<uint32_t>(bindings_.size() - 1);
	binding.descriptorType = type;
	binding.descriptorCount = 1;
	binding.stageFlags = stage;
	binding.pImmutableSamplers = nullptr;

	return *this;
}

Layout Layout::Builder::build() const
{
	VkDescriptorSetLayoutCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	createInfo.bindingCount = static_cast<uint32_t>(bindings_.size());
	createInfo.pBindings = bindings_.data();

	VkDescriptorSetLayout handle;
	CALL_VK(vkCreateDescriptorSetLayout(hDevice_, &createInfo, nullptr, &handle))

	return Layout(hDevice_, handle);
}

/*---------------------------------------------------------------------------------------------*/

Layout::~Layout()
{
	vkDestroyPipelineLayout(hDevice_, pipelineLayout_, nullptr);
	pipelineLayout_ = VK_NULL_HANDLE;

	vkDestroyDescriptorSetLayout(hDevice_, descriptorSetLayout_, nullptr);
	descriptorSetLayout_ = VK_NULL_HANDLE;
}

Layout::Layout(VkDevice hDevice, VkDescriptorSetLayout handle) :
		hDevice_(hDevice),
		descriptorSetLayout_(handle),
		pipelineLayout_(VK_NULL_HANDLE)
{
	VkPipelineLayoutCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	createInfo.setLayoutCount = 1;
	createInfo.pSetLayouts = &descriptorSetLayout_;
	createInfo.pushConstantRangeCount = 0;
	createInfo.pPushConstantRanges = nullptr;

	CALL_VK(vkCreatePipelineLayout(hDevice_, &createInfo, nullptr, &pipelineLayout_))
}

VkPipelineLayout Layout::getPipelineLayout() const
{
	return pipelineLayout_;
}
