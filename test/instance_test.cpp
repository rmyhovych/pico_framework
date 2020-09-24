//
// Created by ross on 2020-09-21.
//

#include "instance.h"

#include <string>
#include <window/glfw_window_manager.h>

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
	vkGetPhysicalDeviceProperties(physicalDevices[0].getHandle(), &p);

	Surface surface(instance.getSurfaceHandle(), VK_FORMAT_A8B8G8R8_UNORM_PACK32, VK_COLORSPACE_SRGB_NONLINEAR_KHR, VK_PRESENT_MODE_FIFO_KHR);

	SwapchainConfigurations configurations = surface.getSwapchainConfigurations(physicalDevices[0].getHandle(), windowManager.getExtent());

	return 0;
}