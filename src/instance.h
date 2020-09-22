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
	Instance(WindowManager* pWindowManager, const Surface::Properties &surfaceProperties, const Device::Properties &deviceProperties);

	~Instance();

	Renderer* getRenderer();

	Device* getDevice();

private:
	static VkInstance createInstanceHandle(WindowManager* pWindowManager);

private:
	VkInstance handle_;
	WindowManager* pWindowManager_;

	Surface surface_;
	Device device_;

	Renderer* renderer_;
};


#endif //PICOFRAMEWORK_INSTANCE_H
