#include "pipeline.h"


Pipeline::Builder::Builder(VkDevice hDevice) :
		hDevice_(hDevice),
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

Pipeline::Builder& Pipeline::Builder::linkShaders(const ShaderStages* shaders)
{
	createInfo_.stageCount = shaders->size();
	createInfo_.pStages = shaders->data();

	return *this;
}

Pipeline::Builder& Pipeline::Builder::linkStates(const StateManager* stateManager)
{
	stateManager->applyOn(&createInfo_);
	return *this;
}

Pipeline::Builder& Pipeline::Builder::linkLayout(const Layout* layout)
{
	createInfo_.layout = layout->getPipelineLayout();
	return *this;
}

Pipeline::Builder& Pipeline::Builder::linkRenderPass(const RenderPass* renderPass)
{
	createInfo_.renderPass = renderPass->handle_;
	return *this;
}

Pipeline Pipeline::Builder::build()
{
	VkPipeline handle;
	CALL_VK(vkCreateGraphicsPipelines(hDevice_, VK_NULL_HANDLE, 1, &createInfo_, nullptr, &handle))
	return Pipeline(hDevice_, handle, VK_NULL_HANDLE);
}

/*------------------------------------------------------------------------------------------------------------------------*/

Pipeline::Pipeline(VkDevice hDevice, VkPipeline handle, VkPipelineLayout layout) :
		hDevice_(hDevice),
		handle_(handle),
		layout_(layout)
{
}

Pipeline::~Pipeline()
{
	vkDestroyPipeline(hDevice_, handle_, nullptr);
	handle_ = VK_NULL_HANDLE;
}

Pipeline::Pipeline(Pipeline&& p) noexcept:
		hDevice_(p.hDevice_),
		handle_(p.handle_),
		layout_(p.layout_)
{
	p.handle_ = VK_NULL_HANDLE;
	p.layout_ = VK_NULL_HANDLE;
}
