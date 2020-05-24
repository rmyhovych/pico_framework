//
// Created by rmyho on 5/20/2020.
//

#ifndef PICOFRAMEWORK_GLFW_WINDOW_MANAGER_H
#define PICOFRAMEWORK_GLFW_WINDOW_MANAGER_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include "abs_window_manager.h"

class GLFWWindowManager : public AbsWindowManager
{
public:
	GLFWWindowManager(uint32_t width, uint32_t height);
	~GLFWWindowManager();

	std::vector<const char*> getRequiredInstanceExtensions() const override;

	VkExtent2D getWindowSize() override;

	VkSurfaceKHR createSurfaceHandle(VkInstance instance) override;

private:
	GLFWwindow* pWindow_;
};


#endif //PICOFRAMEWORK_GLFW_WINDOW_MANAGER_H
