//
// Created by rmyho on 5/24/2020.
//

#ifndef PICOFRAMEWORK_RENDERER_H
#define PICOFRAMEWORK_RENDERER_H

#include <vulkan/vulkan.h>

#include "surface.h"
#include "device.h"
#include "swapchain.h"


class Renderer
{
public:
	Renderer(VkInstance instance, Surface* pSurface, Device* pDevice);

	~Renderer();

	void draw();

	Swapchain* createSwapchain(const Swapchain::Properties &swapchainProperties);

private:
	VkInstance instance_;

	Surface* pSurface_;
	Device* pDevice_;

	std::vector<Swapchain*> swapchains_;
};


#endif //PICOFRAMEWORK_RENDERER_H
