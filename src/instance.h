//
// Created by rmyho on 5/19/2020.
//

#ifndef PICOFRAMEWORK_INSTANCE_H
#define PICOFRAMEWORK_INSTANCE_H

#include <cstdint>
#include <pfvk.h>

#include "surface.h"
#include "device.h"
#include "window/window_manager.h"
#include "renderer.h"

class Instance
{
public:
	Instance(const VkApplicationInfo &applicationInfo, const WindowManager* pWindowManager);

	~Instance();

	void destroy();

	VkSurfaceKHR getSurfaceHandle() const;

	std::vector<PhysicalDevice> getPhysicalDevices();

private:
	VkInstance handle_;
	const WindowManager* pWindowManager_;

	VkSurfaceKHR surface_;
};


#endif //PICOFRAMEWORK_INSTANCE_H
