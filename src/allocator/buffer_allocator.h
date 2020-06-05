//
// Created by rmyho on 5/31/2020.
//

#ifndef PICOFRAMEWORK_BUFFER_ALLOCATOR_H
#define PICOFRAMEWORK_BUFFER_ALLOCATOR_H


#include "abs_allocator.h"

class BufferAllocator : public Allocator<VkImage>
{
public:
	BufferAllocator(VkDevice logicalDevice, VkPhysicalDevice physicalDevice);

	void allocate(const VkImage &key) override;

	void free(const VkImage &key) override;

private:
	int hash(const VkImage &key) override;
};

#endif //PICOFRAMEWORK_BUFFER_ALLOCATOR_H
