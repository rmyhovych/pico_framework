#include "pipeline_layout.h"


PipelineLayout::PipelineLayout(const Device& device, const DescriptorSetLayout& descriptorSetLayout) :
	handle_(VK_NULL_HANDLE)
{
	VkPipelineLayoutCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	createInfo.setLayoutCount = 1;
	createInfo.pSetLayouts = &descriptorSetLayout.handle_;
	createInfo.pushConstantRangeCount = 0;
	createInfo.pPushConstantRanges = nullptr;

	CALL_VK(vkCreatePipelineLayout(device.handle_, &createInfo, nullptr, &handle_))
}

PipelineLayout::~PipelineLayout()
{
	CHECK_NULL_HANDLE(handle_)
}

void PipelineLayout::destroy(const Device &device)
{
	vkDestroyPipelineLayout(device.handle_, handle_, nullptr);
	handle_ = VK_NULL_HANDLE;
}
