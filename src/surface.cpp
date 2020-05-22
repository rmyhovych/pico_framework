//
// Created by rmyho on 5/19/2020.
//

#include "surface.h"

#include <exception>
#include <algorithm>

Surface::Surface(Surface::Properties properties, VkInstance instance, AbsWindowManager* windowManagerPtr) :
		properties_(properties),
		instance_(instance),
		windowManagerPtr_(windowManagerPtr),

		handle_(VK_NULL_HANDLE)
{
	if (instance == VK_NULL_HANDLE)
	{
		throw std::exception("Can't construct surface with NULL instance handle!");
	}
	if (windowManagerPtr == nullptr)
	{
		throw std::exception("Can't init Surface with NULL window manager!");
	}

	windowManagerPtr_ = windowManagerPtr;
	handle_ = windowManagerPtr_->createSurfaceHandle(instance_);
}


void Surface::destroy()
{
	windowManagerPtr_ = nullptr;
	if (handle_ != VK_NULL_HANDLE)
	{
		vkDestroySurfaceKHR(instance_, handle_, nullptr);
		handle_ = VK_NULL_HANDLE;
	}
}

SwapchainConfigurations Surface::getSwapchainConfigurations(VkPhysicalDevice physicalDevice) const
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


	VkExtent2D windowExtent = windowManagerPtr_->getWindowSize();
	configurations.extent = chooseExtent(configurations.surfaceCapabilities, windowExtent);


	return configurations;
}

VkSurfaceFormatKHR Surface::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &surfaceFormats) const
{
	for (const VkSurfaceFormatKHR &format : surfaceFormats)
	{
		if (format.format == properties_.format && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return format;
		}
	}

	return surfaceFormats[0];
}

VkPresentModeKHR Surface::choosePresentMode(const std::vector<VkPresentModeKHR> &presentModes)
{
	for (const VkPresentModeKHR &presentMode : presentModes)
	{
		if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
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

