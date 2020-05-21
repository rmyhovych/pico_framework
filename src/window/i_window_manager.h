//
// Created by rmyho on 5/20/2020.
//

#ifndef PICOFRAMEWORK_I_WINDOW_MANAGER_H
#define PICOFRAMEWORK_I_WINDOW_MANAGER_H

#include <vulkan/vulkan.h>

class IWindowManager
{
public:
	virtual VkSurfaceKHR createSurfaceHandle(VkInstance instance) = 0;

	virtual VkExtent2D getWindowSize() = 0;
};


#endif //PICOFRAMEWORK_I_WINDOW_MANAGER_H
