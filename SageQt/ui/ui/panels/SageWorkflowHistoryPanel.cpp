#include "ui/panels/SageWorkflowHistoryPanel.h"

#include "SageDefine.h"
#include "ui/models/SageHistoryFilterProxyModel.h"
#include "ui/models/SageHistoryModel.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/widgets/SageEmptyState.h"
#include "ui/widgets/SageFilterPillBar.h"
#include "ui/widgets/SageTableView.h"

#include <QSizePolicy>
#include <QSpacerItem>
#include <QVBoxLayout>

SageWorkflowHistoryPanel::SageWorkflowHistoryPanel(QWidget* parent)
    : QWidget(parent)
{
    createWidgets();
    createLayout();
    connectSignals();
    updateFilterLabels();
    updateEmptyState();
}

void SageWorkflowHistoryPanel::appendEntries(const QList<SageHistoryEntry>& entries)
{
    m_model->prependEntries(entries);
    updateFilterLabels();
    updateEmptyState();
}

int SageWorkflowHistoryPanel::visibleRowCount() const
{
    return m_proxy->rowCount();
}

void SageWorkflowHistoryPanel::createWidgets()
{
    m_filterPills = new SageFilterPillBar(this);
    m_model = new SageHistoryModel(this);
    m_proxy = new SageHistoryFilterProxyModel(*m_model, this);
    m_tableView = new SageTableView(this);
    m_tableView->setModel(m_proxy);
    m_tableView->setRowSeparator(true);
    m_tableView->setColumnSpecs({{SAGE_HISTORY_TIME_WIDTH, false},
                                 {SAGE_HISTORY_RESULT_WIDTH, false},
                                 {SAGE_HISTORY_INPUT_WIDTH, true},
                                 {SAGE_HISTORY_OUTPUT_WIDTH, true},
                                 {SAGE_HISTORY_REASON_WIDTH, true}});
    m_emptyState = new SageEmptyState(this);
}

void SageWorkflowHistoryPanel::createLayout()
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_filterPills);
    m_filterGap = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Fixed);
    m_layout->addItem(m_filterGap);
    m_layout->addWidget(m_tableView, 1);
    m_layout->addWidget(m_emptyState, 1);
}

void SageWorkflowHistoryPanel::connectSignals()
{
    connect(m_filterPills, &SageFilterPillBar::selectedIndexChanged, this, &SageWorkflowHistoryPanel::onFilterSelected);
}

void SageWorkflowHistoryPanel::onFilterSelected(int index)
{
    m_proxy->setFilter(static_cast<SageHistoryFilter>(index));
    updateEmptyState();
}

void SageWorkflowHistoryPanel::updateFilterLabels()
{
    const int totalCount = m_model->rowCount();
    const int successCount = m_model->successCount();
    m_filterPills->setLabels({SAGE_UI_HISTORY_FILTER_ALL.arg(totalCount),
                              SAGE_UI_HISTORY_FILTER_SUCCESS.arg(successCount),
                              SAGE_UI_HISTORY_FILTER_FAILED.arg(totalCount - successCount)});
}

void SageWorkflowHistoryPanel::updateEmptyState()
{
    const bool hasHistory = m_model->rowCount() > 0;
    const bool hasVisibleRows = m_proxy->rowCount() > 0;
    m_filterPills->setVisible(hasHistory);
    m_filterGap->changeSize(0, hasHistory ? SAGE_CARD_ROW_GAP : 0, QSizePolicy::Minimum, QSizePolicy::Fixed);
    m_layout->invalidate();
    m_tableView->setVisible(hasVisibleRows);
    m_emptyState->setVisible(!hasVisibleRows);
    if (hasVisibleRows) {
        return;
    }
    m_emptyState->setContent(hasHistory ? SAGE_UI_HISTORY_FILTER_EMPTY_TITLE : SAGE_UI_HISTORY_EMPTY_TITLE,
                             hasHistory ? SAGE_UI_HISTORY_FILTER_EMPTY_DESC : SAGE_UI_HISTORY_EMPTY_DESC);
}
