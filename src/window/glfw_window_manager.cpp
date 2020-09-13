//
// Created by rmyho on 5/20/2020.
//

#include "glfw_window_manager.h"


void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	AbsWindowManager* windowManager = reinterpret_cast<AbsWindowManager*>(glfwGetWindowUserPointer(window));
	windowManager->callResizeCallback(VkExtent2D{static_cast<uint32_t>(width), static_cast<uint32_t>(height)});
}


GLFWWindowManager::GLFWWindowManager(uint32_t width, uint32_t height) :
		pWindow_(nullptr)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	pWindow_ = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(pWindow_, this);
	glfwSetFramebufferSizeCallback(pWindow_, framebufferResizeCallback);

	if (pWindow_ == nullptr)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window!");
	}
}

GLFWWindowManager::~GLFWWindowManager()
{
	glfwDestroyWindow(pWindow_);
	glfwTerminate();
}

std::vector<const char*> GLFWWindowManager::getRequiredInstanceExtensions() const
{
	uint32_t nExtensions = 0;
	const char** extensions = glfwGetRequiredInstanceExtensions(&nExtensions);

	return std::vector<const char*>(extensions, extensions + nExtensions);
}


VkExtent2D GLFWWindowManager::getWindowSize()
{
	int width, height;
	glfwGetFramebufferSize(pWindow_, &width, &height);

	return VkExtent2D{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
}

VkSurfaceKHR GLFWWindowManager::createSurfaceHandle(VkInstance instance)
{
	VkSurfaceKHR surfaceHandle;
	CALL_VK(glfwCreateWindowSurface(instance, pWindow_, nullptr, &surfaceHandle))

	return surfaceHandle;
}
