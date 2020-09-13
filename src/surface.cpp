//
// Created by rmyho on 5/19/2020.
//

#include "surface.h"

#include <exception>
#include <algorithm>

Surface::Surface(const Surface::Properties &properties, VkInstance instance, AbsWindowManager* pWindowManager) :
		handle_(VK_NULL_HANDLE),
		instance_(instance),
		pWindowManager_(pWindowManager),
		format_(properties.format)
{
	if (instance == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Can't construct surface with NULL instance handle!");
	}
	if (pWindowManager == nullptr)
	{
		throw std::runtime_error("Can't init Surface with NULL window manager!");
	}

	pWindowManager_ = pWindowManager;
	handle_ = pWindowManager_->createSurfaceHandle(instance_);
}


void Surface::destroy()
{
	pWindowManager_ = nullptr;
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


	VkExtent2D windowExtent = pWindowManager_->getWindowSize();
	configurations.extent = chooseExtent(configurations.surfaceCapabilities, windowExtent);


	return configurations;
}

VkSurfaceKHR Surface::getHandle() const
{
	return handle_;
}


VkSurfaceFormatKHR Surface::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &surfaceFormats) const
{
	for (const VkSurfaceFormatKHR &format : surfaceFormats)
	{
		if (format.format == format_ && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
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

