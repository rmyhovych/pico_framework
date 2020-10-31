#include "shader_stages.h"

ShaderStages::ShaderStages() :
	shaders_(),
	stageInfos_()
{

}

ShaderStages::~ShaderStages()
{
	if (!shaders_.empty())
		printf("WARNING: ShaderStages was not explicitly destroyed!\n");
}

void ShaderStages::destroy(const Device &device)
{
	for (Shader& shader : shaders_)
		shader.destroy(device);

	shaders_.clear();
	stageInfos_.clear();
}

ShaderStages &ShaderStages::addModule(const Device &device, const char *path, VkShaderStageFlagBits stage)
{
	shaders_.push_back(Shader(device, path));

	stageInfos_.push_back({});
	VkPipelineShaderStageCreateInfo& createInfo = stageInfos_.back();
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	createInfo.module = shaders_.back().module_;
	createInfo.stage = stage;
	createInfo.pName = "main";

	return *this;
}

uint32_t ShaderStages::size() const
{
	return static_cast<uint32_t>(stageInfos_.size());
}

const VkPipelineShaderStageCreateInfo *ShaderStages::data() const
{
	return stageInfos_.data();
}
