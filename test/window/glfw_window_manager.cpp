//
// Created by rmyho on 5/20/2020.
//

#include "glfw_window_manager.h"


void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	WindowManager* windowManager = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	windowManager->resize(VkExtent2D{static_cast<uint32_t>(width), static_cast<uint32_t>(height)});
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


bool GLFWWindowManager::isOpen() const
{
	return glfwWindowShouldClose(pWindow_) == 0;
}


std::vector<const char*> GLFWWindowManager::getInstanceExtensions() const
{
	uint32_t nExtensions = 0;
	const char** extensions = glfwGetRequiredInstanceExtensions(&nExtensions);

	return std::vector<const char*>(extensions, extensions + nExtensions);
}


VkExtent2D GLFWWindowManager::getExtent() const
{
	int width, height;
	glfwGetFramebufferSize(pWindow_, &width, &height);

	return VkExtent2D{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
}

Surface GLFWWindowManager::createSurface(VkInstance hInstance, const SurfaceProperties &surfaceProperties) const
{
	VkSurfaceKHR surfaceHandle;
	CALL_VK(glfwCreateWindowSurface(hInstance, pWindow_, nullptr, &surfaceHandle))

	return Surface(surfaceHandle, hInstance, surfaceProperties);
}
