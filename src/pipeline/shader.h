#ifndef SHADER_H
#define SHADER_H

#include "pfvk.h"
#include "device.h"


class Shader
{
public:
	Shader(const Device& device, const char* filepath);

public:
	VkShaderModule module_;
};

#endif // SHADER_H
