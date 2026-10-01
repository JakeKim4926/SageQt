#pragma once

#include <QModelIndex>
#include <QSortFilterProxyModel>

class SageHistoryModel;

enum class SageHistoryFilter
{
    All,
    Success,
    Failed
};

class SageHistoryFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit SageHistoryFilterProxyModel(SageHistoryModel& historyModel, QObject* parent = nullptr);

    void setFilter(SageHistoryFilter filter);
    SageHistoryFilter filter() const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    SageHistoryModel& m_historyModel;
    SageHistoryFilter m_filter = SageHistoryFilter::All;
};
