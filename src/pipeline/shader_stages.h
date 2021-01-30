#ifndef SGADERSTAGES_H
#define SGADERSTAGES_H

#include "pfvk.h"


class ShaderStages
{
public:
	explicit ShaderStages(VkDevice hDevice);

	~ShaderStages();

	ShaderStages& addModule(const char* path, VkShaderStageFlagBits stage);

	uint32_t size() const;

	const VkPipelineShaderStageCreateInfo* data() const;

private:
	VkShaderModule createModule(const char* path) const;

private:
	VkDevice hDevice_;

	std::vector<VkPipelineShaderStageCreateInfo> stageInfos_;
};

#endif // SGADERSTAGES_H
