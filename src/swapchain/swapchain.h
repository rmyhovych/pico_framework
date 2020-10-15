//
// Created by ross on 2020-10-05.
//

#ifndef PICOFRAMEWORK_SWAPCHAIN_H
#define PICOFRAMEWORK_SWAPCHAIN_H


#include "resources/resource_factory.h"
#include "device.h"
#include "surface.h"

class Swapchain
{
public:
	class Builder
	{
		Builder(const Surface &surface, const Device &device, const ResourceFactory &resourceFactory);

		Swapchain build(const SwapchainConfigurations &configurations) const;

	private:
		VkSwapchainKHR createHandle(const SwapchainConfigurations &configurations) const;

		std::vector<VkImage> getImages(VkSwapchainKHR handle) const;

	private:
		const Surface &surface_;
		const Device &device_;
		const ResourceFactory &resourceFactory_;
	};

private:
	Swapchain(VkSwapchainKHR handle, std::vector<VkImage>& images, std::vector<VkImageView> imageViews);

	~Swapchain();

	void destroy(const Device &device, const ResourceFactory &resourceFactory);

	std::vector<VkImageView> &getImageViews();

public:
	VkSwapchainKHR handle_;

private:
	std::vector<VkImage> images_;
	std::vector<VkImageView> imageViews_;
};


#endif //PICOFRAMEWORK_SWAPCHAIN_H
