#pragma once

#include "core/workflow/SageWorkflowHistory.h"

#include <QList>
#include <QWidget>

class QSpacerItem;
class QVBoxLayout;
class SageEmptyState;
class SageFilterPillBar;
class SageHistoryFilterProxyModel;
class SageHistoryModel;
class SageTableView;

class SageWorkflowHistoryPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SageWorkflowHistoryPanel(QWidget* parent = nullptr);

    void appendEntries(const QList<SageHistoryEntry>& entries);
    int visibleRowCount() const;

private slots:
    void onFilterPillBarSelectedIndexChanged(int index);

private:
    void createWidgets();
    void createLayout();
    void connectSignals();
    void updateFilterLabels();
    void updateEmptyState();

private:
    SageFilterPillBar* m_filterPillBar = nullptr;
    QSpacerItem* m_filterGap = nullptr;
    QVBoxLayout* m_layout = nullptr;
    SageTableView* m_tableView = nullptr;
    SageEmptyState* m_emptyState = nullptr;
    SageHistoryModel* m_model = nullptr;
    SageHistoryFilterProxyModel* m_proxy = nullptr;
};
