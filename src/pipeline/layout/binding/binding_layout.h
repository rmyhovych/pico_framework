//
// Created by ross on 2020-11-29.
//

#ifndef PICOFRAMEWORK_BINDING_LAYOUT_H
#define PICOFRAMEWORK_BINDING_LAYOUT_H

#include "pfvk.h"

template<typename BindingType>
class BindingLayout
{
public:
	virtual BindingType createBinding() const = 0;

	VkDescriptorType getDescriptorType() const
	{
		return descriptorType_;
	}

	uint32_t getBindingIndex() const
	{
		return bindingIndex_;
	}

	VkShaderStageFlags getStageFlags() const
	{
		return shaderStageFlags_;
	}

protected:
	BindingLayout(VkDescriptorType descriptorType, uint32_t bindingIndex, VkShaderStageFlags shaderStageFlags) :
			descriptorType_(descriptorType), bindingIndex_(bindingIndex), shaderStageFlags_(shaderStageFlags)
	{
	}

private:
	const VkDescriptorType descriptorType_;
	const uint32_t bindingIndex_;
	const VkShaderStageFlags shaderStageFlags_;
};


#endif //PICOFRAMEWORK_BINDING_LAYOUT_H
