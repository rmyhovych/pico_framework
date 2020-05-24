//
// Created by rmyho on 5/19/2020.
//

#include "instance.h"

#include <vector>


Instance::Instance(AbsWindowManager* windowManagerPtr, const Surface::Properties& surfaceProperties, const Device::Properties& deviceProperties) :
		handle_(createInstanceHandle(windowManagerPtr)),
		windowManagerPtr_(windowManagerPtr),

		surface_(surfaceProperties, handle_, windowManagerPtr),
		device_(deviceProperties, handle_, surface_)
{
}

Instance::~Instance()
{
	if (renderer_ != nullptr)
	{
		delete renderer_;
	}

	device_.destroy();
	surface_.destroy();

	vkDestroyInstance(handle_, nullptr);
}


Renderer* Instance::getRenderer()
{
	if (renderer_ == nullptr)
	{
		renderer_ = new Renderer(handle_, &surface_, &device_);
	}

	return renderer_;
}


VkInstance Instance::createInstanceHandle(AbsWindowManager* windowManagerPtr)
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

	std::vector<const char*> requiredExtensions = windowManagerPtr->getRequiredInstanceExtensions();

	instanceCreateInfo.enabledLayerCount = 0;
	instanceCreateInfo.ppEnabledLayerNames = nullptr;

	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

	VkInstance instance;
	if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS)
	{
		throw std::exception("Failed to initialize instance.");
	}

	return instance;
}
