#ifndef DESCRIPTORSETLAYOUT_H
#define DESCRIPTORSETLAYOUT_H

#include <vector>

#include "pfvk.h"


class DescriptorSetLayout
{
public:
	class Builder
	{
	public:
		explicit Builder(VkDevice hDevice);

		Builder &pushBinding(VkDescriptorType type, VkShaderStageFlags stage);

		DescriptorSetLayout build() const;

	private:
		VkDevice hDevice_;

		std::vector<VkDescriptorSetLayoutBinding> bindings_;
	};


public:
	~DescriptorSetLayout();

private:
	explicit DescriptorSetLayout(VkDevice hDevice, VkDescriptorSetLayout handle);

public:
	VkDescriptorSetLayout handle_;

private:
	VkDevice hDevice_;
};

#endif // DESCRIPTORSETLAYOUT_H
