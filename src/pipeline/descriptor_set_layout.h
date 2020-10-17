#ifndef DESCRIPTORSETLAYOUT_H
#define DESCRIPTORSETLAYOUT_H

#include <vector>

#include "device.h"


class DescriptorSetLayout
{
public:
	class Builder {
	public:
		Builder();

		Builder& pushBinding(VkDescriptorType type, VkShaderStageFlags stage);

		DescriptorSetLayout build(const Device& device);

	private:
		std::vector<VkDescriptorSetLayoutBinding> bindings_;
	};


public:
	~DescriptorSetLayout();

	void destroy(const Device& device);

private:
	DescriptorSetLayout(VkDescriptorSetLayout handle);

public:
	VkDescriptorSetLayout handle_;
};

#endif // DESCRIPTORSETLAYOUT_H
