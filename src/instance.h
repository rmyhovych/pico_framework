//
// Created by rmyho on 5/19/2020.
//

#ifndef PICOFRAMEWORK_INSTANCE_H
#define PICOFRAMEWORK_INSTANCE_H

#include <cstdint>
#include <pfvk.h>
#include "window/window_manager.h"

#include "device/physical_device.h"

class Instance
{
public:
	Instance(const VkApplicationInfo& applicationInfo, const std::vector<const char*>& requiredExtensions);

	~Instance();

	Surface createSurface(const WindowManager* pWindowManager, const SurfaceProperties& surfaceProperties) const;

	std::vector<PhysicalDevice> getPhysicalDevices();

private:
	VkInstance handle_;

#ifndef NDEBUG
	VkDebugUtilsMessengerEXT debugMessenger_ = VK_NULL_HANDLE;
#endif // !NDEBUG
};


#endif //PICOFRAMEWORK_INSTANCE_H
