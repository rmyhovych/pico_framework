//
// Created by ross on 2020-11-06.
//

#ifndef PICOFRAMEWORK_RENDERER_H
#define PICOFRAMEWORK_RENDERER_H


#include <swapchain/swapchain.h>
#include <primitive/object_descriptor.h>
#include <renderpass/render_pass.h>
#include <pipeline/pipeline.h>
#include "device/device.h"
#include "frame_manager.h"

class Renderer
{
public:
	Renderer(const Device* pDevice, const Swapchain* pSwapchain, const RenderPass* pRenderPass, const Pipeline* pPipeline, ResourceFactory* pResourceFactory, const SwapchainConfigurations &swapchainConfigurations);

	~Renderer();

	void destroy();

	template<typename T>
	ObjectDescriptor* createObjectDescriptor(const std::vector<T> &vertices, const std::vector<uint16_t> &indexes);

	void recordCommands();

	void render();

private:
	static VkCommandPool createCommandPool(const Device* pDevice);

private:
	const Device* pDevice_;
	const Swapchain* pSwapchain_;
	const RenderPass* pRenderPass_;
	const Pipeline* pPipeline_;
	ResourceFactory* pResourceFactory_;
	const SwapchainConfigurations swapchainConfigurations_;

	FrameManager frameManager_;

	VkQueue graphicsQueue_;

	std::vector<ObjectDescriptor> objectDescriptors_;

	std::vector<VkFramebuffer> framebuffers_;
	VkCommandPool commandPool_;
	std::vector<VkCommandBuffer> commandBuffers_;
};

template<typename T>
ObjectDescriptor* Renderer::createObjectDescriptor(const std::vector<T> &vertices, const std::vector<uint16_t> &indexes)
{
	objectDescriptors_.push_back(ObjectDescriptor(*pResourceFactory_, vertices, indexes));
	return &objectDescriptors_.back();
}


#endif //PICOFRAMEWORK_RENDERER_H
