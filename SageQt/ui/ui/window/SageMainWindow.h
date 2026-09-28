#pragma once

#include <QMainWindow>

class QFrame;
class SageAuthSession;
class SageHeaderPanel;
class SageSidebarPanel;
class SageWorkflowRegistry;

class SageMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SageMainWindow(const SageWorkflowRegistry& registry, SageAuthSession& authSession, QWidget* parent = nullptr);

private:
    void createWidgets(const SageWorkflowRegistry& registry, SageAuthSession& authSession);
    void createLayout();
    void connectSignals();

private:
    QWidget* m_centralWidget = nullptr;
    SageSidebarPanel* m_sidebarPanel = nullptr;
    QFrame* m_sidebarDivider = nullptr;
    QWidget* m_contentArea = nullptr;
    SageHeaderPanel* m_headerPanel = nullptr;
};
