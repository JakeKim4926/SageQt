#include "ui/panels/SageWorkspacePanel.h"

#include "core/workflow/ISageWorkflowHandler.h"
#include "core/workflow/SageWorkflowRegistry.h"
#include "ui/panels/SageWorkflowHistoryPanel.h"
#include "ui/panels/SageWorkflowInputPanel.h"
#include "ui/panels/SageWorkflowResultPanel.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/widgets/SageSurface.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QSignalBlocker>
#include <QStackedWidget>
#include <QTabBar>
#include <QVBoxLayout>

#include <utility>

SageWorkspacePanel::SageWorkspacePanel(const SageWorkflowRegistry& registry, QWidget* parent)
    : QWidget(parent)
    , m_registry(registry)
{
    createWidgets();
    createLayout();
    connectSignals();
}

SageWorkflowTabKind SageWorkspacePanel::selectedTabKind() const
{
    return m_selectedTabKind;
}

void SageWorkspacePanel::showWorkflow(SageWorkflowType workflowType)
{
    saveCurrentState();
    const ISageWorkflowHandler* handler = m_registry.findHandler(workflowType);
    if (handler == nullptr) {
        m_currentWorkflow.reset();
        m_tabs.clear();
        rebuildTabs();
        selectTabKind(SageWorkflowTabKind::Input);
        return;
    }
    m_currentWorkflow = workflowType;
    m_tabs = handler->tabs();
    rebuildTabs();
    selectTabKind(m_states.value(workflowType).m_tabKind);
}

void SageWorkspacePanel::onTabChanged(int visualIndex)
{
    if (visualIndex < 0 || visualIndex >= m_tabs.size()) {
        return;
    }
    m_selectedTabKind = m_tabs.at(visualIndex).m_kind;
    m_panelStack->setCurrentWidget(panelFor(m_selectedTabKind));
}

void SageWorkspacePanel::createWidgets()
{
    m_tabRow = new SageSurface(SageSurface::SageSurfaceVariant::Panel, this);
    m_tabStrip = new QWidget(m_tabRow);
    m_tabStrip->setFixedHeight(SAGE_TAB_HEIGHT - SAGE_BORDER_THICKNESS);
    m_taskTabs = new QTabBar(m_tabStrip);
    m_taskTabs->setExpanding(false);
    m_taskTabs->setDrawBase(false);
    m_tabRowLine = new QFrame(m_tabRow);
    m_tabRowLine->setFrameShape(QFrame::HLine);
    m_tabRowLine->setFixedHeight(SAGE_BORDER_THICKNESS);

    m_panelStack = new QStackedWidget(this);
    m_inputPanel = new SageWorkflowInputPanel(m_panelStack);
    m_resultPanel = new SageWorkflowResultPanel(m_panelStack);
    m_historyPanel = new SageWorkflowHistoryPanel(m_panelStack);
    m_panelStack->addWidget(m_inputPanel);
    m_panelStack->addWidget(m_resultPanel);
    m_panelStack->addWidget(m_historyPanel);
}

void SageWorkspacePanel::createLayout()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_tabRow);
    layout->addWidget(m_panelStack);

    QVBoxLayout* tabRowLayout = new QVBoxLayout(m_tabRow);
    tabRowLayout->setContentsMargins(0, 0, 0, 0);
    tabRowLayout->setSpacing(0);
    tabRowLayout->addWidget(m_tabStrip);
    tabRowLayout->addWidget(m_tabRowLine);

    QHBoxLayout* tabStripLayout = new QHBoxLayout(m_tabStrip);
    tabStripLayout->setContentsMargins(SAGE_CONTENT_PAD_X, 0, SAGE_CONTENT_PAD_X, 0);
    tabStripLayout->addWidget(m_taskTabs);
    tabStripLayout->addStretch();

    m_panelStack->setContentsMargins(SAGE_CONTENT_PAD_X, SAGE_CONTENT_PAD_Y, SAGE_CONTENT_PAD_X, SAGE_CONTENT_PAD_Y);
}

void SageWorkspacePanel::connectSignals()
{
    connect(m_taskTabs, &QTabBar::currentChanged, this, &SageWorkspacePanel::onTabChanged);
}

void SageWorkspacePanel::saveCurrentState()
{
    if (!m_currentWorkflow.has_value()) {
        return;
    }
    m_states[*m_currentWorkflow].m_tabKind = m_selectedTabKind;
}

void SageWorkspacePanel::rebuildTabs()
{
    const QSignalBlocker blocker(m_taskTabs);
    while (m_taskTabs->count() > 0) {
        m_taskTabs->removeTab(0);
    }
    for (const SageWorkflowTab& tab : std::as_const(m_tabs)) {
        m_taskTabs->addTab(tab.m_label);
    }
}

void SageWorkspacePanel::selectTabKind(SageWorkflowTabKind tabKind)
{
    int visualIndex = 0;
    for (int index = 0; index < m_tabs.size(); ++index) {
        if (m_tabs.at(index).m_kind == tabKind) {
            visualIndex = index;
            break;
        }
    }
    const SageWorkflowTabKind selectedKind =
        m_tabs.isEmpty() ? SageWorkflowTabKind::Input : m_tabs.at(visualIndex).m_kind;
    {
        const QSignalBlocker blocker(m_taskTabs);
        m_taskTabs->setCurrentIndex(visualIndex);
    }
    m_selectedTabKind = selectedKind;
    m_panelStack->setCurrentWidget(panelFor(m_selectedTabKind));
}

QWidget* SageWorkspacePanel::panelFor(SageWorkflowTabKind tabKind) const
{
    switch (tabKind) {
    case SageWorkflowTabKind::Input:
        return m_inputPanel;
    case SageWorkflowTabKind::DocumentResult:
        return m_resultPanel;
    case SageWorkflowTabKind::DocumentHistory:
        return m_historyPanel;
    }
    return m_inputPanel;
}
