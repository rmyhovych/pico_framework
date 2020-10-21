#ifndef PIPELINE_STATE_H
#define PIPELINE_STATE_H

#include "pfvk.h"

template <class T>
class PipelineState {
public:
	const T* getCreateInfoPtr() const {
		return &createInfo_;
	}

protected:
	T createInfo_;
};

#endif // PIPELINE_STATE_H
