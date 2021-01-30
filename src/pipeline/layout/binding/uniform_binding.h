//
// Created by ross on 2021-01-27.
//

#ifndef PICOFRAMEWORK_UNIFORM_BINDING_H
#define PICOFRAMEWORK_UNIFORM_BINDING_H

#include "binding.h"
#include "buffer_binding_resource.h"

template<typename T>
class UniformBinding : public Binding<BufferBindingResource<T>>
{
public:
	explicit UniformBinding(uint32_t bindingIndex) :
			Binding<BufferBindingResource<T>>(bindingIndex)
	{
	}

	UniqueFunction<void(VkWriteDescriptorSet&)> getDescriptorWriteApplicator(uint32_t frameIndex) const override
	{
		VkDescriptorBufferInfo descriptorBufferInfo{};
		descriptorBufferInfo.buffer = this->resource_.getBuffer();
		descriptorBufferInfo.offset = frameIndex * sizeof(T);
		descriptorBufferInfo.range = sizeof(T);

		VkWriteDescriptorSet writeOnto{};
		writeOnto.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeOnto.dstBinding = this->bindingIndex_;
		writeOnto.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writeOnto.descriptorCount = 1;
		writeOnto.pBufferInfo = nullptr;

		return UniqueFunction<void(VkWriteDescriptorSet&)>([descriptorBufferInfo, bindingIndex = this->bindingIndex_](VkWriteDescriptorSet& writeOnto)
		                                                   {
			                                                   writeOnto.dstBinding = bindingIndex;
			                                                   writeOnto.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			                                                   writeOnto.descriptorCount = 1;
			                                                   writeOnto.pBufferInfo = &descriptorBufferInfo;
		                                                   });
	}

	void allocateResource(ResourceFactory& resourceFactory, uint32_t nFrames) override
	{
		this->resource_.allocate(resourceFactory, nFrames, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
	}

	void updateResource(const ResourceFactory& resourceFactory, uint32_t frameIndex) override
	{
		this->resource_.update(resourceFactory, frameIndex);
	}
};

#endif //PICOFRAMEWORK_UNIFORM_BINDING_H
