#pragma once

#include "SageDefine.h"
#include "core/workflow/SageResultRow.h"
#include "core/workflow/SageWorkflowResultTable.h"

#include <QList>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include <QString>

class SageResultTableModel;

class SageResultFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit SageResultFilterProxyModel(SageResultTableModel& resultModel, QObject* parent = nullptr);

    void setFilterCriteria(const QList<SageWorkflowFilterCriteria>& criteria);
    void setFilter(const QString& keyword, int criteria);
    QList<SageWorkflowFilterCriteria> filterCriteria() const;
    QString keyword() const;
    int criteria() const;
    int effectiveCriteria() const;
    QList<SageResultRow> visibleRows() const;
    int checkedRowCount() const;
    bool isRowChecked(int row) const;
    void setRowChecked(int row, bool checked);
    void setAllRowsChecked(bool checked);
    QString checkedRowNums() const;
    void restoreCheckedRowNums(const QString& checkedRowNums);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    int sourceRowIndex(int row) const;
    SageResultField effectiveField() const;

private:
    SageResultTableModel& m_resultModel;
    QList<SageWorkflowFilterCriteria> m_criteriaList;
    QString m_keyword;
    int m_criteria = SAGE_FILTER_CRITERIA_NONE;
};
