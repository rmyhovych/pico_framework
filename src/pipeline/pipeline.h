#ifndef PIPELINE_H
#define PIPELINE_H

#include "device.h"
#include "shader.h"

#include "state/vertex_input_state.h"
#include "state/input_assembly_state.h"
#include "state/viewport_state.h"
#include "state/rasterization_state.h"
#include "state/multisample_state.h"
#include "state/depth_stencil_state.h"
#include "state/color_blend_state.h"
#include "state/dynamic_state.h"

#include "pipeline_layout.h"
#include "renderpass/render_pass.h"


class Pipeline
{
public:
	class Builder {
	public:
		Builder();

		Builder& setShaders(const Shader& vertex, const Shader& fragment);
		Builder& setVertexInputState(const VertexInputState& state);
		Builder& setInputAssemblyState(const InputAssemblyState& state);
		Builder& setViewportState(const ViewportState& state);
		Builder& setRasterizationState(const RasterizationState& state);
		Builder& setMultisampleState(const MultisampleState& state);
		Builder& setDepthStencilState(const DepthStencilState& state);
		Builder& setColorBlendState(const ColorBlendState& state);
		Builder& setDynamicState(const DynamicState& state);

		Pipeline build(const Device& device);

	private:
		VkGraphicsPipelineCreateInfo createInfo_;

		std::array<VkShaderModule, 2> shaderModules_;
	};

public:

private:
	Pipeline(VkPipeline handle);
	~Pipeline();

	void destroy(const Device& device);

public:
	VkPipeline handle_;
};

#endif // PIPELINE_H
