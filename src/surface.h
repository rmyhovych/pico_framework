//
// Created by rmyho on 5/19/2020.
//

#ifndef PICOFRAMEWORK_SURFACE_H
#define PICOFRAMEWORK_SURFACE_H

#include <cstdint>
#include <pfvk.h>
#include <vector>

#include "window/window_manager.h"

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
	explicit Surface(VkSurfaceKHR handle, VkFormat format, VkColorSpaceKHR colorSpace, VkPresentModeKHR presentMode);

	SwapchainConfigurations getSwapchainConfigurations(VkPhysicalDevice physicalDevice, VkExtent2D windowExtent) const;

	VkSurfaceKHR getHandle() const;

private:
	VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &surfaceFormats) const;

	VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> &presentModes) const;

	static VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR &swapChainCapabilities, VkExtent2D windowExtent);

private:
	VkSurfaceKHR handle_;

	VkFormat format_;
	VkColorSpaceKHR colorSpace_;
	VkPresentModeKHR presentMode_;
};


#endif //PICOFRAMEWORK_SURFACE_H
