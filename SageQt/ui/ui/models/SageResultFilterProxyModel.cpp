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

void SageResultFilterProxyModel::setFilterCriteria(const QList<SageWorkflowFilterCriteria>& criteria)
{
    beginFilterChange();
    m_criteriaList = criteria;
    endFilterChange(Direction::Rows);
}

void SageResultFilterProxyModel::setFilter(const QString& keyword, int criteria)
{
    m_resultModel.clearCheckStates();
    beginFilterChange();
    m_keyword = keyword.trimmed();
    m_criteria = criteria;
    endFilterChange(Direction::Rows);
}

QList<SageWorkflowFilterCriteria> SageResultFilterProxyModel::filterCriteria() const
{
    return m_criteriaList;
}

QString SageResultFilterProxyModel::keyword() const
{
    return m_keyword;
}

int SageResultFilterProxyModel::criteria() const
{
    return m_criteria;
}

int SageResultFilterProxyModel::effectiveCriteria() const
{
    for (const SageWorkflowFilterCriteria& definition : m_criteriaList) {
        if (definition.m_criteria == m_criteria) {
            return m_criteria;
        }
    }
    if (m_criteriaList.isEmpty()) {
        return SAGE_FILTER_CRITERIA_NONE;
    }
    return m_criteriaList.constFirst().m_criteria;
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
    return SageWorkflowResultTable::rowText(m_resultModel.row(sourceRow), effectiveField())
        .contains(m_keyword, Qt::CaseInsensitive);
}

SageResultField SageResultFilterProxyModel::effectiveField() const
{
    const int criteria = effectiveCriteria();
    for (const SageWorkflowFilterCriteria& definition : m_criteriaList) {
        if (definition.m_criteria == criteria) {
            return definition.m_field;
        }
    }
    return SageResultField::Value;
}

int SageResultFilterProxyModel::sourceRowIndex(int row) const
{
    return index(row, 0).data(static_cast<int>(SageTableRole::SourceRowIndex)).toInt();
}
