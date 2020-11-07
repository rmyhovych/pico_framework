#ifndef PFVK_H
#define PFVK_H

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <string>

#define CALL_VK(result)                                         \
    if (VK_SUCCESS != (result))                                 \
    {                                                           \
        throw std::runtime_error(std::string("Vulkan Error[")   \
            .append(std::to_string(result))                     \
            .append(std::string("]. File["))                    \
            .append(std::string(__FILE__))                      \
            .append(std::string("], line["))                    \
            .append(std::to_string(__LINE__))                   \
            .append(std::string("]. Error["))                   \
            .append(std::to_string(result))                     \
            .append(std::string("].")));                        \
    }


#define CHECK_NULL_HANDLE(handle)                                            \
    if (handle != VK_NULL_HANDLE)                                            \
        printf(                                                              \
            "WARNING : [%s:%s] handle was not explicitly destroyed.\n",      \
            std::string(__FILE__).c_str(),                                   \
            std::to_string(__LINE__).c_str()                                 \
        );

#endif // PFVK_H
