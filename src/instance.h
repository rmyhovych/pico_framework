//
// Created by rmyho on 5/19/2020.
//

#ifndef PICOFRAMEWORK_INSTANCE_H
#define PICOFRAMEWORK_INSTANCE_H

#include <cstdint>
#include <pfvk.h>

#include "surface.h"
#include "device.h"
#include "window/abs_window_manager.h"
#include "renderer.h"

class Instance
{
public:
	Instance(AbsWindowManager* pWindowManager, const Surface::Properties& surfaceProperties, const Device::Properties& deviceProperties);

	~Instance();

	Renderer* getRenderer();

	Device* getDevice();

private:
	static VkInstance createInstanceHandle(AbsWindowManager* pWindowManager);

private:
	VkInstance handle_;
	AbsWindowManager* pWindowManager_;

	Surface surface_;
	Device device_;

	Renderer* renderer_;
};


#endif //PICOFRAMEWORK_INSTANCE_H
