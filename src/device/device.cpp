//
// Created by rmyho on 5/19/2020.
//

#include "device.h"
#include <set>
#include <utility>


Device::Device(VkInstance hInstance, VkPhysicalDevice hPhysicalDevice, std::vector<DeviceQueue> &deviceQueues, const std::vector<const char*> &deviceExtensions) :
		handle_(VK_NULL_HANDLE),

		hInstance_(hInstance),
		hPhysicalDevice_(hPhysicalDevice),
		deviceQueues_(std::move(deviceQueues))
{

	handle_ = createHandle(hPhysicalDevice, deviceQueues_, deviceExtensions);

	for (DeviceQueue &deviceQueue : deviceQueues_)
		vkGetDeviceQueue(handle_, deviceQueue.family, 0, &deviceQueue.queue);
}

Device::~Device()
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


ResourceFactory Device::createResourceFactory() const
{
	return ResourceFactory(hInstance_, handle_, hPhysicalDevice_, getQueue(VK_QUEUE_TRANSFER_BIT));
}


ShaderStages Device::createShaderStages() const
{
	return ShaderStages(handle_);
}

RenderPass::Builder Device::createRenderPassBuilder() const
{
	return RenderPass::Builder(handle_);
}

DescriptorSetLayout::Builder Device::createDescriptorSetLayoutBuilder() const
{
	return DescriptorSetLayout::Builder(handle_);
}

Swapchain::Builder Device::createSwapchainBuilder(const Surface* pSurface, ResourceFactory* pResourceFactory) const
{
	const DeviceQueue* pGraphicsQueue = getQueue(VK_QUEUE_GRAPHICS_BIT);
	return Swapchain::Builder(handle_, pGraphicsQueue, pSurface, pResourceFactory);
}

Pipeline::Builder Device::createPipelineBuilder() const
{
	return Pipeline::Builder(handle_);
}


const DeviceQueue* Device::getQueue(VkQueueFlags type) const
{
	for (const DeviceQueue &deviceQueue : deviceQueues_)
	{
		if ((deviceQueue.type | type) == type)
			return &deviceQueue;
	}

	return nullptr;
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

#ifndef NDEBUG
	deviceCreateInfo.enabledLayerCount = (uint32_t) VALIDATION_LAYERS.size();
	deviceCreateInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
#else
	deviceCreateInfo.enabledLayerCount = 0;
	deviceCreateInfo.ppEnabledLayerNames = nullptr;
#endif // !NDEBUG


	VkDevice device;
	CALL_VK(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device))

	return device;
}

