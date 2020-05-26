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

	Swapchain(const Properties &properties, VkSurfaceKHR surfaceHandle, Device* pDevice);
	~Swapchain();

	void init(const SwapchainConfigurations& swapchainConfigurations);

	void reset(const SwapchainConfigurations& swapchainConfigurations);

private:
	void destroy();

	static VkSwapchainKHR createHandle(const SwapchainConfigurations& swapchainConfigurations, const Device* pDevice, VkSurfaceKHR surfaceHandle);

	static VkRenderPass createRenderPass(VkFormat format, bool )

private:
	VkSurfaceKHR surfaceHandle_;
	Device* pDevice_;

	VkSwapchainKHR handle_;

	std::vector<VkImage> images_;
	std::vector<VkImageView> imageViews_;

	VkRenderPass renderPass_;
	std::vector<VkFramebuffer> framebuffers_;

	bool isDepth_;

	std::vector<Pipeline*> pipelines_;
};


#endif //PICOFRAMEWORK_SWAPCHAIN_H
