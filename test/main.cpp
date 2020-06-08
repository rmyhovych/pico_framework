//
// Created by rmyho on 5/19/2020.
//

#include <cstdio>
#include <resource_factory.h>

#include "instance.h"
#include "window/glfw_window_manager.h"

int main()
{
	printf("Hello, World!");

	/*
	GLFWWindowManager windowManager(800, 600);

	Surface::Properties surfaceProperties{};
	surfaceProperties.format = VK_FORMAT_R8G8B8A8_UNORM;

	Device::Properties deviceProperties{};
	deviceProperties.type = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
	deviceProperties.extensions = std::vector<const char*>({VK_KHR_SWAPCHAIN_EXTENSION_NAME});

	Instance instance(&windowManager, surfaceProperties, deviceProperties);
	Renderer* pRenderer = instance.getRenderer();
	pRenderer->createSwapchain(Swapchain::Properties{true});

	*/

	GLFWWindowManager windowManager(800, 600);

	Surface::Properties surfaceProperties{};
	surfaceProperties.format = VK_FORMAT_R8G8B8A8_UNORM;

	Device::Properties deviceProperties{};
	deviceProperties.type = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
	deviceProperties.extensions = std::vector<const char*>({VK_KHR_SWAPCHAIN_EXTENSION_NAME});

	Instance instance(&windowManager, surfaceProperties, deviceProperties);

	Device* d = instance.getDevice();

	ResourceFactory resourceFactory(d);
	resourceFactory.createImage(VkExtent2D{500, 500}, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	return 0;
}