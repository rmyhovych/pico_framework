//
// Created by rmyho on 5/20/2020.
//

#ifndef PICOFRAMEWORK_WINDOW_MANAGER_H
#define PICOFRAMEWORK_WINDOW_MANAGER_H

#include <pfvk.h>
#include <functional>
#include <vector>

class WindowManager
{
public:
	WindowManager();

	virtual VkSurfaceKHR createSurfaceHandle(VkInstance instance) = 0;

	virtual std::vector<const char*> getRequiredInstanceExtensions() const = 0;

	virtual VkExtent2D getWindowSize() = 0;

	void setResizeCallback(std::function<void(VkExtent2D)> resizeCallback);

	void resize(VkExtent2D newSize) const;

private:
	std::function<void(VkExtent2D)> resizeCallback_;
};


#endif //PICOFRAMEWORK_WINDOW_MANAGER_H
