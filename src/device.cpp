//
// Created by rmyho on 5/19/2020.
//

#include "device.h"
#include <set>

Device::Device(const PhysicalDevice* pPhysicalDevice, const std::vector<VkQueueFlags> &queueFlags, const std::vector<const char*> &deviceExtensions) :
		handle_(VK_NULL_HANDLE),
		pPhysicalDevice_(pPhysicalDevice),
		deviceQueues_(0)
{
	deviceQueues_.reserve(queueFlags.size());
	for (VkQueueFlags queueFlag : queueFlags)
		deviceQueues_.push_back({VK_NULL_HANDLE, 0, queueFlag});

	pPhysicalDevice_->pickQueueFamilies(deviceQueues_);

	handle_ = createHandle(pPhysicalDevice_->handle_, deviceQueues_, deviceExtensions);

	for (DeviceQueue &deviceQueue : deviceQueues_)
		vkGetDeviceQueue(handle_, deviceQueue.family, 0, &deviceQueue.queue);
}

Device::~Device()
{
	CHECK_NULL_HANDLE(handle_)
}

void Device::destroy()
{
	vkDestroyDevice(handle_, nullptr);
	handle_ = VK_NULL_HANDLE;
}


VkCommandPool Device::createCommandPool(VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex) const
{
	VkCommandPoolCreateInfo commandPoolCreateInfo{};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
	commandPoolCreateInfo.flags = flags;
	commandPoolCreateInfo.pNext = nullptr;

	VkCommandPool commandPool;
	CALL_VK(vkCreateCommandPool(handle_, &commandPoolCreateInfo, nullptr, &commandPool))

	return commandPool;
}

VkDevice Device::createHandle(VkPhysicalDevice physicalDevice, const std::vector<DeviceQueue> &queues, const std::vector<const char*> &deviceExtensions)
{
	std::set<uint32_t> uniqueQueueFamilies;
	for (const DeviceQueue &q : queues)
		uniqueQueueFamilies.insert(q.family);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	float priority = 1.0f;
	for (uint32_t queueFamilyIndex : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &priority;

		queueCreateInfos.push_back(queueCreateInfo);
	}


	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

	VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
	physicalDeviceFeatures.samplerAnisotropy = VK_TRUE;

	deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
	deviceCreateInfo.enabledExtensionCount = (uint32_t) deviceExtensions.size();

	deviceCreateInfo.enabledLayerCount = 0;
	deviceCreateInfo.ppEnabledLayerNames = nullptr;

	VkDevice device;
	CALL_VK(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device))

	return device;
}

