//
// Created by rmyho on 5/20/2020.
//

#ifndef PICOFRAMEWORK_GLFW_WINDOW_MANAGER_H
#define PICOFRAMEWORK_GLFW_WINDOW_MANAGER_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include "window_manager.h"

class GLFWWindowManager : public WindowManager
{
public:
	GLFWWindowManager(uint32_t width, uint32_t height);

	~GLFWWindowManager();

	std::vector<const char*> getRequiredInstanceExtensions() const override;

	VkExtent2D getExtent() const override;

	VkSurfaceKHR createSurfaceHandle(VkInstance instance) const override;

private:
	GLFWwindow* pWindow_;
};


#endif //PICOFRAMEWORK_GLFW_WINDOW_MANAGER_H
