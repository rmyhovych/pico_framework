//
// Created by ross on 2020-11-10.
//

#ifndef PICOFRAMEWORK_DATA_TRANSFER_MANAGER_H
#define PICOFRAMEWORK_DATA_TRANSFER_MANAGER_H

#include "pfvk.h"

#include "device/device_queue.h"

class DataTransferManager
{
public:
	DataTransferManager(VkDevice hDevice, const DeviceQueue* pDeviceQueue);

	~DataTransferManager();

	DataTransferManager &begin();

	void submit();

	DataTransferManager &transfer(VkBuffer src, VkBuffer dst, VkDeviceSize size);

private:
	VkDevice hDevice_;
	VkQueue hQueue_;

	VkCommandPool transferCmdPool_;
	VkCommandBuffer transferCmdBuffer_;
};


#endif //PICOFRAMEWORK_DATA_TRANSFER_MANAGER_H
