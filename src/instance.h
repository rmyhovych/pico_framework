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
	Instance(const VkApplicationInfo &applicationInfo, const std::vector<const char*> &requiredExtensions);

	~Instance();

	VkInstance getHandle() const;

	std::vector<PhysicalDevice> getPhysicalDevices();

private:
	VkInstance handle_;
};


#endif //PICOFRAMEWORK_INSTANCE_H
