//
// Created by ross on 2020-10-05.
//

#ifndef PICOFRAMEWORK_SWAPCHAIN_H
#define PICOFRAMEWORK_SWAPCHAIN_H


#include "resources/resource_factory.h"
#include "device/device.h"
#include "surface.h"

class Swapchain
{
public:
	class Builder
	{
	public:
		Builder(const Surface &surface, const Device &device, ResourceFactory &resourceFactory);

		Swapchain build(const SwapchainConfigurations &configurations) const;

	private:
		VkSwapchainKHR createHandle(const SwapchainConfigurations &configurations) const;

		std::vector<VkImage> getImages(VkSwapchainKHR handle) const;

	private:
		const Surface &surface_;
		const Device &device_;
		ResourceFactory &resourceFactory_;
	};

	~Swapchain();

	void destroy(const Device &device, ResourceFactory &resourceFactory);

	std::vector<std::vector<VkImageView>> getAttachments() const;

private:
	Swapchain(VkSwapchainKHR handle, std::vector<VkImageView> imageViews);

public:
	VkSwapchainKHR handle_;

private:
	std::vector<VkImageView> imageViews_;

	VkImage depthImage_;
	VkImageView depthImageView_;
};


#endif //PICOFRAMEWORK_SWAPCHAIN_H
