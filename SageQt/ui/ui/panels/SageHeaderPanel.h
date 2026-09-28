#pragma once

#include "SageDefine.h"

#include <QWidget>

class QFrame;
class SageAuthSession;
class SageBadge;
class SageButton;
class SageLabel;
class SageSurface;
class SageWorkflowRegistry;

class SageHeaderPanel : public QWidget
{
    Q_OBJECT

public:
    SageHeaderPanel(const SageWorkflowRegistry& registry, SageAuthSession& authSession, QWidget* parent = nullptr);

signals:
    void loginRequested();

public slots:
    void showWorkflow(SageWorkflowType workflowType);

private slots:
    void onLogoutButtonClicked();
    void updateAuthState();

private:
    void createWidgets();
    void createLayout();
    void connectSignals();

private:
    const SageWorkflowRegistry& m_registry;
    SageAuthSession& m_authSession;
    SageSurface* m_surface = nullptr;
    QWidget* m_row = nullptr;
    SageLabel* m_titleLabel = nullptr;
    SageLabel* m_categoryLabel = nullptr;
    SageLabel* m_userLabel = nullptr;
    SageBadge* m_roleBadge = nullptr;
    SageButton* m_loginButton = nullptr;
    SageButton* m_logoutButton = nullptr;
    QFrame* m_bottomLine = nullptr;
};
