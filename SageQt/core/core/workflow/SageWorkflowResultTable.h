#pragma once

#include "core/workflow/SageResultRow.h"

#include <QList>
#include <QString>

enum class SageColumnAlign
{
    Left,
    Right,
    Center
};

enum class SageResultField
{
    Field,
    Value,
    Status,
    Reason
};

enum class SageResultTotalRole
{
    Label,
    Count,
    Amount,
    AmountHighlight
};

struct SageWorkflowColumn
{
    QString m_label;
    SageColumnAlign m_align = SageColumnAlign::Center;
    bool m_isStretch = false;
    SageResultField m_field = SageResultField::Field;
};

struct SageWorkflowFilterCriteria
{
    int m_criteria = 0;
    QString m_label;
    SageResultField m_field = SageResultField::Field;
};

struct SageResultSummaryItem
{
    QString m_label;
    QString m_value;
    QString m_unit;
    bool m_isHighlighted = false;
    bool m_isBadge = false;
};

struct SageResultTotalCell
{
    int m_column = 0;
    QString m_text;
    SageResultTotalRole m_role = SageResultTotalRole::Label;
};

struct SageWorkflowResultStyle
{
    bool m_hasCheckbox = false;
    bool m_hasGridLines = false;
    int m_highlightStart = 0;
    int m_highlightCount = 0;
};

class SageWorkflowResultTable
{
public:
    static QList<SageWorkflowColumn> genericColumns();
    static QString rowText(const SageResultRow& row, SageResultField field);
};
