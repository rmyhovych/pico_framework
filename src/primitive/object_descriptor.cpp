//
// Created by ross on 2020-11-06.
//

#include "object_descriptor.h"

ObjectDescriptor::~ObjectDescriptor()
{
	CHECK_NULL_HANDLE(vertexBuffer_.handle)
	CHECK_NULL_HANDLE(indexBuffer_.handle)
}

void ObjectDescriptor::destroy(const ResourceFactory &resourceFactory)
{
	resourceFactory.destroyBuffer(vertexBuffer_);
	vertexBuffer_ = BufferAllocation{};

	resourceFactory.destroyBuffer(indexBuffer_);
	indexBuffer_ = BufferAllocation{};
}

ObjectDescriptor::ObjectDescriptor(ObjectDescriptor &&other) noexcept:
		vertexBuffer_(other.vertexBuffer_),
		indexBuffer_(other.indexBuffer_),
		nIndexes_(other.nIndexes_)
{
	other.vertexBuffer_ = BufferAllocation{};
	other.indexBuffer_ = BufferAllocation{};
}

void ObjectDescriptor::recordCommands(VkCommandBuffer commandBuffer) const
{
	VkDeviceSize zeroOffset{0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer_.handle, &zeroOffset);
	vkCmdBindIndexBuffer(commandBuffer, indexBuffer_.handle, zeroOffset, VK_INDEX_TYPE_UINT16);

	vkCmdDrawIndexed(commandBuffer, nIndexes_, 1, 0, 0, 0);
}
