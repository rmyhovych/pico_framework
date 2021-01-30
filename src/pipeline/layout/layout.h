#ifndef DESCRIPTORSETLAYOUT_H
#define DESCRIPTORSETLAYOUT_H

#include <vector>

#include "pfvk.h"
#include "binding/binding_layout.h"

class Layout
{
public:
	class Builder
	{
	public:
		explicit Builder(VkDevice hDevice);

		template<typename BindingType>
		Builder& addBinding(const BindingLayout<BindingType>& bindingLayout);

		Layout build() const;

	private:
		VkDevice hDevice_;

		std::vector<VkDescriptorSetLayoutBinding> bindings_;
	};

public:
	explicit Layout(VkDevice hDevice, VkDescriptorSetLayout handle);

	~Layout();

	VkPipelineLayout getPipelineLayout() const;

private:
	VkDevice hDevice_;

	VkDescriptorSetLayout descriptorSetLayout_;
	VkPipelineLayout pipelineLayout_;
};

template<typename BindingType>
Layout::Builder& Layout::Builder::addBinding(const BindingLayout<BindingType>& bindingLayout)
{
	bindings_.push_back(VkDescriptorSetLayoutBinding{});

	VkDescriptorSetLayoutBinding& binding = bindings_.back();
	binding.binding = bindingLayout.getBindingIndex();
	binding.descriptorType = bindingLayout.getDescriptorType();
	binding.descriptorCount = 1;
	binding.stageFlags = bindingLayout.getStageFlags();
	binding.pImmutableSamplers = nullptr;

	return *this;
}

#endif // DESCRIPTORSETLAYOUT_H
