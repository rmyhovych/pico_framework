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
		windowPtr_(nullptr)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	windowPtr_ = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(windowPtr_, this);
	glfwSetFramebufferSizeCallback(windowPtr_, framebufferResizeCallback);

	if (windowPtr_ == nullptr)
	{
		glfwTerminate();
		throw std::exception("Failed to create GLFW window!");
	}
}

GLFWWindowManager::~GLFWWindowManager()
{
	glfwDestroyWindow(windowPtr_);
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
	glfwGetFramebufferSize(windowPtr_, &width, &height);

	return VkExtent2D{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
}

VkSurfaceKHR GLFWWindowManager::createSurfaceHandle(VkInstance instance)
{
	VkSurfaceKHR surfaceHandle;
	if (glfwCreateWindowSurface(instance, windowPtr_, nullptr, &surfaceHandle) != VK_SUCCESS)
	{
		throw std::exception("Failed to create window surface!");
	}

	return surfaceHandle;
}
