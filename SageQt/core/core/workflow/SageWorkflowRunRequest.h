#pragma once

#include "SageDefine.h"

#include <QString>

struct SageWorkflowRunRequest
{
    SageWorkflowType m_workflowType = SageWorkflowType::Sample;
    SageTaskType m_taskType = SageTaskType::Generate;
    QString m_inputPath;
    QString m_outputFolder;
    QString m_selectedRowNums;
};
