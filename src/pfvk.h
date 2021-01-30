#ifndef PFVK_H
#define PFVK_H

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <string>

#include <memory>
#include <functional>

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

#ifndef NDEBUG

#include <vector>

const std::vector<const char*> VALIDATION_LAYERS({"VK_LAYER_KHRONOS_validation"});

#endif // !NDEBUG


#define NO_COPY_MOVE_ONLY(Type)                     \
    Type(Type&&) noexcept = default;                \
    Type(const Type&) noexcept = delete;            \
    Type operator=(Type) noexcept = delete;         \
    Type operator=(const Type&) noexcept = delete;  \
    Type& operator=(Type&&) noexcept = default;


///////////////// UNIQUE FUNCTION /////////////////
template<typename T>
class UniqueFunction;

template<typename R, typename... AA>
class UniqueFunction<R(AA...)> : private std::unique_ptr<std::function<R(AA...)>>
{
public:
	NO_COPY_MOVE_ONLY(UniqueFunction);

	explicit UniqueFunction(std::function<R(AA...)>&& func) :
			std::unique_ptr<std::function<R(AA...)>>(std::make_unique<std::function<R(AA...)>>(func))
	{
	}

	R operator()(AA... args) const
	{
		return (*this)(args...);
	}
};
///////////////////////////////////////////////////


#endif // PFVK_H
