#pragma once

#include "SageDefine.h"
#include "core/workflow/ISageWorkflowHandler.h"

#include <QList>

#include <memory>
#include <vector>

class SageWorkflowRegistry
{
public:
    SageWorkflowRegistry();

    const ISageWorkflowHandler* findHandler(SageWorkflowType workflowType) const;
    QList<const ISageWorkflowHandler*> handlers() const;

    void registerHandler(std::unique_ptr<ISageWorkflowHandler> handler);

private:
    std::vector<std::unique_ptr<ISageWorkflowHandler>> m_handlers;
};
