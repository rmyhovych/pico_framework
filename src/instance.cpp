//
// Created by rmyho on 5/19/2020.
//

#include "instance.h"

#include <vector>


/*----------------------------------------------------------------------------------------------------------*/
Instance::Instance() :
		instance_(VK_NULL_HANDLE)
{
	VkApplicationInfo appInfo = {};

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan Test";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.pEngineName = "PicoEngine";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;


	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtentionCount = 0;
	const char** glfwExtentions = nullptr;//glfwGetRequiredInstanceExtensions(&glfwExtentionCount);

	std::vector<const char*> requiredExtensions(glfwExtentions, glfwExtentions + glfwExtentionCount);

	instanceCreateInfo.enabledLayerCount = 0;
	instanceCreateInfo.ppEnabledLayerNames = nullptr;

	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

	if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance_) != VK_SUCCESS)
	{
		throw std::exception("Failed to initialize instance.");
	}
}

/*----------------------------------------------------------------------------------------------------------*/
Instance::~Instance()
{

}

/*----------------------------------------------------------------------------------------------------------*/
void Instance::createSurface(VkSurfaceKHR surfaceHandle)
{

}

/*----------------------------------------------------------------------------------------------------------*/
void Instance::createDevice()
{

}

