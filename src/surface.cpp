//
// Created by rmyho on 5/19/2020.
//

#include "surface.h"

#include <algorithm>

#include "physical_device.h"

Surface::Surface(VkSurfaceKHR handle, VkFormat format, VkColorSpaceKHR colorSpace, VkPresentModeKHR presentMode) :
		handle_(handle),

		format_(format),
		colorSpace_(colorSpace),
		presentMode_(presentMode)
{
}

void Surface::destroy(VkInstance instance)
{
	vkDestroySurfaceKHR(instance, handle_, nullptr);
	handle_ = VK_NULL_HANDLE;
}


bool Surface::isQueueFamilySupported(const PhysicalDevice &physicalDevice, uint32_t queueFamilyIndex) const
{
	VkBool32 supported;
	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice.handle_, queueFamilyIndex, handle_, &supported);

	return supported == VK_TRUE;
}

SwapchainConfigurations Surface::getSwapchainConfigurations(VkPhysicalDevice physicalDevice, VkExtent2D windowExtent) const
{
	SwapchainConfigurations configurations{};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, handle_, &configurations.surfaceCapabilities);


	std::vector<VkSurfaceFormatKHR> swapChainFormats;
	uint32_t nSurfaceFormats = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, handle_, &nSurfaceFormats, nullptr);
	if (nSurfaceFormats != 0)
	{
		swapChainFormats.resize(nSurfaceFormats);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, handle_, &nSurfaceFormats, swapChainFormats.data());
	}
	configurations.surfaceFormat = chooseSurfaceFormat(swapChainFormats);


	std::vector<VkPresentModeKHR> presentModes;
	uint32_t nPresentModes = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, handle_, &nPresentModes, nullptr);
	if (nPresentModes != 0)
	{
		presentModes.resize(nPresentModes);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, handle_, &nPresentModes, presentModes.data());
	}
	configurations.presentMode = choosePresentMode(presentModes);

	configurations.extent = chooseExtent(configurations.surfaceCapabilities, windowExtent);

	return configurations;
}


VkSurfaceFormatKHR Surface::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &surfaceFormats) const
{
	for (const VkSurfaceFormatKHR &format : surfaceFormats)
	{
		if (format.format == format_ && format.colorSpace == colorSpace_)
		{
			return format;
		}
	}

	return surfaceFormats[0];
}

VkPresentModeKHR Surface::choosePresentMode(const std::vector<VkPresentModeKHR> &presentModes) const
{
	for (const VkPresentModeKHR &presentMode : presentModes)
	{
		if (presentMode == presentMode_)
		{
			return presentMode;
		}
	}

	// always available
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Surface::chooseExtent(const VkSurfaceCapabilitiesKHR &swapChainCapabilities, VkExtent2D windowExtent)
{
	if (swapChainCapabilities.currentExtent.width != UINT32_MAX)
	{
		return swapChainCapabilities.currentExtent;
	}

	VkExtent2D extentToUse{0, 0};

	extentToUse.width = std::max(swapChainCapabilities.maxImageExtent.width,
	                             std::min(swapChainCapabilities.maxImageExtent.width, windowExtent.width));
	extentToUse.height = std::max(swapChainCapabilities.maxImageExtent.height,
	                              std::min(swapChainCapabilities.maxImageExtent.height, windowExtent.height));

	return extentToUse;
}
