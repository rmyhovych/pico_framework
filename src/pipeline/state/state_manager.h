#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "vertex_input_state.h"
#include "input_assembly_state.h"
#include "viewport_state.h"
#include "rasterization_state.h"
#include "multisample_state.h"
#include "depth_stencil_state.h"
#include "color_blend_state.h"
#include "dynamic_state.h"

#include <memory>


class StateManager
{
public:
	StateManager();

	~StateManager();

	VertexInputState* setVertexInput(uint32_t stride, VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX);

	InputAssemblyState* setInputAssembly(VkPrimitiveTopology topology);

	ViewportState* setViewport(VkExtent2D extent);

	RasterizationState* setRasterization(VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE, VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT);

	MultisampleState* setMultisample();

	DepthStencilState* setDepthStencil(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp = VK_COMPARE_OP_LESS);

	ColorBlendState* setColorBlend();

	DynamicState* setDynamic();

	void applyOn(VkGraphicsPipelineCreateInfo* createInfo) const;

private:
	VertexInputState* vertexInput_;
	InputAssemblyState* inputAssembly_;
	ViewportState* viewport_;
	RasterizationState* rasterization_;
	MultisampleState* multisample_;
	DepthStencilState* depthStencil_;
	ColorBlendState* colorBlend_;
	DynamicState* dynamic_;
};

#endif // STATEMANAGER_H
