#pragma once

#include "SageDefine.h"
#include "core/workflow/SageWorkflowTab.h"

#include <QHash>
#include <QList>
#include <QString>
#include <QStringList>
#include <QWidget>

#include <optional>

class QFrame;
class QStackedWidget;
class QTabBar;
class SageSurface;
class SageWorkflowHistoryPanel;
class SageWorkflowInputPanel;
class SageWorkflowRegistry;
class SageWorkflowResultPanel;

struct SageWorkspaceState
{
    SageWorkflowTabKind m_tabKind = SageWorkflowTabKind::Input;
    QString m_inputPath;
    QString m_outputFolder;
};

class SageWorkspacePanel : public QWidget
{
    Q_OBJECT

public:
    explicit SageWorkspacePanel(const SageWorkflowRegistry& registry, QWidget* parent = nullptr);

    SageWorkflowTabKind selectedTabKind() const;

public slots:
    void showWorkflow(SageWorkflowType workflowType);
    void applyDroppedPaths(const QStringList& paths);

private slots:
    void onTabChanged(int visualIndex);

private:
    void createWidgets();
    void createLayout();
    void connectSignals();
    void saveCurrentState();
    void rebuildTabs();
    void selectTabKind(SageWorkflowTabKind tabKind);
    QWidget* panelFor(SageWorkflowTabKind tabKind) const;

private:
    const SageWorkflowRegistry& m_registry;
    SageSurface* m_tabRow = nullptr;
    QWidget* m_tabStrip = nullptr;
    QTabBar* m_taskTabs = nullptr;
    QFrame* m_tabRowLine = nullptr;
    QWidget* m_contentArea = nullptr;
    QStackedWidget* m_panelStack = nullptr;
    SageWorkflowInputPanel* m_inputPanel = nullptr;
    SageWorkflowResultPanel* m_resultPanel = nullptr;
    SageWorkflowHistoryPanel* m_historyPanel = nullptr;
    QList<SageWorkflowTab> m_tabs;
    SageWorkflowTabKind m_selectedTabKind = SageWorkflowTabKind::Input;
    std::optional<SageWorkflowType> m_currentWorkflow;
    QHash<SageWorkflowType, SageWorkspaceState> m_states;
};
