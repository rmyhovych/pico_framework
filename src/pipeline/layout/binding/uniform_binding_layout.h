//
// Created by ross on 2021-01-29.
//

#ifndef PICOFRAMEWORK_UNIFORM_BINDING_LAYOUT_H
#define PICOFRAMEWORK_UNIFORM_BINDING_LAYOUT_H

#include "binding_layout.h"
#include "uniform_binding.h"

template<typename T>
class UniformBindingLayout : public BindingLayout<UniformBinding<T>>
{
public:
	UniformBindingLayout(uint32_t bindingIndex, VkShaderStageFlags shaderStageFlags) :
			BindingLayout<UniformBinding<T>>(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, bindingIndex, shaderStageFlags)
	{
	}

	UniformBinding<T> createBinding() const override
	{
		return UniformBinding<T>(this->getBindingIndex());
	}
};


#endif //PICOFRAMEWORK_UNIFORM_BINDING_LAYOUT_H
