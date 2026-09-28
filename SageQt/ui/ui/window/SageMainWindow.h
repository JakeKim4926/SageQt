#pragma once

#include <QMainWindow>

class SageAuthSession;
class SageSidebarPanel;
class SageWorkflowRegistry;

class SageMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SageMainWindow(const SageWorkflowRegistry& registry, const SageAuthSession& authSession, QWidget* parent = nullptr);

private:
    void createWidgets(const SageWorkflowRegistry& registry, const SageAuthSession& authSession);
    void createLayout();

private:
    QWidget* m_centralWidget = nullptr;
    SageSidebarPanel* m_sidebarPanel = nullptr;
};
