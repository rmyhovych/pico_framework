#ifndef MULTISAMPLESTATE_H
#define MULTISAMPLESTATE_H

#include "pipeline_state.h"


/**
 * @brief Multisampling not supported yet, fixed parameters
 */
class MultisampleState : public PipelineState<VkPipelineMultisampleStateCreateInfo>
{
public:
	MultisampleState();
};

#endif // MULTISAMPLESTATE_H
