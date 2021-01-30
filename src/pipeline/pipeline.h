#ifndef PIPELINE_H
#define PIPELINE_H

#include <optional>

#include "shader_stages.h"

#include "state/state_manager.h"

#include "renderpass/render_pass.h"

#include "layout/layout.h"

class Pipeline
{
public:
	class Builder
	{
	public:
		explicit Builder(VkDevice hDevice);

		Builder& linkShaders(const ShaderStages* shaders);

		Builder& linkStates(const StateManager* stateManager);

		Builder& linkLayout(const Layout* layout);

		Builder& linkRenderPass(const RenderPass* renderPass);

		Pipeline build();

	private:
		VkDevice hDevice_;

		VkGraphicsPipelineCreateInfo createInfo_;
	};

public:
	~Pipeline();

	Pipeline(Pipeline&& p) noexcept;

private:
	Pipeline(VkDevice hDevice, VkPipeline handle, VkPipelineLayout layout);

public:
	VkDevice hDevice_;

	VkPipeline handle_;
	VkPipelineLayout layout_;
};

#endif // PIPELINE_H
