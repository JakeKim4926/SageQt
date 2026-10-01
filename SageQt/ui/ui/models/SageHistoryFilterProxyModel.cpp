#include "ui/models/SageHistoryFilterProxyModel.h"

#include "ui/models/SageHistoryModel.h"

SageHistoryFilterProxyModel::SageHistoryFilterProxyModel(SageHistoryModel& historyModel, QObject* parent)
    : QSortFilterProxyModel(parent)
    , m_historyModel(historyModel)
{
    setSourceModel(&historyModel);
}

void SageHistoryFilterProxyModel::setFilter(SageHistoryFilter filter)
{
    beginFilterChange();
    m_filter = filter;
    endFilterChange(Direction::Rows);
}

SageHistoryFilter SageHistoryFilterProxyModel::filter() const
{
    return m_filter;
}

bool SageHistoryFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    Q_UNUSED(sourceParent)
    const bool success = m_historyModel.entry(sourceRow).m_success;
    switch (m_filter) {
    case SageHistoryFilter::Success:
        return success;
    case SageHistoryFilter::Failed:
        return !success;
    case SageHistoryFilter::All:
        break;
    }
    return true;
}
