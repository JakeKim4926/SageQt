#pragma once

#include "core/workflow/SageWorkflowHistory.h"

#include <QAbstractTableModel>
#include <QList>
#include <QModelIndex>
#include <QVariant>

enum class SageHistoryColumn
{
    Time,
    Result,
    Input,
    Output,
    Reason,
    Count
};

class SageHistoryModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SageHistoryModel(QObject* parent = nullptr);

    void prependEntries(const QList<SageHistoryEntry>& entries);
    const SageHistoryEntry& entry(int row) const;
    int successCount() const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    static QString displayText(const SageHistoryEntry& entry, SageHistoryColumn column);

private:
    QList<SageHistoryEntry> m_entries;
};
