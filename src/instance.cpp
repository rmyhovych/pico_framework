//
// Created by rmyho on 5/19/2020.
//

#include "instance.h"

#include <vector>

Instance::Instance(const VkApplicationInfo &applicationInfo, const std::vector<const char*> &requiredExtensions) :
		handle_(VK_NULL_HANDLE)
{
	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;

	instanceCreateInfo.enabledLayerCount = 0;
	instanceCreateInfo.ppEnabledLayerNames = nullptr;

	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

	CALL_VK(vkCreateInstance(&instanceCreateInfo, nullptr, &handle_))
}

Instance::~Instance()
{
	vkDestroyInstance(handle_, nullptr);
}

VkInstance Instance::getHandle() const
{
	return handle_;
}

std::vector<PhysicalDevice> Instance::getPhysicalDevices()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(handle_, &deviceCount, nullptr);

	std::vector<VkPhysicalDevice> physicalDeviceHandles(deviceCount);
	vkEnumeratePhysicalDevices(handle_, &deviceCount, physicalDeviceHandles.data());

	return std::vector<PhysicalDevice>();
}
