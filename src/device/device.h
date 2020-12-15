//
// Created by rmyho on 5/19/2020.
//

#ifndef PICOFRAMEWORK_DEVICE_H
#define PICOFRAMEWORK_DEVICE_H

#include <pfvk.h>
#include <vector>
#include <pipeline/shader_stages.h>
#include <renderpass/render_pass.h>
#include <pipeline/layout/layout.h>
#include <swapchain/swapchain.h>
#include <pipeline/pipeline.h>

#include "surface.h"
#include "resources/resource_factory.h"

class Device
{
public:
	Device(VkInstance hInstance, VkPhysicalDevice hPhysicalDevice, std::vector<DeviceQueue> &deviceQueues, const std::vector<const char*> &deviceExtensions);

	~Device();

	VkCommandPool createCommandPool(VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex) const;

	ResourceFactory createResourceFactory() const;

	ShaderStages createShaderStages() const;

	RenderPass::Builder createRenderPassBuilder() const;

	Layout::Builder createDescriptorSetLayoutBuilder() const;

	Swapchain::Builder createSwapchainBuilder(const Surface* pSurface, ResourceFactory* pResourceFactory) const;

	Pipeline::Builder createPipelineBuilder() const;

	const DeviceQueue* getQueue(VkQueueFlags type) const;

private:
	static VkDevice createHandle(VkPhysicalDevice physicalDevice, const std::vector<DeviceQueue> &queues, const std::vector<const char*> &deviceExtensions);

public:
	VkDevice handle_;

private:
	VkInstance hInstance_;
	VkPhysicalDevice hPhysicalDevice_;
	std::vector<DeviceQueue> deviceQueues_;
};


#endif //PICOFRAMEWORK_DEVICE_H
