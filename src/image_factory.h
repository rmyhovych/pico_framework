//
// Created by rmyho on 5/24/2020.
//

#ifndef PICOFRAMEWORK_IMAGE_FACTORY_H
#define PICOFRAMEWORK_IMAGE_FACTORY_H

#include <vector>
#include <pfvk.h>

class ImageFactory
{
public:
	ImageFactory(VkDevice deviceHandle);

	void createImages(std::vector<VkImage>& destination, VkSwapchainKHR swapchainHandle);

	void createImageViews(std::vector<VkImageView>& destination, const std::vector<VkImage>& images, VkFormat format, VkImageAspectFlags aspectFlags);

private:
	VkDevice deviceHandle_;
};


#endif //PICOFRAMEWORK_IMAGE_FACTORY_H
