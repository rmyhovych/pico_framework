//
// Created by rmyho on 5/19/2020.
//

#include "instance.h"

#include <vector>

Instance::Instance(const VkApplicationInfo &applicationInfo, const WindowManager* pWindowManager) :
		handle_(VK_NULL_HANDLE),
		pWindowManager_(pWindowManager),

		surface_(VK_NULL_HANDLE)
{
	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;

	instanceCreateInfo.enabledLayerCount = 0;
	instanceCreateInfo.ppEnabledLayerNames = nullptr;

	const std::vector<const char*> requiredExtensions = pWindowManager_->getInstanceExtensions();
	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

	CALL_VK(vkCreateInstance(&instanceCreateInfo, nullptr, &handle_))

	surface_ = pWindowManager_->createSurfaceHandle(handle_);
}

Instance::~Instance()
{
	CHECK_NULL_HANDLE(handle_)
}

void Instance::destroy()
{
	vkDestroyInstance(handle_, nullptr);
	handle_ = VK_NULL_HANDLE;
}


VkSurfaceKHR Instance::getSurfaceHandle() const
{
	return surface_;
}

std::vector<PhysicalDevice> Instance::getPhysicalDevices()
{
	std::vector<PhysicalDevice> physicalDevices = std::vector<PhysicalDevice>();

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(handle_, &deviceCount, nullptr);

	std::vector<VkPhysicalDevice> physicalDeviceHandles(deviceCount);
	vkEnumeratePhysicalDevices(handle_, &deviceCount, physicalDeviceHandles.data());

	physicalDevices.reserve(deviceCount);
	for (VkPhysicalDevice physicalDeviceHandle : physicalDeviceHandles)
		physicalDevices.emplace_back(physicalDeviceHandle);

	return physicalDevices;
}
