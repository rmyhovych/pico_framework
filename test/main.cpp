//
// Created by ross on 2020-09-21.
//

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "instance.h"

#include <window/glfw_window_manager.h>
#include <resources/allocator.h>
#include <resources/resource_factory.h>
#include <renderer/renderer.h>

#include "renderpass/render_pass.h"
#include "pipeline/pipeline.h"

#include <thread>

struct Vertex
{
	alignas(16) glm::vec3 position;
};

Pipeline createPipeline(
		const Device &device,
		const SwapchainConfigurations &configurations,
		const ShaderStages &shaderStages,
		const Layout &descriptorSetLayout,
		const RenderPass &renderPass
);


int main()
{
	// WINDOW
	GLFWWindowManager windowManager(500, 400);


	// INSTANCE

	VkApplicationInfo appInfo = {};

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan Test";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.pEngineName = "PicoFramework";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	Instance instance = Instance(appInfo, windowManager.getInstanceExtensions());


	// DEVICE
	std::vector<PhysicalDevice> allPhysicalDevices = instance.getPhysicalDevices();
	Surface surface = instance.createSurface(
			&windowManager,
			SurfaceProperties{
					VK_FORMAT_A8B8G8R8_UNORM_PACK32,
					VK_COLORSPACE_SRGB_NONLINEAR_KHR,
					VK_PRESENT_MODE_FIFO_KHR});

	PhysicalDevice physicalDevice = allPhysicalDevices[0];
	Device device = physicalDevice.createDevice();

	surface.setPhysicalDevice(physicalDevice.handle_);
	SwapchainConfigurations configurations = surface.getSwapchainConfigurations(windowManager.getExtent());


	// RENDER PASS
	RenderPass renderPass = device.createRenderPassBuilder()
			.pushBackColor(configurations.surfaceFormat.format)
					//.pushBackDepth(physicalDevices[0].pickSupportedDepthFormat())
			.build();


	// PIPELINE
	ShaderStages shaders = device.createShaderStages();
	shaders
			.addModule("/home/ross/code/pico_framework/shaders/base.vert.spv", VK_SHADER_STAGE_VERTEX_BIT)
			.addModule("/home/ross/code/pico_framework/shaders/base.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

	Layout layout = device.createDescriptorSetLayoutBuilder()
			.build();

	Pipeline pipeline = createPipeline(device, configurations, shaders, layout, renderPass);

	// RESOURCES
	ResourceFactory resourceFactory = device.createResourceFactory();

	Swapchain swapchain = device.createSwapchainBuilder(&surface, &resourceFactory).build(configurations);

	std::vector<Vertex> vertices({{{-0.5f, 0.5f,  0.0f}},
	                              {{0.5f,  0.5f,  0.0f}},
	                              {{0.0,   -0.5f, 0.0f}}});
	std::vector<uint16_t> indexes({0,
	                               1,
	                               2});

	Renderer renderer(&device, &swapchain, &renderPass, &pipeline, &resourceFactory, configurations);
	renderer.createObjectDescriptor(vertices, indexes);

	renderer.recordCommands();

	while (windowManager.isOpen())
	{
		glfwPollEvents();
		renderer.render();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	// CLEAN UP
	renderer.destroy();

	return 0;
}


Pipeline createPipeline(
		const Device &device,
		const SwapchainConfigurations &configurations,
		const ShaderStages &shaderStages,
		const Layout &layout,
		const RenderPass &renderPass
)
{
	StateManager stateManager;
	stateManager.setVertexInput(sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX)->
			pushAttribute(VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, position));

	stateManager.setInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	stateManager.setViewport(configurations.extent);
	stateManager.setRasterization(VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_CULL_MODE_NONE);
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

	Pipeline pipeline = device.createPipelineBuilder()
			.linkShaders(&shaderStages)
			.linkStates(&stateManager)
			.linkLayout(&layout)
			.linkRenderPass(&renderPass)
			.build();

	return pipeline;
}

