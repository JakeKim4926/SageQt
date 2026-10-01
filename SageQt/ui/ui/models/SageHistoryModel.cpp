#include "ui/models/SageHistoryModel.h"

#include "SageDefine.h"
#include "ui/models/SageTableRole.h"

SageHistoryModel::SageHistoryModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

void SageHistoryModel::prependEntries(const QList<SageHistoryEntry>& entries)
{
    if (entries.isEmpty()) {
        return;
    }
    beginInsertRows(QModelIndex(), 0, static_cast<int>(entries.size()) - 1);
    m_entries = entries + m_entries;
    endInsertRows();
}

const SageHistoryEntry& SageHistoryModel::entry(int row) const
{
    return m_entries.at(row);
}

int SageHistoryModel::successCount() const
{
    int count = 0;
    for (const SageHistoryEntry& historyEntry : m_entries) {
        if (historyEntry.m_success) {
            ++count;
        }
    }
    return count;
}

int SageHistoryModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(m_entries.size());
}

int SageHistoryModel::columnCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(SageHistoryColumn::Count);
}

QVariant SageHistoryModel::data(const QModelIndex& index, int role) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid)) {
        return {};
    }
    const SageHistoryEntry& historyEntry = m_entries.at(index.row());
    const SageHistoryColumn column = static_cast<SageHistoryColumn>(index.column());
    switch (role) {
    case Qt::DisplayRole:
        return displayText(historyEntry, column);
    case Qt::TextAlignmentRole:
        return QVariant::fromValue(Qt::AlignCenter);
    case static_cast<int>(SageTableRole::Muted):
        return column == SageHistoryColumn::Output && historyEntry.m_success && historyEntry.m_outputPath.isEmpty();
    case static_cast<int>(SageTableRole::RowTone):
        return QVariant::fromValue(historyEntry.m_success ? SageTableTone::None : SageTableTone::Failed);
    case static_cast<int>(SageTableRole::BadgeTone):
        if (column != SageHistoryColumn::Result) {
            return QVariant::fromValue(SageTableTone::None);
        }
        return QVariant::fromValue(historyEntry.m_success ? SageTableTone::Success : SageTableTone::Failed);
    default:
        return {};
    }
}

QVariant SageHistoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
        return QAbstractTableModel::headerData(section, orientation, role);
    }
    switch (static_cast<SageHistoryColumn>(section)) {
    case SageHistoryColumn::Time:
        return SAGE_UI_HISTORY_COL_TIME;
    case SageHistoryColumn::Result:
        return SAGE_UI_HISTORY_COL_RESULT;
    case SageHistoryColumn::Input:
        return SAGE_UI_HISTORY_COL_INPUT;
    case SageHistoryColumn::Output:
        return SAGE_UI_HISTORY_COL_OUTPUT;
    case SageHistoryColumn::Reason:
        return SAGE_UI_HISTORY_COL_REASON;
    case SageHistoryColumn::Count:
        break;
    }
    return {};
}

QString SageHistoryModel::displayText(const SageHistoryEntry& entry, SageHistoryColumn column)
{
    switch (column) {
    case SageHistoryColumn::Time:
        return entry.m_time.toString(SAGE_UI_HISTORY_TIME_FORMAT);
    case SageHistoryColumn::Result:
        return entry.m_success ? SAGE_UI_HISTORY_SUCCESS : SAGE_UI_HISTORY_FAILED;
    case SageHistoryColumn::Input:
        return entry.m_inputPath.isEmpty() ? SAGE_UI_AMOUNT_EMPTY_MARK : entry.m_inputPath;
    case SageHistoryColumn::Output:
        if (!entry.m_success) {
            return SAGE_UI_AMOUNT_EMPTY_MARK;
        }
        return entry.m_outputPath.isEmpty() ? SAGE_UI_HISTORY_NO_OUTPUT : entry.m_outputPath;
    case SageHistoryColumn::Reason:
        return entry.m_success || entry.m_reason.isEmpty() ? SAGE_UI_AMOUNT_EMPTY_MARK : entry.m_reason;
    case SageHistoryColumn::Count:
        break;
    }
    return {};
}
