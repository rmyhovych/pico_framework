#ifndef PIPELINE_H
#define PIPELINE_H

#include <optional>

#include "device.h"
#include "shader_stages.h"

#include "state/state_manager.h"

#include "pipeline_layout.h"
#include "renderpass/render_pass.h"


class Pipeline
{
public:
	class Builder {
	public:
		Builder();

		Builder& linkShaders(const ShaderStages* shaders);
		Builder& linkStates(const StateManager* stateManager);
		Builder& linkLayout(const PipelineLayout* layout);
		Builder& linkRenderPass(const RenderPass* renderPass);

		Pipeline build(const Device& device);

	private:
		VkGraphicsPipelineCreateInfo createInfo_;
	};

public:
	~Pipeline();

	Pipeline(Pipeline&& p);

	void destroy(const Device& device);

private:
	Pipeline(VkPipeline handle);

public:
	VkPipeline handle_;
};

#endif // PIPELINE_H
