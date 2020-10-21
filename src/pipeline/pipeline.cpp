#include "pipeline.h"


Pipeline::Builder::Builder():
	createInfo_({})
{
	createInfo_.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

	createInfo_.flags = 0;
	createInfo_.stageCount = 0;
	createInfo_.pStages = nullptr;
	createInfo_.pVertexInputState = nullptr;
	createInfo_.pInputAssemblyState = nullptr;
	createInfo_.pTessellationState = nullptr;
	createInfo_.pViewportState = nullptr;
	createInfo_.pRasterizationState = nullptr;
	createInfo_.pMultisampleState = nullptr;
	createInfo_.pDepthStencilState = nullptr;
	createInfo_.pColorBlendState = nullptr;
	createInfo_.pDynamicState = nullptr;

	createInfo_.layout = VK_NULL_HANDLE;
	createInfo_.renderPass = VK_NULL_HANDLE;

	// unused
	createInfo_.subpass = 0;
	createInfo_.basePipelineHandle = VK_NULL_HANDLE;
	createInfo_.basePipelineIndex = -1;
}

Pipeline::Builder &Pipeline::Builder::linkShaders(const ShaderStages* shaders)
{
	createInfo_.stageCount = shaders->size();
	createInfo_.pStages = shaders->data();

	return *this;
}

Pipeline::Builder &Pipeline::Builder::linkStates(const StateManager* stateManager)
{
	stateManager->applyOn(&createInfo_);
	return *this;
}

Pipeline::Builder &Pipeline::Builder::linkLayout(const PipelineLayout *layout)
{
	createInfo_.layout = layout->handle_;
	return *this;
}

Pipeline::Builder &Pipeline::Builder::linkRenderPass(const RenderPass *renderPass)
{
	createInfo_.renderPass = renderPass->handle_;
	return *this;
}

Pipeline Pipeline::Builder::build(const Device &device)
{
	VkPipeline handle;
	CALL_VK(vkCreateGraphicsPipelines(device.handle_, VK_NULL_HANDLE, 1, &createInfo_, nullptr, &handle))
	return Pipeline(handle);
}

/*------------------------------------------------------------------------------------------------------------------------*/

Pipeline::Pipeline(VkPipeline handle) :
	handle_(handle)
{
}

Pipeline::~Pipeline()
{
	CHECK_NULL_HANDLE(handle_)
}

void Pipeline::destroy(const Device &device)
{
	vkDestroyPipeline(device.handle_, handle_, nullptr);
}
