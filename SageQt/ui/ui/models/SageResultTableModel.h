#pragma once

#include "core/workflow/SageResultRow.h"
#include "core/workflow/SageWorkflowResultTable.h"

#include <QAbstractTableModel>
#include <QList>
#include <QModelIndex>
#include <QVariant>

enum class SageResultTableRole
{
    SourceRowIndex = Qt::UserRole
};

class SageResultTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SageResultTableModel(QObject* parent = nullptr);

    void setColumns(const QList<SageWorkflowColumn>& columns, const SageWorkflowResultStyle& style);
    void setRows(const QList<SageResultRow>& rows);
    void clearRows();
    void clearCheckStates();

    const SageResultRow& row(int row) const;
    SageWorkflowResultStyle resultStyle() const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    bool isCheckCell(const QModelIndex& index) const;
    Qt::Alignment columnAlignment(int column) const;

private:
    QList<SageWorkflowColumn> m_columns;
    SageWorkflowResultStyle m_style;
    QList<SageResultRow> m_rows;
    QList<bool> m_checked;
};
