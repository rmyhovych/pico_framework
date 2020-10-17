#include "descriptor_set_layout.h"


DescriptorSetLayout::Builder::Builder() :
    bindings_(0)
{
}

DescriptorSetLayout::Builder& DescriptorSetLayout::Builder::pushBinding(VkDescriptorType type, VkShaderStageFlags stage)
{
	bindings_.push_back(VkDescriptorSetLayoutBinding{});

	VkDescriptorSetLayoutBinding& binding = bindings_.back();
	binding.binding = static_cast<uint32_t>(bindings_.size() - 1);
	binding.descriptorType = type;
	binding.descriptorCount = 1;
	binding.stageFlags = stage;
	binding.pImmutableSamplers = nullptr;

	return *this;
}

DescriptorSetLayout DescriptorSetLayout::Builder::build(const Device &device)
{
	VkDescriptorSetLayoutCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	createInfo.bindingCount = static_cast<uint32_t>(bindings_.size());
	createInfo.pBindings = bindings_.data();

	VkDescriptorSetLayout handle;
	CALL_VK(vkCreateDescriptorSetLayout(device.handle_, &createInfo, nullptr, &handle))

	return DescriptorSetLayout(handle);
}

/*---------------------------------------------------------------------------------------------*/

DescriptorSetLayout::~DescriptorSetLayout()
{
	if (handle_ != VK_NULL_HANDLE)
		printf("WARNING : VkDescriptorSetLayout handle was not explicitly destroyed.\n");
}

void DescriptorSetLayout::destroy(const Device &device)
{
	vkDestroyDescriptorSetLayout(device.handle_, handle_, nullptr);
	handle_ = VK_NULL_HANDLE;
}

DescriptorSetLayout::DescriptorSetLayout(VkDescriptorSetLayout handle) :
    handle_(handle)
{
}
