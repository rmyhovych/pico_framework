//
// Created by rmyho on 6/7/2020.
//

#ifndef PICOFRAMEWORK_RESOURCE_FACTORY_H
#define PICOFRAMEWORK_RESOURCE_FACTORY_H

#include <cstring>

#include "pfvk.h"
#include "device.h"

#include "allocator.h"

class ResourceFactory
{
public:
	explicit ResourceFactory(const Device* pDevice, const Allocator* pAllocator);

	~ResourceFactory();

	void destroy();

	BufferAllocation createBuffer(VkDeviceSize size, VkBufferUsageFlags usageFlags, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags = 0) const;

	template<typename T>
	BufferAllocation createDeviceBuffer(const std::vector<T> &data, VkBufferUsageFlags usageFlags) const;

	void destroyBuffer(BufferAllocation &buffer) const;

	ImageAllocation createImage(VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlags, VmaMemoryUsage memoryUsage) const;

	void destroyImage(ImageAllocation &image) const;

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) const;

	void destroyImageView(VkImageView imageView) const;

private:
	VkCommandBuffer createTransferCommandBuffer() const;

	void submitTransferCommandBuffer(VkCommandBuffer transferCommandBuffer) const;

private:
	const Device* pDevice_;
	const Allocator* pAllocator_;
	const DeviceQueue* pTransferQueue_;

	VkCommandPool transferCommandPool_;
};

template<typename T>
BufferAllocation ResourceFactory::createDeviceBuffer(const std::vector<T> &data, VkBufferUsageFlags usageFlags) const
{
	VkDeviceSize bufferSize = sizeof(T) * data.size();

	BufferAllocation stagingBuffer = createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
	void* ptr = pAllocator_->map(stagingBuffer);
	memcpy(ptr, data.data(), bufferSize);
	pAllocator_->unmap(stagingBuffer);

	BufferAllocation buffer = createBuffer(bufferSize, usageFlags | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY);

	VkCommandBuffer transferCommandBuffer = createTransferCommandBuffer();
	VkBufferCopy bufferCopy{};
	bufferCopy.size = bufferSize;
	bufferCopy.srcOffset = 0;
	bufferCopy.dstOffset = 0;
	vkCmdCopyBuffer(transferCommandBuffer, stagingBuffer.handle, buffer.handle, 1, &bufferCopy);
	submitTransferCommandBuffer(transferCommandBuffer);

	destroyBuffer(stagingBuffer);
	return buffer;
}


#endif //PICOFRAMEWORK_RESOURCE_FACTORY_H
