//
// Created by rmyho on 5/19/2020.
//

#include "instance.h"

#include <vector>
#include <cstring>

#ifndef NDEBUG

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
		void* userData)
{
	const char validation[] = "VALIDATION";
	const char performance[] = "Performance";
	const char error[] = "ERROR";
	const char warning[] = "WARNING";
	const char unknownType[] = "UNKNOWN_TYPE";
	const char unknownSeverity[] = "UNKNOWN_SEVERITY";
	const char* typeString = unknownType;
	const char* severityString = unknownSeverity;
	const char* messageIdName = callbackData->pMessageIdName;
	int32_t messageIdNumber = callbackData->messageIdNumber;
	const char* message = callbackData->pMessage;

	if (messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		return VK_FALSE;
	}

	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{
		severityString = error;
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		severityString = warning;
	}
	if (messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
	{
		typeString = validation;
	}
	else if (messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
	{
		typeString = performance;
	}

	printf("\n%s %s:\n\t[%s] Code %i : %s\n\n",
	       typeString,
	       severityString,
	       messageIdName,
	       messageIdNumber,
	       message);

	// Returning false tells the layer not to stop when the event occurs, so
	// they see the same behavior with and without validation layers enabled.
	return VK_FALSE;
}


VkResult vkCreateDebugUtilsMessengerEXT_PROXY(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                              const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{

	PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
	                                                                                                     "vkCreateDebugUtilsMessengerEXT");

	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}

	return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void vkDestroyDebugUtilsMessengerEXT_PROXY(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks* pAllocator)
{
	PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
	                                                                                                       "vkDestroyDebugUtilsMessengerEXT");

	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

const std::vector<const char*> VALIDATION_LAYERS = {
		"VK_LAYER_KHRONOS_validation"
};

#endif // !NDEBUG


Instance::Instance(const VkApplicationInfo &applicationInfo, const WindowManager* pWindowManager) :
		handle_(VK_NULL_HANDLE),
		pWindowManager_(pWindowManager),

		surface_(VK_NULL_HANDLE)
{
#ifndef NDEBUG

	uint32_t layerCount;
	CALL_VK(vkEnumerateInstanceLayerProperties(&layerCount, nullptr))
	std::vector<VkLayerProperties> avalibleLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, avalibleLayers.data());

	for (const char* layerName : VALIDATION_LAYERS)
	{
		bool isLayerFound = false;
		for (VkLayerProperties &layerProperty : avalibleLayers)
		{
			if (strcmp(layerName, layerProperty.layerName) == 0)
			{
				isLayerFound = true;
				break;
			}
		}

		if (!isLayerFound)
		{
			throw std::runtime_error("Unable to find validation layer.");
		}
	}

	VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = {};
	debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugMessengerCreateInfo.messageSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugMessengerCreateInfo.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debugMessengerCreateInfo.pfnUserCallback = debugCallback;
	debugMessengerCreateInfo.pUserData = nullptr;

#endif // !NDEBUG


	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;

	std::vector<const char*> requiredExtensions = pWindowManager_->getInstanceExtensions();

#ifndef NDEBUG
	requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
	instanceCreateInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();

	instanceCreateInfo.pNext = &debugMessengerCreateInfo;
#else
	instanceCreateInfo.enabledLayerCount = 0;
	instanceCreateInfo.ppEnabledLayerNames = nullptr;
#endif // !NDEBUG

	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

	CALL_VK(vkCreateInstance(&instanceCreateInfo, nullptr, &handle_))

#ifndef NDEBUG
	CALL_VK(vkCreateDebugUtilsMessengerEXT_PROXY(handle_, &debugMessengerCreateInfo, nullptr, &debugMessenger_))
#endif // !NDEBUG

	surface_ = pWindowManager_->createSurfaceHandle(handle_);
}

Instance::~Instance()
{
	CHECK_NULL_HANDLE(handle_)
}

void Instance::destroy()
{
#ifndef NDEBUG
	vkDestroyDebugUtilsMessengerEXT_PROXY(handle_, debugMessenger_, nullptr);
#endif // !NDEBUG

	vkDestroyInstance(handle_, nullptr);
	handle_ = VK_NULL_HANDLE;
}


VkSurfaceKHR Instance::getSurfaceHandle() const
{
	return surface_;
}

std::vector<PhysicalDevice> Instance::getPhysicalDevices()
{
	std::vector<PhysicalDevice> physicalDevices = std::vector<PhysicalDevice>();

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(handle_, &deviceCount, nullptr);

	std::vector<VkPhysicalDevice> physicalDeviceHandles(deviceCount);
	vkEnumeratePhysicalDevices(handle_, &deviceCount, physicalDeviceHandles.data());

	physicalDevices.reserve(deviceCount);
	for (VkPhysicalDevice physicalDeviceHandle : physicalDeviceHandles)
		physicalDevices.emplace_back(physicalDeviceHandle);

	return physicalDevices;
}
