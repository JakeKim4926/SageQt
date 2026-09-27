#pragma once

#include "SageDefine.h"
#include "core/workflow/ISageWorkflowHandler.h"

#include <memory>
#include <vector>

class SageWorkflowRegistry
{
public:
    SageWorkflowRegistry();

    const ISageWorkflowHandler* findHandler(SageWorkflowType workflowType) const;

private:
    std::vector<std::unique_ptr<ISageWorkflowHandler>> m_handlers;
};
