//
// Created by rmyho on 6/7/2020.
//

#ifndef PICOFRAMEWORK_RESOURCE_FACTORY_H
#define PICOFRAMEWORK_RESOURCE_FACTORY_H

#include "pfvk.h"
#include "device.h"

#include "allocator.h"

class ResourceFactory
{
public:
	explicit ResourceFactory(const Device &device, const Allocator* pAllocator);

	BufferAllocation createBuffer(VkDeviceSize size, VkBufferUsageFlags usageFlags, VmaMemoryUsage memoryUsage) const;

	void destroyBuffer(BufferAllocation &buffer) const;

	ImageAllocation createImage(VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlags, VmaMemoryUsage memoryUsage) const;

	void destroyImage(ImageAllocation &image) const;

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) const;

	void destroyImageView(VkImageView imageView) const;

private:
	VkDevice deviceHandle_;
	const Allocator* pAllocator_;
};


#endif //PICOFRAMEWORK_RESOURCE_FACTORY_H
