#include <resources/asset_reader.h>
#include "shader_stages.h"

ShaderStages::ShaderStages(VkDevice hDevice) :
		hDevice_(hDevice),
		stageInfos_()
{
}

ShaderStages::~ShaderStages()
{
	for (const VkPipelineShaderStageCreateInfo& stageInfo : stageInfos_)
		vkDestroyShaderModule(hDevice_, stageInfo.module, nullptr);
	stageInfos_.clear();
}

ShaderStages& ShaderStages::addModule(const char* path, VkShaderStageFlagBits stage)
{
	stageInfos_.push_back({});
	VkPipelineShaderStageCreateInfo& createInfo = stageInfos_.back();
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	createInfo.module = createModule(path);
	createInfo.stage = stage;
	createInfo.pName = "main";

	return *this;
}

uint32_t ShaderStages::size() const
{
	return static_cast<uint32_t>(stageInfos_.size());
}

const VkPipelineShaderStageCreateInfo* ShaderStages::data() const
{
	return stageInfos_.data();
}

VkShaderModule ShaderStages::createModule(const char* path) const
{
	std::vector<char> shaderData = AssetReader::readData(path);

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = shaderData.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderData.data());

	VkShaderModule module;
	CALL_VK(vkCreateShaderModule(hDevice_, &createInfo, nullptr, &module))

	return module;
}
