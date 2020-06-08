//
// Created by rmyho on 6/7/2020.
//

#ifndef PICOFRAMEWORK_RESOURCE_FACTORY_H
#define PICOFRAMEWORK_RESOURCE_FACTORY_H

#include "pfvk.h"
#include "device.h"

class ResourceFactory
{
public:
	explicit ResourceFactory(Device* pDevice);

	ImageAllocation createImage(VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlags, VmaMemoryUsage memoryUsage) const;

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) const;

private:
	VkDevice deviceHandle_;
	Allocator* pAllocator_;
};


#endif //PICOFRAMEWORK_RESOURCE_FACTORY_H
