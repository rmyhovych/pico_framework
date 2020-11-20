#include "pipeline_layout.h"


PipelineLayout::PipelineLayout(VkDevice hDevice, const DescriptorSetLayout &descriptorSetLayout) :
		hDevice_(hDevice),
		handle_(VK_NULL_HANDLE)
{
	VkPipelineLayoutCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	createInfo.setLayoutCount = 1;
	createInfo.pSetLayouts = &descriptorSetLayout.handle_;
	createInfo.pushConstantRangeCount = 0;
	createInfo.pPushConstantRanges = nullptr;

	CALL_VK(vkCreatePipelineLayout(hDevice_, &createInfo, nullptr, &handle_))
}

PipelineLayout::~PipelineLayout()
{
	vkDestroyPipelineLayout(hDevice_, handle_, nullptr);
}
