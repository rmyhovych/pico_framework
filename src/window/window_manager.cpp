//
// Created by rmyho on 5/21/2020.
//

#include "window_manager.h"

#include <utility>

void defaultResizeCallback(VkExtent2D)
{
}


WindowManager::WindowManager() :
		resizeCallback_(defaultResizeCallback)
{
}

void WindowManager::setResizeCallback(std::function<void(VkExtent2D)> &resizeCallback)
{
	resizeCallback_ = std::move(resizeCallback);
}

void WindowManager::resize(VkExtent2D newExtent)
{
	resizeCallback_(newExtent);
}

