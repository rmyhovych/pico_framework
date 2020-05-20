//
// Created by rmyho on 5/19/2020.
//

#ifndef PICOFRAMEWORK_INSTANCE_H
#define PICOFRAMEWORK_INSTANCE_H

#include <cstdint>
#include <vulkan/vulkan.h>

#include "surface.h"
#include "device.h"

class Instance
{
public:
	Instance();

	Instance(uint32_t width, uint32_t height);

private:
	VkInstance instance_;

	Surface surface_;
	Device device_;
};


#endif //PICOFRAMEWORK_INSTANCE_H
