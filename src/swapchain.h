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

	Pipeline* createPipeline();

	void recordCommands() const;

	void init(const SwapchainConfigurations &swapchainConfigurations);

	void destroy();

	void reset(const SwapchainConfigurations &swapchainConfigurations);

private:
	static VkSwapchainKHR createHandle(const SwapchainConfigurations &swapchainConfigurations, const Device* pDevice, VkSurfaceKHR surfaceHandle);

	void getImages(std::vector<VkImage> &destination, VkSwapchainKHR swapchainHandle);

	void createDepthResources(VkExtent2D extent);

	void createFramebuffers(VkExtent2D extent);

	void createCommandBuffers();

private:
	Swapchain::Properties properties_;

	VkSurfaceKHR surfaceHandle_;
	Device* pDevice_;

	VkSwapchainKHR handle_;

	std::vector<VkImage> images_;
	std::vector<VkImageView> imageViews_;

	ImageAllocation depthImageAllocation_;
	VkImageView depthImageView_;

	VkRenderPass renderPass_;
	std::vector<VkFramebuffer> framebuffers_;

	VkCommandPool commandPool_;
	std::vector<VkCommandBuffer> commandBuffers_;

	std::vector<Pipeline*> pipelines_;
};


#endif //PICOFRAMEWORK_SWAPCHAIN_H
