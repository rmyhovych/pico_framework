#ifndef PFVK_H
#define PFVK_H

#include <vulkan/vulkan.h>
#include <exception>
#include <string>

#define CALL_VK(result)                                             \
	if (VK_SUCCESS != (result)) {                                   \
		throw std::exception(std::string("Vulkan error. File[")     \
			.append(std::string(__FILE__))                          \
			.append(std::string("], line[%d]"))                     \
			.append(std::to_string(__LINE__)).c_str());             \
	}


#endif // PFVK_H