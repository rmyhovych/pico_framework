//
// Created by ross on 2021-01-27.
//

#ifndef PICOFRAMEWORK_BINDING_RESOURCE_H
#define PICOFRAMEWORK_BINDING_RESOURCE_H

#include "pfvk.h"

#include "resources/resource_factory.h"

class BindingResource
{
public:
	virtual void update(const ResourceFactory& resourceFactory, uint32_t frameIndex) = 0;
};

#endif //PICOFRAMEWORK_BINDING_RESOURCE_H
