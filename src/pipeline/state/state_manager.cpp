#include "state_manager.h"

#include <memory>

StateManager::StateManager() :
		vertexInput_(nullptr),
		inputAssembly_(nullptr),
		viewport_(nullptr),
		rasterization_(nullptr),
		multisample_(nullptr),
		depthStencil_(nullptr),
		colorBlend_(nullptr),
		dynamic_(nullptr)
{
}

std::shared_ptr<VertexInputState> StateManager::setVertexInput(uint32_t stride, VkVertexInputRate inputRate)
{
	vertexInput_ = std::make_shared<VertexInputState>(stride, inputRate);
	return vertexInput_;
}

std::shared_ptr<InputAssemblyState> StateManager::setInputAssembly(VkPrimitiveTopology topology)
{
	inputAssembly_ = std::make_shared<InputAssemblyState>(topology);
	return inputAssembly_;
}

std::shared_ptr<ViewportState> StateManager::setViewport(VkExtent2D extent)
{
	viewport_ = std::make_shared<ViewportState>(extent);
	return viewport_;
}

std::shared_ptr<RasterizationState> StateManager::setRasterization(VkFrontFace frontFace, VkCullModeFlags cullMode)
{
	rasterization_ = std::make_shared<RasterizationState>(frontFace, cullMode);
	return rasterization_;
}

std::shared_ptr<MultisampleState> StateManager::setMultisample()
{
	multisample_ = std::make_shared<MultisampleState>();
	return multisample_;
}

std::shared_ptr<DepthStencilState> StateManager::setDepthStencil(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp)
{
	depthStencil_ = std::make_shared<DepthStencilState>(depthTestEnable, depthWriteEnable, depthCompareOp);
	return depthStencil_;
}

std::shared_ptr<ColorBlendState> StateManager::setColorBlend()
{
	colorBlend_ = std::make_shared<ColorBlendState>();
	return colorBlend_;
}

std::shared_ptr<DynamicState> StateManager::setDynamic()
{
	dynamic_ = std::make_shared<DynamicState>();
	return dynamic_;
}


void StateManager::applyOn(VkGraphicsPipelineCreateInfo* createInfo) const
{
	createInfo->pVertexInputState = getCreateInfo(vertexInput_.get());
	createInfo->pInputAssemblyState = getCreateInfo(inputAssembly_.get());
	createInfo->pViewportState = getCreateInfo(viewport_.get());
	createInfo->pRasterizationState = getCreateInfo(rasterization_.get());
	createInfo->pMultisampleState = getCreateInfo(multisample_.get());
	createInfo->pDepthStencilState = getCreateInfo(depthStencil_.get());
	createInfo->pColorBlendState = getCreateInfo(colorBlend_.get());
	createInfo->pDynamicState = getCreateInfo(dynamic_.get());
}

