//
// Created by rmyho on 5/19/2020.
//

#include <cstdio>
#include "allocator/buffer_allocator.h"

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

	BuddyTree tree(256, 10);
	Node* n0 = tree.fillFreeNode(100);
	Node* n1 = tree.fillFreeNode(400);
	Node* n2 = tree.fillFreeNode(200);
	Node* n3 = tree.fillFreeNode(200);
	Node* n4 = tree.fillFreeNode(400);

	GLFWWindowManager windowManager(800, 600);

	Surface::Properties surfaceProperties{};
	surfaceProperties.format = VK_FORMAT_R8G8B8A8_UNORM;

	Device::Properties deviceProperties{};
	deviceProperties.type = VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
	deviceProperties.extensions = std::vector<const char*>({VK_KHR_SWAPCHAIN_EXTENSION_NAME});

	Instance instance(&windowManager, surfaceProperties, deviceProperties);

	Device* d = instance.getDevice();

	//BufferAllocator alloc(d->getHandle(), d->getPhysicalDevice().getHandle());

	return 0;
}