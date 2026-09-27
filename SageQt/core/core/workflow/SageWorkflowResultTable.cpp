#include "core/workflow/SageWorkflowResultTable.h"

#include "SageDefine.h"

QList<SageWorkflowColumn> SageWorkflowResultTable::genericColumns()
{
    return {
        {SAGE_UI_RESULT_FIELD, SageColumnAlign::Center, false, SageResultField::Field},
        {SAGE_UI_RESULT_VALUE, SageColumnAlign::Center, true, SageResultField::Value},
        {SAGE_UI_RESULT_STATUS, SageColumnAlign::Center, false, SageResultField::Status},
        {SAGE_UI_RESULT_REASON, SageColumnAlign::Center, false, SageResultField::Reason},
    };
}

QString SageWorkflowResultTable::rowText(const SageResultRow& row, SageResultField field)
{
    switch (field) {
    case SageResultField::Value:
        return row.m_value;
    case SageResultField::Status:
        return row.m_status;
    case SageResultField::Reason:
        return row.m_reason;
    case SageResultField::Field:
        return row.m_field;
    }
    return row.m_field;
}
