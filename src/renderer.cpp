//
// Created by rmyho on 5/24/2020.
//

#include "renderer.h"

Renderer::Renderer(VkInstance instance, Surface* pSurface, Device* pDevice) :
		instance_(instance),
		pSurface_(pSurface),
		pDevice_(pDevice),

		swapchains_(0)
{
}

Renderer::~Renderer()
{
	for (Swapchain* swapchain : swapchains_)
	{
		delete swapchain;
	}

	swapchains_.clear();
}

void Renderer::draw()
{
	// TODO: Manage rendering of swapchains / resizing / synchronization
}

Swapchain* Renderer::createSwapchain(const Swapchain::Properties &swapchainProperties)
{
	Swapchain* newSwapchain = new Swapchain(swapchainProperties, pSurface_->handle_, pDevice_);

	VkPhysicalDevice physicalDeviceHandle = pDevice_->pPhysicalDevice_->handle_;
	//newSwapchain->init(pSurface_->getSwapchainConfigurations(physicalDeviceHandle));

	swapchains_.push_back(newSwapchain);
	return newSwapchain;
}
