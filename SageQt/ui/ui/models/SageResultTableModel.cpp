#include "ui/models/SageResultTableModel.h"

#include "SageDefine.h"

SageResultTableModel::SageResultTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

void SageResultTableModel::setColumns(const QList<SageWorkflowColumn>& columns, const SageWorkflowResultStyle& style)
{
    beginResetModel();
    m_columns = columns;
    m_style = style;
    m_rows.clear();
    m_checked.clear();
    endResetModel();
}

void SageResultTableModel::setRows(const QList<SageResultRow>& rows)
{
    beginResetModel();
    m_rows = rows;
    m_checked = QList<bool>(rows.size(), false);
    endResetModel();
}

void SageResultTableModel::clearRows()
{
    setRows({});
}

void SageResultTableModel::clearCheckStates()
{
    if (!m_checked.contains(true)) {
        return;
    }
    m_checked.fill(false);
    emit dataChanged(index(0, 0), index(rowCount() - 1, 0), {Qt::CheckStateRole});
}

const SageResultRow& SageResultTableModel::row(int row) const
{
    return m_rows.at(row);
}

const SageWorkflowColumn& SageResultTableModel::column(int column) const
{
    return m_columns.at(column);
}

SageWorkflowResultStyle SageResultTableModel::resultStyle() const
{
    return m_style;
}

int SageResultTableModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid() || m_columns.isEmpty()) {
        return 0;
    }
    return static_cast<int>(m_rows.size());
}

int SageResultTableModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(m_columns.size());
}

QVariant SageResultTableModel::data(const QModelIndex& index, int role) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid)) {
        return {};
    }
    const SageResultRow& resultRow = m_rows.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
        return SageWorkflowResultTable::rowText(resultRow, m_columns.at(index.column()).m_field);
    case Qt::TextAlignmentRole:
        return QVariant::fromValue(columnAlignment(index.column()) | Qt::AlignVCenter);
    case Qt::CheckStateRole:
        if (!isCheckCell(index)) {
            return {};
        }
        return m_checked.at(index.row()) ? Qt::Checked : Qt::Unchecked;
    case static_cast<int>(SageTableRole::SourceRowIndex):
        return resultRow.m_sourceRowIndex;
    case static_cast<int>(SageTableRole::Highlighted):
        return isHighlightColumn(index.column());
    case static_cast<int>(SageTableRole::Muted):
        return m_style.m_highlightCount > 0 &&
               SageWorkflowResultTable::rowText(resultRow, m_columns.at(index.column()).m_field) ==
                   SAGE_UI_AMOUNT_EMPTY_MARK;
    default:
        return {};
    }
}

bool SageResultTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role != Qt::CheckStateRole || !checkIndex(index, CheckIndexOption::IndexIsValid) || !isCheckCell(index)) {
        return false;
    }
    const bool checked = value.value<Qt::CheckState>() == Qt::Checked;
    if (m_checked.at(index.row()) == checked) {
        return true;
    }
    m_checked[index.row()] = checked;
    emit dataChanged(index, index, {Qt::CheckStateRole});
    return true;
}

Qt::ItemFlags SageResultTableModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags itemFlags = QAbstractTableModel::flags(index);
    if (isCheckCell(index)) {
        itemFlags |= Qt::ItemIsUserCheckable;
    }
    return itemFlags;
}

QVariant SageResultTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole || section < 0 || section >= m_columns.size()) {
        return QAbstractTableModel::headerData(section, orientation, role);
    }
    return m_columns.at(section).m_label;
}

bool SageResultTableModel::isCheckCell(const QModelIndex& index) const
{
    return m_style.m_hasCheckbox && index.isValid() && index.column() == 0;
}

bool SageResultTableModel::isHighlightColumn(int column) const
{
    return m_style.m_highlightCount > 0 && column >= m_style.m_highlightStart &&
           column < m_style.m_highlightStart + m_style.m_highlightCount;
}

Qt::Alignment SageResultTableModel::columnAlignment(int column) const
{
    if (column == 0) {
        return Qt::AlignHCenter;
    }
    switch (m_columns.at(column).m_align) {
    case SageColumnAlign::Left:
        return Qt::AlignLeft;
    case SageColumnAlign::Right:
        return Qt::AlignRight;
    case SageColumnAlign::Center:
        return Qt::AlignHCenter;
    }
    return Qt::AlignHCenter;
}
