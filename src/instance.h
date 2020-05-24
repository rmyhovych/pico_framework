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
#include "renderer.h"

class Instance
{
public:
	Instance(AbsWindowManager* windowManagerPtr, const Surface::Properties& surfaceProperties, const Device::Properties& deviceProperties);

	~Instance();

	Renderer* getRenderer();

private:
	static VkInstance createInstanceHandle(AbsWindowManager* windowManagerPtr);

private:
	VkInstance handle_;
	AbsWindowManager* windowManagerPtr_;

	Surface surface_;
	Device device_;

	Renderer* renderer_;
};


#endif //PICOFRAMEWORK_INSTANCE_H
