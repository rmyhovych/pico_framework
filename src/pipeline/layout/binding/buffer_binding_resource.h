//
// Created by ross on 2021-01-27.
//

#ifndef PICOFRAMEWORK_BUFFER_BINDING_RESOURCE_H
#define PICOFRAMEWORK_BUFFER_BINDING_RESOURCE_H

#include "resources/resource_factory.h"
#include "binding_resource.h"

template<typename T>
class BufferBindingResource : public BindingResource
{
public:
	explicit BufferBindingResource() :
			buffer_(VK_NULL_HANDLE),
			data_()
	{
	}

	void allocate(ResourceFactory& resourceFactory, uint32_t nFrames, VkBufferUsageFlags bufferUsage)
	{
		buffer_ = resourceFactory.createBuffer(nFrames * sizeof(T), bufferUsage, VMA_MEMORY_USAGE_CPU_TO_GPU, 0);
	}

	void update(const ResourceFactory& resourceFactory, uint32_t frameIndex) override
	{
		T* bufferPtr_ = (T*) resourceFactory.map(buffer_);
		memcpy((void*) bufferPtr_[frameIndex], (void*) &data_, sizeof(T));
		resourceFactory.unmap(buffer_);
	}

	VkBuffer getBuffer() const
	{
		return buffer_;
	}

	T& getData()
	{
		return data_;
	}

private:
	VkBuffer buffer_;
	T data_;
};


#endif //PICOFRAMEWORK_BUFFER_BINDING_RESOURCE_H
