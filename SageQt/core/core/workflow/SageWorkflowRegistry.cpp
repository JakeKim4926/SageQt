#include "core/workflow/SageWorkflowRegistry.h"

#include "core/workflow/handlers/SageSampleWorkflowHandler.h"

#include <utility>

SageWorkflowRegistry::SageWorkflowRegistry()
{
    registerHandler(std::make_unique<SageSampleWorkflowHandler>());
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

QList<const ISageWorkflowHandler*> SageWorkflowRegistry::handlers() const
{
    QList<const ISageWorkflowHandler*> result;
    result.reserve(static_cast<qsizetype>(m_handlers.size()));
    for (const std::unique_ptr<ISageWorkflowHandler>& handler : m_handlers) {
        result.append(handler.get());
    }
    return result;
}

void SageWorkflowRegistry::registerHandler(std::unique_ptr<ISageWorkflowHandler> handler)
{
    m_handlers.push_back(std::move(handler));
}
