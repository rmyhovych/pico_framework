//
// Created by ross on 2020-11-06.
//

#include "frame_manager.h"

FrameManager::FrameManager(const Device& device, uint32_t nImages) :
		deviceHandle_(device.handle_),

		imageIndex_(0),

		fencesWait_(nImages, VK_NULL_HANDLE),

		semaphorePresentComplete_(VK_NULL_HANDLE),
		semaphoreRenderFinished_(VK_NULL_HANDLE)
{
	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (uint32_t i = 0; i < nImages; ++i)
	{
		CALL_VK(vkCreateFence(deviceHandle_, &fenceCreateInfo, nullptr, &fencesWait_[i]))
	}


	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	CALL_VK(vkCreateSemaphore(deviceHandle_, &semaphoreCreateInfo, nullptr, &semaphorePresentComplete_))
	CALL_VK(vkCreateSemaphore(deviceHandle_, &semaphoreCreateInfo, nullptr, &semaphoreRenderFinished_))
}

FrameManager::~FrameManager()
{
	if (!fencesWait_.empty())
		printf("FrameManager was not explicitly destroyed!");
}

void FrameManager::destroy()
{
	for (auto& fence : fencesWait_)
	{
		vkDestroyFence(deviceHandle_, fence, nullptr);
	}

	fencesWait_.clear();

	vkDestroySemaphore(deviceHandle_, semaphorePresentComplete_, nullptr);
	semaphorePresentComplete_ = VK_NULL_HANDLE;

	vkDestroySemaphore(deviceHandle_, semaphoreRenderFinished_, nullptr);
	semaphoreRenderFinished_ = VK_NULL_HANDLE;
}

uint32_t FrameManager::waitForImage(const Swapchain& swapchain)
{
	CALL_VK(vkAcquireNextImageKHR(deviceHandle_, swapchain.handle_, UINT64_MAX, semaphorePresentComplete_, VK_NULL_HANDLE, &imageIndex_))
	return imageIndex_;
}

void FrameManager::submit(VkQueue graphicsQueue, const std::vector<VkCommandBuffer>& commandBuffers)
{
	CALL_VK(vkWaitForFences(deviceHandle_, 1, &fencesWait_[imageIndex_], VK_TRUE, UINT64_MAX));
	CALL_VK(vkResetFences(deviceHandle_, 1, &fencesWait_[imageIndex_]));

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	VkPipelineStageFlags waitFlag = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &semaphorePresentComplete_;
	submitInfo.pWaitDstStageMask = &waitFlag;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex_];

	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &semaphoreRenderFinished_;

	CALL_VK(vkQueueSubmit(graphicsQueue, 1, &submitInfo, fencesWait_[imageIndex_]))
}

void FrameManager::present(VkQueue presentQueue, const Swapchain& swapchain)
{
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &semaphoreRenderFinished_;

	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchain.handle_;
	presentInfo.pImageIndices = &imageIndex_;

	VkResult presentResult = vkQueuePresentKHR(presentQueue, &presentInfo);
	if (presentResult != VK_SUCCESS && presentResult != VK_SUBOPTIMAL_KHR)
	{
		CALL_VK(presentResult)
	}
}
