#pragma once

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

    void setFilter(const QString& keyword, SageResultField field);
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

private:
    SageResultTableModel& m_resultModel;
    QString m_keyword;
    SageResultField m_field = SageResultField::Value;
};
