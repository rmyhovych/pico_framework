#ifndef SHADER_H
#define SHADER_H

#include "pfvk.h"
#include "device.h"


class Shader
{
public:
	Shader(const Device &device, const char* filepath);

	Shader(Shader &&s) noexcept;

	~Shader();

	void destroy(const Device &device);

public:
	VkShaderModule module_;
};

#endif // SHADER_H
