//
// Created by rmyho on 5/24/2020.
//

#include "renderer.h"

Renderer::Renderer(VkInstance instance, Surface* surfacePtr, Device* devicePtr) :
		instance_(instance),
		surfacePtr_(surfacePtr),
		devicePtr_(devicePtr),

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

Swapchain* Renderer::createSwapchain()
{
	Swapchain* newSwapchain = new Swapchain();

	swapchains_.push_back(newSwapchain);
	return newSwapchain;
}
