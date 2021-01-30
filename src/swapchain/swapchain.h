//
// Created by ross on 2020-10-05.
//

#ifndef PICOFRAMEWORK_SWAPCHAIN_H
#define PICOFRAMEWORK_SWAPCHAIN_H


#include "resources/resource_factory.h"
#include "surface.h"

class Swapchain
{
public:
	class Builder
	{
	public:
		Builder(VkDevice hDevice, const DeviceQueue* pDeviceQueue, const Surface* pSurface, ResourceFactory* pResourceFactory);

		Swapchain build(const SwapchainConfigurations& configurations) const;

	private:
		VkSwapchainKHR createHandle(const SwapchainConfigurations& configurations) const;

		std::vector<VkImage> getImages(VkSwapchainKHR handle) const;

	private:
		VkDevice hDevice_;

		const Surface* pSurface_;
		ResourceFactory* pResourceFactory_;
	};

	~Swapchain();

	std::vector<std::vector<VkImageView>> getAttachments() const;


private:
	Swapchain(VkDevice hDevice, ResourceFactory* pResourceFactory, VkSwapchainKHR handle, std::vector<VkImageView> imageViews);

public:
	VkSwapchainKHR handle_;

private:
	VkDevice hDevice_;
	ResourceFactory* pResourceFactory_;

	std::vector<VkImageView> imageViews_;

	VkImage depthImage_;
	VkImageView depthImageView_;
};


#endif //PICOFRAMEWORK_SWAPCHAIN_H
