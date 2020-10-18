//
// Created by ross on 2020-09-21.
//

#include "instance.h"

#include <string>
#include <window/glfw_window_manager.h>
#include <resources/allocator.h>
#include <resources/resource_factory.h>

#include "renderpass/render_pass.h"


int main()
{
	VkApplicationInfo appInfo = {};

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan Test";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.pEngineName = "PicoFramework";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	GLFWWindowManager windowManager(200, 200);

	Instance instance = Instance(appInfo, &windowManager);
	std::vector<PhysicalDevice> physicalDevices = instance.getPhysicalDevices();

	VkPhysicalDeviceProperties p;
	vkGetPhysicalDeviceProperties(physicalDevices[0].handle_, &p);

	Surface surface(instance.getSurfaceHandle(), VK_FORMAT_A8B8G8R8_UNORM_PACK32, VK_COLORSPACE_SRGB_NONLINEAR_KHR, VK_PRESENT_MODE_FIFO_KHR);

	SwapchainConfigurations configurations = surface.getSwapchainConfigurations(physicalDevices[0].handle_, windowManager.getExtent());

	Device device(&physicalDevices[0], {VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_TRANSFER_BIT}, {VK_KHR_SWAPCHAIN_EXTENSION_NAME});

	Allocator allocator(instance, device);
	ResourceFactory resourceFactory(device, &allocator);

	BufferAllocation bufferAllocation = resourceFactory.createBuffer(64, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_ONLY);

	uint32_t* ptr = (uint32_t*) allocator.map(bufferAllocation);
	ptr[0] = 12;
	allocator.unmap(bufferAllocation);

	allocator.free(bufferAllocation);

	RenderPass renderPass = RenderPass::Builder(device)
			.pushBackColor(VK_FORMAT_A2B10G10R10_SINT_PACK32)
			.pushBackDepth(physicalDevices[0].pickSupportedDepthFormat())
			.build();

	renderPass.destroy(device.handle_);

	allocator.destroy();
	surface.destroy(instance.handle_);
	device.destroy();
	instance.destroy();

	return 0;
}
