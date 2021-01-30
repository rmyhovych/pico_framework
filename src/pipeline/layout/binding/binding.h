//
// Created by ross on 2020-11-29.
//

#ifndef PICOFRAMEWORK_BINDING_H
#define PICOFRAMEWORK_BINDING_H

#include "resources/resource_factory.h"
#include "pfvk.h"

class DescriptorWriteProvider
{
public:
	const VkWriteDescriptorSet& getDescriptorWrite() const
	{
		return writeDescriptorSet;
	}

protected:
	VkWriteDescriptorSet writeDescriptorSet;
};

template<typename T>
class Binding
{
public:
	explicit Binding(uint32_t bindingIndex) :
			bindingIndex_(bindingIndex),
			resource_()
	{
	}

	virtual UniqueFunction<void(VkWriteDescriptorSet&)> getDescriptorWriteApplicator(uint32_t frameIndex) const = 0;

	virtual void allocateResource(ResourceFactory& resourceFactory, uint32_t nFrames) = 0;

	virtual void updateResource(const ResourceFactory& resourceFactory, uint32_t frameIndex) = 0;

	T* getResourcePtr()
	{
		return resource_;
	}

protected:
	uint32_t bindingIndex_;
	T resource_;
};

#endif //PICOFRAMEWORK_BINDING_H
