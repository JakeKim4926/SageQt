#pragma once

#include "SageDefine.h"

#include <QString>

struct SageWorkflowTab
{
    SageWorkflowTabKind m_kind = SageWorkflowTabKind::Input;
    QString m_label;
};
