//
// Created by rmyho on 5/19/2020.
//

#ifndef PICOFRAMEWORK_SURFACE_H
#define PICOFRAMEWORK_SURFACE_H

#include "pfvk.h"

#include <cstdint>
#include <vector>

struct SwapchainConfigurations
{
	VkPresentModeKHR presentMode;
	VkSurfaceFormatKHR surfaceFormat;
	VkExtent2D extent;
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
};

class Surface
{
public:
	explicit Surface(VkSurfaceKHR handle, VkInstance hInstance, VkFormat format, VkColorSpaceKHR colorSpace, VkPresentModeKHR presentMode);

	~Surface();

	void setPhysicalDevice(VkPhysicalDevice hPhysicalDevice);

	bool isQueueFamilySupported(uint32_t queueFamilyIndex) const;

	SwapchainConfigurations getSwapchainConfigurations(VkExtent2D windowExtent) const;

private:
	VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &surfaceFormats) const;

	VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> &presentModes) const;

	static VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR &swapChainCapabilities, VkExtent2D windowExtent);


public:
	VkSurfaceKHR handle_;

private:
	VkInstance hInstance_;
	VkPhysicalDevice hPhysicalDevice_;

	VkFormat format_;
	VkColorSpaceKHR colorSpace_;
	VkPresentModeKHR presentMode_;
};


#endif //PICOFRAMEWORK_SURFACE_H
