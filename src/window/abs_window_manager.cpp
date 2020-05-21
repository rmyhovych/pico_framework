//
// Created by rmyho on 5/21/2020.
//

#include "abs_window_manager.h"

#include <utility>

void defaultResizeCallback(VkExtent2D)
{
}


AbsWindowManager::AbsWindowManager() :
		resizeCallback_(defaultResizeCallback)
{
}

void AbsWindowManager::setResizeCallback(std::function<void(VkExtent2D)> resizeCallback)
{
	resizeCallback_ = std::move(resizeCallback);
}

void AbsWindowManager::callResizeCallback(VkExtent2D newSize) const
{
	resizeCallback_(newSize);
}
