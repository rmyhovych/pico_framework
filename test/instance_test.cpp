//
// Created by ross on 2020-09-21.
//

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "instance.h"

#include <string>
#include <window/glfw_window_manager.h>
#include <resources/allocator.h>
#include <resources/resource_factory.h>

#include "renderpass/render_pass.h"
#include "pipeline/pipeline.h"

struct Vertex {
	glm::vec3 position;
};

Pipeline createPipeline(
		const Device& device,
		const SwapchainConfigurations& configurations,
		const ShaderStages& shaderStages,
		const DescriptorSetLayout& descriptorSetLayout,
		const RenderPass& renderPass
		);


int main()
{
	// WINDOW
	GLFWWindowManager windowManager(200, 200);


	// INSTANCE

	VkApplicationInfo appInfo = {};

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan Test";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.pEngineName = "PicoFramework";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	Instance instance = Instance(appInfo, &windowManager);


	// DEVICE
	std::vector<PhysicalDevice> physicalDevices = instance.getPhysicalDevices();

	VkPhysicalDeviceProperties p;
	vkGetPhysicalDeviceProperties(physicalDevices[0].handle_, &p);

	Surface surface(instance.getSurfaceHandle(), VK_FORMAT_A8B8G8R8_UNORM_PACK32, VK_COLORSPACE_SRGB_NONLINEAR_KHR, VK_PRESENT_MODE_FIFO_KHR);

	SwapchainConfigurations configurations = surface.getSwapchainConfigurations(physicalDevices[0].handle_, windowManager.getExtent());
	Device device(&physicalDevices[0], {VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_TRANSFER_BIT}, {VK_KHR_SWAPCHAIN_EXTENSION_NAME});


	// RENDER PASS
	RenderPass renderPass = RenderPass::Builder(device)
			.pushBackColor(VK_FORMAT_A2B10G10R10_SINT_PACK32)
			.pushBackDepth(physicalDevices[0].pickSupportedDepthFormat())
			.build();


	// PIPELINE
	ShaderStages shaders;
	shaders
			.addModule(device, "/home/ross/code/pico_framework/shaders/base.vert", VK_SHADER_STAGE_VERTEX_BIT)
			.addModule(device, "/home/ross/code/pico_framework/shaders/base.frag", VK_SHADER_STAGE_FRAGMENT_BIT);

	DescriptorSetLayout descriptorSetLayout = DescriptorSetLayout::Builder()
			.pushBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.build(device);

	Pipeline pipeline = createPipeline(device, configurations, shaders, descriptorSetLayout, renderPass);
	std::cout << pipeline.handle_ << std::endl;



	// RESOURCES
	Allocator allocator(instance, device);
	ResourceFactory resourceFactory(device, &allocator);


	// INPUT BUFFERS
	std::vector<Vertex> vertices({{{-0.5f, 0.5f, 0.0f}}, {{0.5f, 0.5f, 0.0f}}, {{0.0f, -0.5f, 0.0f}}});
	BufferAllocation vertexBuffer = resourceFactory.createBuffer(vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_GPU_ONLY);
	std::cout << vertexBuffer.allocation << std::endl;

	// CLEAN UP

	resourceFactory.destroyBuffer(vertexBuffer);

	shaders.destroy(device);
	pipeline.destroy(device);
	descriptorSetLayout.destroy(device);
	renderPass.destroy(device.handle_);

	allocator.destroy();
	surface.destroy(instance.handle_);
	device.destroy();
	instance.destroy();

	return 0;
}


Pipeline createPipeline(
		const Device& device,
		const SwapchainConfigurations& configurations,
		const ShaderStages& shaderStages,
		const DescriptorSetLayout& descriptorSetLayout,
		const RenderPass& renderPass
		)
{
	StateManager stateManager;
	stateManager.setVertexInput(sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX)->
			pushAttribute(VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, position));

	stateManager.setInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	stateManager.setViewport(configurations.extent);
	stateManager.setRasterization(VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_CULL_MODE_BACK_BIT);
	stateManager.setMultisample();
	stateManager.setDepthStencil(VK_FALSE, VK_FALSE, VK_COMPARE_OP_LESS);

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	stateManager.setColorBlend()->addAttachment(colorBlendAttachment);
	stateManager.setDynamic();

	PipelineLayout pipelineLayout(device, descriptorSetLayout);
	Pipeline pipeline = Pipeline::Builder()
			.linkShaders(&shaderStages)
			.linkStates(&stateManager)
			.linkLayout(&pipelineLayout)
			.linkRenderPass(&renderPass)
			.build(device);
	pipelineLayout.destroy(device);

	return pipeline;
}

