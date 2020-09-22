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
	struct Properties
	{
		VkFormat format;
	};

	explicit Surface(const Surface::Properties &properties, VkInstance instance, WindowManager* pWindowManager);

	void destroy();

	SwapchainConfigurations getSwapchainConfigurations(VkPhysicalDevice physicalDevice) const;

	VkSurfaceKHR getHandle() const;

private:
	VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &surfaceFormats) const;

	static VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> &presentModes);

	static VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR &swapChainCapabilities, VkExtent2D windowExtent);


private:
	VkSurfaceKHR handle_;

	VkInstance instance_;
	WindowManager* pWindowManager_;

	VkFormat format_;
};


#endif //PICOFRAMEWORK_SURFACE_H
