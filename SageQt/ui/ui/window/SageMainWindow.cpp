#include "ui/window/SageMainWindow.h"

#include "SageDefine.h"
#include "ui/dialogs/SageLoginDlg.h"
#include "ui/dialogs/SagePasswordChangeDlg.h"
#include "ui/panels/SageHeaderPanel.h"
#include "ui/panels/SageSidebarPanel.h"
#include "ui/panels/SageWorkspacePanel.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/window/SageFileDropFilter.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

#include <optional>

SageMainWindow::SageMainWindow(const SageWorkflowRegistry& registry, const SageUserService& userService,
                               SageAuthSession& authSession, QWidget* parent)
    : QMainWindow(parent)
    , m_userService(userService)
    , m_authSession(authSession)
{
    setWindowTitle(SAGE_UI_MAIN_WINDOW_TITLE);
    resize(SAGE_MAIN_WINDOW_WIDTH, SAGE_MAIN_WINDOW_HEIGHT);
    createWidgets(registry, authSession);
    createLayout();
    connectSignals();
}

void SageMainWindow::openLoginDialog()
{
    SageLoginDlg loginDialog(m_userService, m_authSession, this);
    loginDialog.exec();
}

void SageMainWindow::openPasswordChangeDialog()
{
    SagePasswordChangeDlg passwordDialog(m_userService, m_authSession, this);
    passwordDialog.exec();
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
    m_workspacePanel = new SageWorkspacePanel(registry, m_contentArea);
    m_fileDropFilter = new SageFileDropFilter(this);
    setAcceptDrops(true);
    installEventFilter(m_fileDropFilter);
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
    contentLayout->addWidget(m_workspacePanel);
}

void SageMainWindow::connectSignals()
{
    connect(m_sidebarPanel, &SageSidebarPanel::workflowSelected, m_headerPanel, &SageHeaderPanel::showWorkflow);
    connect(m_sidebarPanel, &SageSidebarPanel::workflowSelected, m_workspacePanel, &SageWorkspacePanel::showWorkflow);
    connect(m_fileDropFilter, &SageFileDropFilter::filesDropped, m_workspacePanel,
            &SageWorkspacePanel::applyDroppedPaths);
    connect(m_headerPanel, &SageHeaderPanel::loginRequested, this, &SageMainWindow::openLoginDialog);
    connect(m_sidebarPanel, &SageSidebarPanel::passwordChangeRequested, this,
            &SageMainWindow::openPasswordChangeDialog);
    const std::optional<SageWorkflowType> selectedWorkflow = m_sidebarPanel->selectedWorkflow();
    if (selectedWorkflow.has_value()) {
        m_headerPanel->showWorkflow(*selectedWorkflow);
        m_workspacePanel->showWorkflow(*selectedWorkflow);
    }
}
