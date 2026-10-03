#pragma once

#include <QMainWindow>

class QFrame;
class SageFileDropFilter;
class SageAuthSession;
class SageHeaderPanel;
class SageSidebarPanel;
class SageUserService;
class SageLabel;
class SageWorkspacePanel;
class SageWorkflowRegistry;

class SageMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SageMainWindow(const SageWorkflowRegistry& registry, const SageUserService& userService,
                   SageAuthSession& authSession, QWidget* parent = nullptr);

private slots:
    void onHeaderPanelLoginRequested();
    void onSidebarPanelPasswordChangeRequested();

private:
    void createWidgets(const SageWorkflowRegistry& registry, SageAuthSession& authSession);
    void createLayout();
    void connectSignals();

private:
    const SageUserService& m_userService;
    SageAuthSession& m_authSession;
    QWidget* m_centralWidget = nullptr;
    SageSidebarPanel* m_sidebarPanel = nullptr;
    QFrame* m_sidebarDivider = nullptr;
    QWidget* m_contentArea = nullptr;
    SageHeaderPanel* m_headerPanel = nullptr;
    SageWorkspacePanel* m_workspacePanel = nullptr;
    SageLabel* m_statusLabel = nullptr;
    SageFileDropFilter* m_fileDropFilter = nullptr;
};
