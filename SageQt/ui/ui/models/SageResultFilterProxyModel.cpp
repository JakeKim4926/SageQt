#include "ui/models/SageResultFilterProxyModel.h"

#include "SageDefine.h"
#include "ui/models/SageResultTableModel.h"

#include <QStringList>

SageResultFilterProxyModel::SageResultFilterProxyModel(SageResultTableModel& resultModel, QObject* parent)
    : QSortFilterProxyModel(parent)
    , m_resultModel(resultModel)
{
    setSourceModel(&resultModel);
}

void SageResultFilterProxyModel::setFilter(const QString& keyword, SageResultField field)
{
    m_resultModel.clearCheckStates();
    beginFilterChange();
    m_keyword = keyword.trimmed();
    m_field = field;
    endFilterChange(Direction::Rows);
}

QList<SageResultRow> SageResultFilterProxyModel::visibleRows() const
{
    QList<SageResultRow> rows;
    rows.reserve(rowCount());
    for (int row = 0; row < rowCount(); ++row) {
        rows.append(m_resultModel.row(mapToSource(index(row, 0)).row()));
    }
    return rows;
}

int SageResultFilterProxyModel::checkedRowCount() const
{
    int checkedCount = 0;
    for (int row = 0; row < rowCount(); ++row) {
        if (isRowChecked(row)) {
            ++checkedCount;
        }
    }
    return checkedCount;
}

bool SageResultFilterProxyModel::isRowChecked(int row) const
{
    return index(row, 0).data(Qt::CheckStateRole).value<Qt::CheckState>() == Qt::Checked;
}

void SageResultFilterProxyModel::setRowChecked(int row, bool checked)
{
    setData(index(row, 0), checked ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
}

void SageResultFilterProxyModel::setAllRowsChecked(bool checked)
{
    for (int row = 0; row < rowCount(); ++row) {
        setRowChecked(row, checked);
    }
}

QString SageResultFilterProxyModel::checkedRowNums() const
{
    QStringList rowNums;
    for (int row = 0; row < rowCount(); ++row) {
        const int rowIndex = sourceRowIndex(row);
        if (!isRowChecked(row) || rowIndex == 0) {
            continue;
        }
        rowNums.append(QString::number(rowIndex));
    }
    return rowNums.join(SAGE_UI_ROW_NUM_SEPARATOR);
}

void SageResultFilterProxyModel::restoreCheckedRowNums(const QString& checkedRowNums)
{
    QStringList rowNums = checkedRowNums.split(SAGE_UI_ROW_NUM_SEPARATOR, Qt::SkipEmptyParts);
    for (QString& rowNum : rowNums) {
        rowNum = rowNum.trimmed();
    }
    for (int row = 0; row < rowCount(); ++row) {
        const int rowIndex = sourceRowIndex(row);
        if (rowIndex != 0 && rowNums.contains(QString::number(rowIndex))) {
            setRowChecked(row, true);
        }
    }
}

bool SageResultFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    Q_UNUSED(sourceParent)
    if (m_keyword.isEmpty()) {
        return true;
    }
    return SageWorkflowResultTable::rowText(m_resultModel.row(sourceRow), m_field)
        .contains(m_keyword, Qt::CaseInsensitive);
}

int SageResultFilterProxyModel::sourceRowIndex(int row) const
{
    return index(row, 0).data(static_cast<int>(SageResultTableRole::SourceRowIndex)).toInt();
}
