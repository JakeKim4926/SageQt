#pragma once

#include <Qt>

enum class SageTableRole
{
    SourceRowIndex = Qt::UserRole,
    Highlighted,
    Muted,
    RowTone,
    BadgeTone
};

enum class SageTableTone
{
    None,
    Success,
    Failed
};
