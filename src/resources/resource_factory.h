//
// Created by rmyho on 6/7/2020.
//

#ifndef PICOFRAMEWORK_RESOURCE_FACTORY_H
#define PICOFRAMEWORK_RESOURCE_FACTORY_H

#include "pfvk.h"
#include "allocator.h"
#include "data_transfer_manager.h"

#include <cstring>
#include <vector>
#include <map>
#include <set>

class ResourceFactory
{
public:
	explicit ResourceFactory(VkInstance hInstance, VkDevice hDevice, VkPhysicalDevice hPhysicalDevice, const DeviceQueue* pTransferDeviceQueue);

	~ResourceFactory();


	VkBuffer createBuffer(VkDeviceSize size, VkBufferUsageFlags usageFlags, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags flags = 0);

	template<typename T>
	VkBuffer createDeviceBuffer(const std::vector<T> &data, VkBufferUsageFlags usageFlags);

	void destroyBuffer(VkBuffer buffer);


	VkImage createImage(VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlags, VmaMemoryUsage memoryUsage);

	void destroyImage(VkImage image);


	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	void destroyImageView(VkImageView imageView);


	void* map(VkBuffer buffer) const;

	void unmap(VkBuffer buffer) const;

private:
	VkDevice hDevice_;

	Allocator allocator_;
	DataTransferManager transferManager_;

	std::map<VkBuffer, VmaAllocation> createdBuffers_;
	std::map<VkImage, VmaAllocation> createdImages_;
	std::set<VkImageView> createdImageViews_;
};

template<typename T>
VkBuffer ResourceFactory::createDeviceBuffer(const std::vector<T> &data, VkBufferUsageFlags usageFlags)
{
	VkDeviceSize bufferSize = sizeof(T) * data.size();

	VkBuffer stagingBuffer = createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
	void* ptr = map(stagingBuffer);
	memcpy(ptr, data.data(), bufferSize);
	unmap(stagingBuffer);

	VkBuffer buffer = createBuffer(bufferSize, usageFlags | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY);

	transferManager_.begin().transfer(stagingBuffer, buffer, bufferSize).submit();

	destroyBuffer(stagingBuffer);
	return buffer;
}


#endif //PICOFRAMEWORK_RESOURCE_FACTORY_H
