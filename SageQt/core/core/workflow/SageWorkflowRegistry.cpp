#include "core/workflow/SageWorkflowRegistry.h"

#include "core/workflow/handlers/SageSampleWorkflowHandler.h"

SageWorkflowRegistry::SageWorkflowRegistry()
{
    m_handlers.push_back(std::make_unique<SageSampleWorkflowHandler>());
}

const ISageWorkflowHandler* SageWorkflowRegistry::findHandler(SageWorkflowType workflowType) const
{
    for (const std::unique_ptr<ISageWorkflowHandler>& handler : m_handlers) {
        if (handler->workflowType() == workflowType) {
            return handler.get();
        }
    }
    return nullptr;
}
