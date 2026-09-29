#pragma once

#include "core/workflow/SageWorkflowRunRequest.h"
#include "core/workflow/SageWorkflowRunResult.h"

#include <QJsonObject>
#include <QString>

class SageWorkflowRegistry;

class SageWorkflowRunner
{
public:
    static SageWorkflowRunResult run(const SageWorkflowRegistry& registry, const SageWorkflowRunRequest& request);
    static QJsonObject buildPayload(const QString& inputPath, const QString& outputFolder, const QString& rowNums);
};
