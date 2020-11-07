#ifndef SGADERSTAGES_H
#define SGADERSTAGES_H

#include "shader.h"


class ShaderStages
{
public:
	ShaderStages();

	~ShaderStages();

	void destroy(const Device &device);

	ShaderStages &addModule(const Device &device, const char* path, VkShaderStageFlagBits stage);

	uint32_t size() const;

	const VkPipelineShaderStageCreateInfo* data() const;

private:
	std::vector<Shader> shaders_;
	std::vector<VkPipelineShaderStageCreateInfo> stageInfos_;
};

#endif // SGADERSTAGES_H
