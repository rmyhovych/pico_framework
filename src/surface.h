//
// Created by rmyho on 5/19/2020.
//

#ifndef PICOFRAMEWORK_SURFACE_H
#define PICOFRAMEWORK_SURFACE_H

#include <cstdint>
#include <vulkan/vulkan.h>
#include <vector>

#include "window/abs_window_manager.h"

class Surface
{
public:
	struct Properties
	{
		VkPresentModeKHR presentMode;
		VkSurfaceFormatKHR surfaceFormat;
		VkExtent2D extent;
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
	};

	explicit Surface(VkInstance instance);

	void init(VkFormat format, AbsWindowManager* windowManagerPtr);
	void destroy();

	Surface::Properties getProperties(VkPhysicalDevice physicalDevice) const;

private:
	VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &surfaceFormats) const;

	static VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> &presentModes);

	static VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR &swapChainCapabilities, VkExtent2D windowExtent);


private:
	VkInstance instance_;
	AbsWindowManager* windowManagerPtr_;

	VkSurfaceKHR handle_;
	VkFormat format_;
};


#endif //PICOFRAMEWORK_SURFACE_H
