//
// Created by rmyho on 5/24/2020.
//

#ifndef PICOFRAMEWORK_SWAPCHAIN_H
#define PICOFRAMEWORK_SWAPCHAIN_H


#include "surface.h"
#include "device.h"
#include "pipeline.h"

class Swapchain
{
public:
	struct Properties
	{
		bool isDepth;
	};

	Swapchain(const Swapchain::Properties &properties, VkSurfaceKHR surfaceHandle, Device* pDevice);
	~Swapchain();

	void init(const SwapchainConfigurations& swapchainConfigurations);

	void reset(const SwapchainConfigurations& swapchainConfigurations);

private:
	void destroy();

	static VkSwapchainKHR createHandle(const SwapchainConfigurations& swapchainConfigurations, const Device* pDevice, VkSurfaceKHR surfaceHandle);

	static void createFramebuffers(std::vector<VkFramebuffer> &framebuffers, VkRenderPass renderPass, const std::vector<VkImageView> &imageViews,
	                               VkImageView depthView = VK_NULL_HANDLE);

	void getImages(std::vector<VkImage> &destination, VkSwapchainKHR swapchainHandle);

private:
	Swapchain::Properties properties_;
	VkSurfaceKHR surfaceHandle_;
	Device* pDevice_;

	VkSwapchainKHR handle_;

	std::vector<VkImage> images_;
	std::vector<VkImageView> imageViews_;

	VkImage depthImage_;
	VkImageView depthImageView_;

	VkRenderPass renderPass_;
	std::vector<VkFramebuffer> framebuffers_;

	std::vector<Pipeline*> pipelines_;
};


#endif //PICOFRAMEWORK_SWAPCHAIN_H
