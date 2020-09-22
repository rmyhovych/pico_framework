//
// Created by rmyho on 5/19/2020.
//

#include "instance.h"

#include <vector>


Instance::Instance(WindowManager* pWindowManager, const Surface::Properties &surfaceProperties, const Device::Properties &deviceProperties) :
		handle_(createInstanceHandle(pWindowManager)),
		pWindowManager_(pWindowManager),

		surface_(surfaceProperties, handle_, pWindowManager),
		device_(deviceProperties, handle_, surface_),

		renderer_(nullptr)
{
}

Instance::~Instance()
{
	delete renderer_;

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


VkInstance Instance::createInstanceHandle(WindowManager* pWindowManager)
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

	std::vector<const char*> requiredExtensions = pWindowManager->getRequiredInstanceExtensions();

	instanceCreateInfo.enabledLayerCount = 0;
	instanceCreateInfo.ppEnabledLayerNames = nullptr;

	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

	VkInstance instance;
	CALL_VK(vkCreateInstance(&instanceCreateInfo, nullptr, &instance))

	return instance;
}

Device* Instance::getDevice()
{
	return &device_;
}
