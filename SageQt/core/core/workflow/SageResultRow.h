#pragma once

#include <QString>

struct SageResultRow
{
    int m_sourceRowIndex = 0;
    QString m_field;
    QString m_value;
    QString m_status;
    QString m_reason;
};
