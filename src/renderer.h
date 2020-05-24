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
	Renderer(VkInstance instance, Surface* surfacePtr, Device* devicePtr);
	~Renderer();

	void draw();

	Swapchain* createSwapchain();

private:
	VkInstance instance_;

	Surface* surfacePtr_;
	Device* devicePtr_;

	std::vector<Swapchain*> swapchains_;
};


#endif //PICOFRAMEWORK_RENDERER_H
