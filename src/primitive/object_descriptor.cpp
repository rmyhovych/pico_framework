//
// Created by ross on 2020-11-06.
//

#include "object_descriptor.h"

ObjectDescriptor::ObjectDescriptor(ObjectDescriptor &&other) noexcept:
		vertexBuffer_(other.vertexBuffer_),
		indexBuffer_(other.indexBuffer_),
		nIndexes_(other.nIndexes_)
{
}

void ObjectDescriptor::recordCommands(VkCommandBuffer commandBuffer) const
{
	VkDeviceSize zeroOffset{0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer_, &zeroOffset);
	vkCmdBindIndexBuffer(commandBuffer, indexBuffer_, zeroOffset, VK_INDEX_TYPE_UINT16);

	vkCmdDrawIndexed(commandBuffer, nIndexes_, 1, 0, 0, 0);
}
