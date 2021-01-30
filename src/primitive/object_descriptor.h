//
// Created by ross on 2020-11-06.
//

#ifndef PICOFRAMEWORK_OBJECT_DESCRIPTOR_H
#define PICOFRAMEWORK_OBJECT_DESCRIPTOR_H

#include <vector>
#include "resources/resource_factory.h"
#include "pfvk.h"

class ObjectDescriptor
{
public:
	template<typename T>
	ObjectDescriptor(ResourceFactory& resourceFactory, const std::vector<T>& vertices, const std::vector<uint16_t>& indexes);

	ObjectDescriptor(ObjectDescriptor&& other) noexcept;

	void recordCommands(VkCommandBuffer commandBuffer) const;

private:
	VkBuffer vertexBuffer_;
	VkBuffer indexBuffer_;

	uint32_t nIndexes_;
};

template<typename T>
ObjectDescriptor::ObjectDescriptor(ResourceFactory& resourceFactory, const std::vector<T>& vertices, const std::vector<uint16_t>& indexes):
		vertexBuffer_{},
		indexBuffer_{},

		nIndexes_(indexes.size())
{
	vertexBuffer_ = resourceFactory.createDeviceBuffer(vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
	indexBuffer_ = resourceFactory.createDeviceBuffer(indexes, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
}


#endif //PICOFRAMEWORK_OBJECT_DESCRIPTOR_H
