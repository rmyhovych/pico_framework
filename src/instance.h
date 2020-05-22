//
// Created by rmyho on 5/19/2020.
//

#ifndef PICOFRAMEWORK_INSTANCE_H
#define PICOFRAMEWORK_INSTANCE_H

#include <cstdint>
#include <vulkan/vulkan.h>

#include "surface.h"
#include "device.h"
#include "window/abs_window_manager.h"

class Instance
{
public:
	Instance(AbsWindowManager* windowManagerPtr, Surface::Properties surfaceProperties);

	~Instance();

private:
	static VkInstance createInstance(AbsWindowManager* windowManagerPtr);

private:
	VkInstance instance_;
	AbsWindowManager* windowManagerPtr_;

	Surface surface_;
	Device device_;
};


#endif //PICOFRAMEWORK_INSTANCE_H
