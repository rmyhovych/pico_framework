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

	std::shared_ptr<VertexInputState> setVertexInput(uint32_t stride, VkVertexInputRate inputRate=VK_VERTEX_INPUT_RATE_VERTEX);
	std::shared_ptr<InputAssemblyState> setInputAssembly(VkPrimitiveTopology topology);
	std::shared_ptr<ViewportState> setViewport(VkExtent2D extent);
	std::shared_ptr<RasterizationState> setRasterization(VkFrontFace frontFace=VK_FRONT_FACE_COUNTER_CLOCKWISE, VkCullModeFlags cullMode=VK_CULL_MODE_BACK_BIT);
	std::shared_ptr<MultisampleState> setMultisample();
	std::shared_ptr<DepthStencilState> setDepthStencil(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp=VK_COMPARE_OP_LESS);
	std::shared_ptr<ColorBlendState> setColorBlend();
	std::shared_ptr<DynamicState> setDynamic();

	void applyOn(VkGraphicsPipelineCreateInfo* createInfo) const;

private:
	template<typename T>
	const T* getCreateInfo(const PipelineState<T>* statePtr) const {
		return statePtr == nullptr ? nullptr : statePtr->getCreateInfoPtr();
	}

private:
	std::shared_ptr<VertexInputState> vertexInput_;
	std::shared_ptr<InputAssemblyState> inputAssembly_;
	std::shared_ptr<ViewportState> viewport_;
	std::shared_ptr<RasterizationState> rasterization_;
	std::shared_ptr<MultisampleState> multisample_;
	std::shared_ptr<DepthStencilState> depthStencil_;
	std::shared_ptr<ColorBlendState> colorBlend_;
	std::shared_ptr<DynamicState> dynamic_;
};

#endif // STATEMANAGER_H
