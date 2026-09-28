#include "ui/window/SageMainWindow.h"

#include "SageDefine.h"
#include "ui/panels/SageHeaderPanel.h"
#include "ui/panels/SageSidebarPanel.h"
#include "ui/style/SageDesignDefine.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

#include <optional>

SageMainWindow::SageMainWindow(const SageWorkflowRegistry& registry, SageAuthSession& authSession, QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(SAGE_UI_MAIN_WINDOW_TITLE);
    resize(SAGE_MAIN_WINDOW_WIDTH, SAGE_MAIN_WINDOW_HEIGHT);
    createWidgets(registry, authSession);
    createLayout();
    connectSignals();
}

void SageMainWindow::createWidgets(const SageWorkflowRegistry& registry, SageAuthSession& authSession)
{
    m_centralWidget = new QWidget(this);
    m_sidebarPanel = new SageSidebarPanel(registry, authSession, m_centralWidget);
    m_sidebarDivider = new QFrame(m_centralWidget);
    m_sidebarDivider->setFrameShape(QFrame::VLine);
    m_sidebarDivider->setFixedWidth(SAGE_BORDER_THICKNESS);
    m_contentArea = new QWidget(m_centralWidget);
    m_headerPanel = new SageHeaderPanel(registry, authSession, m_contentArea);
    setCentralWidget(m_centralWidget);
}

void SageMainWindow::createLayout()
{
    QHBoxLayout* layout = new QHBoxLayout(m_centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_sidebarPanel);
    layout->addWidget(m_sidebarDivider);
    layout->addWidget(m_contentArea);

    QVBoxLayout* contentLayout = new QVBoxLayout(m_contentArea);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(m_headerPanel);
    contentLayout->addStretch();
}

void SageMainWindow::connectSignals()
{
    connect(m_sidebarPanel, &SageSidebarPanel::workflowSelected, m_headerPanel, &SageHeaderPanel::showWorkflow);
    const std::optional<SageWorkflowType> selectedWorkflow = m_sidebarPanel->selectedWorkflow();
    if (selectedWorkflow.has_value()) {
        m_headerPanel->showWorkflow(*selectedWorkflow);
    }
}
