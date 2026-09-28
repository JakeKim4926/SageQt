#include "ui/window/SageMainWindow.h"

#include "SageDefine.h"
#include "ui/panels/SageSidebarPanel.h"
#include "ui/style/SageDesignDefine.h"

#include <QHBoxLayout>
#include <QWidget>

SageMainWindow::SageMainWindow(const SageWorkflowRegistry& registry, const SageAuthSession& authSession,
                               QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(SAGE_UI_MAIN_WINDOW_TITLE);
    resize(SAGE_MAIN_WINDOW_WIDTH, SAGE_MAIN_WINDOW_HEIGHT);
    createWidgets(registry, authSession);
    createLayout();
}

void SageMainWindow::createWidgets(const SageWorkflowRegistry& registry, const SageAuthSession& authSession)
{
    m_centralWidget = new QWidget(this);
    m_sidebarPanel = new SageSidebarPanel(registry, authSession, m_centralWidget);
    setCentralWidget(m_centralWidget);
}

void SageMainWindow::createLayout()
{
    QHBoxLayout* layout = new QHBoxLayout(m_centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_sidebarPanel);
    layout->addStretch();
}
