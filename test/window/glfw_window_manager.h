//
// Created by rmyho on 5/20/2020.
//

#ifndef PICOFRAMEWORK_GLFW_WINDOW_MANAGER_H
#define PICOFRAMEWORK_GLFW_WINDOW_MANAGER_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include "window/window_manager.h"

class GLFWWindowManager : public WindowManager
{
public:
	GLFWWindowManager(uint32_t width, uint32_t height);

	~GLFWWindowManager();

	bool isOpen() const;

	std::vector<const char*> getInstanceExtensions() const override;

	VkExtent2D getExtent() const override;

	Surface createSurface(VkInstance hInstance, VkFormat format, VkColorSpaceKHR colorSpace, VkPresentModeKHR presentMode) const override;

private:
	GLFWwindow* pWindow_;
};


#endif //PICOFRAMEWORK_GLFW_WINDOW_MANAGER_H
