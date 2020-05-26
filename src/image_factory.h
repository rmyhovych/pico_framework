//
// Created by rmyho on 5/24/2020.
//

#ifndef PICOFRAMEWORK_IMAGE_FACTORY_H
#define PICOFRAMEWORK_IMAGE_FACTORY_H

#include <vector>
#include <pfvk.h>
#include "device.h"

class ImageFactory
{
public:
	explicit ImageFactory(const Device* pDevice);

	void createImages(std::vector<VkImage>& destination, VkSwapchainKHR swapchainHandle);

	void createImageViews(std::vector<VkImageView>& destination, const std::vector<VkImage>& images, VkFormat format, VkImageAspectFlags aspectFlags);

	VkFormat getSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags featureFlags);

	VkFormat getDepthFormat();

private:
	const Device* pDevice_;
};


#endif //PICOFRAMEWORK_IMAGE_FACTORY_H
