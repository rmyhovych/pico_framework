//
// Created by ross on 2020-11-10.
//

#ifndef PICOFRAMEWORK_DEVICE_QUEUE_H
#define PICOFRAMEWORK_DEVICE_QUEUE_H

#include "pfvk.h"

struct DeviceQueue
{
	VkQueue queue = VK_NULL_HANDLE;
	uint32_t family = 0;
	VkQueueFlags type = 0;

	bool isType(VkQueueFlags subtype) const
	{
		return (type & subtype) == subtype;
	}
};


#endif //PICOFRAMEWORK_DEVICE_QUEUE_H
