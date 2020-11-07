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

StateManager::~StateManager()
{
	delete vertexInput_;
	vertexInput_ = nullptr;

	delete inputAssembly_;
	inputAssembly_ = nullptr;

	delete viewport_;
	viewport_ = nullptr;

	delete rasterization_;
	rasterization_ = nullptr;

	delete multisample_;
	multisample_ = nullptr;

	delete depthStencil_;
	depthStencil_ = nullptr;

	delete colorBlend_;
	colorBlend_ = nullptr;

	delete dynamic_;
	dynamic_ = nullptr;
}

VertexInputState* StateManager::setVertexInput(uint32_t stride, VkVertexInputRate inputRate)
{
	vertexInput_ = new VertexInputState(stride, inputRate);
	return vertexInput_;
}

InputAssemblyState* StateManager::setInputAssembly(VkPrimitiveTopology topology)
{
	inputAssembly_ = new InputAssemblyState(topology);
	return inputAssembly_;
}

ViewportState* StateManager::setViewport(VkExtent2D extent)
{
	viewport_ = new ViewportState(extent);
	return viewport_;
}

RasterizationState* StateManager::setRasterization(VkFrontFace frontFace, VkCullModeFlags cullMode)
{
	rasterization_ = new RasterizationState(frontFace, cullMode);
	return rasterization_;
}

MultisampleState* StateManager::setMultisample()
{
	multisample_ = new MultisampleState();
	return multisample_;
}

DepthStencilState* StateManager::setDepthStencil(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp)
{
	depthStencil_ = new DepthStencilState(depthTestEnable, depthWriteEnable, depthCompareOp);
	return depthStencil_;
}

ColorBlendState* StateManager::setColorBlend()
{
	colorBlend_ = new ColorBlendState();
	return colorBlend_;
}

DynamicState* StateManager::setDynamic()
{
	dynamic_ = new DynamicState();
	return dynamic_;
}


void StateManager::applyOn(VkGraphicsPipelineCreateInfo* createInfo) const
{
	createInfo->pVertexInputState = vertexInput_->getCreateInfoPtr();
	createInfo->pInputAssemblyState = inputAssembly_->getCreateInfoPtr();
	createInfo->pViewportState = viewport_->getCreateInfoPtr();
	createInfo->pRasterizationState = rasterization_->getCreateInfoPtr();
	createInfo->pMultisampleState = multisample_->getCreateInfoPtr();
	createInfo->pDepthStencilState = depthStencil_->getCreateInfoPtr();
	createInfo->pColorBlendState = colorBlend_->getCreateInfoPtr();
	createInfo->pDynamicState = dynamic_->getCreateInfoPtr();
}

