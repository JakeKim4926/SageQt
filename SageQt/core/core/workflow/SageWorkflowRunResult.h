#pragma once

#include "SageDefine.h"

#include <QJsonObject>
#include <QString>

#include <optional>

struct SageWorkflowRunResult
{
    SageWorkflowType m_workflowType = SageWorkflowType::Sample;
    SageTaskType m_taskType = SageTaskType::Generate;
    QJsonObject m_response;
};

struct SageWorkflowResultState
{
    std::optional<SageWorkflowType> m_workflowType;
    std::optional<SageTaskType> m_taskType;
    bool m_success = false;
    QJsonObject m_response;
    QString m_inputPath;
};
