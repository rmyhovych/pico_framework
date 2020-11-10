//
// Created by rmyho on 5/20/2020.
//

#ifndef PICOFRAMEWORK_WINDOW_MANAGER_H
#define PICOFRAMEWORK_WINDOW_MANAGER_H

#include <functional>
#include <vector>

#include "surface.h"

class WindowManager
{
public:
	WindowManager();

	virtual std::vector<const char*> getInstanceExtensions() const = 0;

	virtual VkExtent2D getExtent() const = 0;

	virtual Surface createSurface(VkInstance hInstance, VkFormat format, VkColorSpaceKHR colorSpace, VkPresentModeKHR presentMode) const = 0;

	void setResizeCallback(std::function<void(VkExtent2D)> &resizeCallback);

	void resize(VkExtent2D newExtent);

private:
	std::function<void(VkExtent2D)> resizeCallback_;
};


#endif //PICOFRAMEWORK_WINDOW_MANAGER_H
