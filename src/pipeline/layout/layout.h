#ifndef DESCRIPTORSETLAYOUT_H
#define DESCRIPTORSETLAYOUT_H

#include <vector>

#include "pfvk.h"


class Layout
{
public:
	class Builder
	{
	public:
		explicit Builder(VkDevice hDevice);

		Builder &pushBinding(VkDescriptorType type, VkShaderStageFlags stage);

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

#endif // DESCRIPTORSETLAYOUT_H
