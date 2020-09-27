//
// Created by ross on 2020-09-21.
//

#include "instance.h"

#include <string>
#include <window/glfw_window_manager.h>
#include <renderpass/render_pass_builder.h>

int main()
{
	VkApplicationInfo appInfo = {};

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan Test";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.pEngineName = "PicoFramework";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	GLFWWindowManager windowManager(200, 200);

	Instance instance = Instance(appInfo, &windowManager);
	std::vector<PhysicalDevice> physicalDevices = instance.getPhysicalDevices();

	VkPhysicalDeviceProperties p;
	vkGetPhysicalDeviceProperties(physicalDevices[0].handle_, &p);

	Surface surface(instance.getSurfaceHandle(), VK_FORMAT_A8B8G8R8_UNORM_PACK32, VK_COLORSPACE_SRGB_NONLINEAR_KHR, VK_PRESENT_MODE_FIFO_KHR);

	SwapchainConfigurations configurations = surface.getSwapchainConfigurations(physicalDevices[0].handle_, windowManager.getExtent());

	Device device(&physicalDevices[0], {VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_TRANSFER_BIT}, {VK_KHR_SWAPCHAIN_EXTENSION_NAME});

	RenderPass renderPass = RenderPassBuilder()
			.pushBackColor(VK_FORMAT_A8B8G8R8_UNORM_PACK32)
			.pushBackDepth(VK_FORMAT_D24_UNORM_S8_UINT)
			.build(device.handle_);


	renderPass.destroy(device.handle_);
	surface.destroy(instance.handle_);
	device.destroy();
	instance.destroy();

	return 0;
}