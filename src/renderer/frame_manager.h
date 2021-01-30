//
// Created by ross on 2020-11-06.
//

#ifndef PICOFRAMEWORK_FRAME_MANAGER_H
#define PICOFRAMEWORK_FRAME_MANAGER_H

#include "swapchain/swapchain.h"
#include "pfvk.h"
#include "device/device.h"

class FrameManager
{
public:
	FrameManager(const Device& device, uint32_t nImages);

	~FrameManager();

	void destroy();

	uint32_t waitForImage(const Swapchain& swapchain);

	void submit(VkQueue graphicsQueue, const std::vector<VkCommandBuffer>& commandBuffers);

	void present(VkQueue presentQueue, const Swapchain& swapchain);

private:
	VkDevice deviceHandle_;

	uint32_t imageIndex_;

	std::vector<VkFence> fencesWait_;

	VkSemaphore semaphorePresentComplete_;
	VkSemaphore semaphoreRenderFinished_;
};


#endif //PICOFRAMEWORK_FRAME_MANAGER_H
