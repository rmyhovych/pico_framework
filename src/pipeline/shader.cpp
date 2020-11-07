#include "shader.h"

#include "resources/resource_reader.h"


Shader::Shader(const Device &device, const char* filepath) :
		module_(VK_NULL_HANDLE)
{
	std::vector<char> shaderData = ResourceReader::readData(filepath);

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = shaderData.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderData.data());

	CALL_VK(vkCreateShaderModule(device.handle_, &createInfo, nullptr, &module_))
}

Shader::Shader(Shader &&s) noexcept:
		module_(s.module_)
{
	s.module_ = VK_NULL_HANDLE;
}

Shader::~Shader()
{
	CHECK_NULL_HANDLE(module_)
}

void Shader::destroy(const Device &device)
{
	vkDestroyShaderModule(device.handle_, module_, nullptr);
	module_ = VK_NULL_HANDLE;
}
