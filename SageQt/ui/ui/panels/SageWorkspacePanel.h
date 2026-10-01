#pragma once

#include "SageDefine.h"
#include "core/workflow/SageResultRow.h"
#include "core/workflow/SageWorkflowRunResult.h"
#include "core/workflow/SageWorkflowTab.h"

#include <QHash>
#include <QJsonObject>
#include <QList>
#include <QString>
#include <QStringList>
#include <QWidget>

#include <optional>

class ISageWorkflowHandler;
class QFrame;
class QStackedWidget;
class QTabBar;
class SageResultTablePanel;
class SageSurface;
class SageWorkflowHistoryPanel;
class SageWorkflowInputPanel;
class SageWorkflowRegistry;
class SageWorkflowController;
class SageWorkflowResultPanel;

struct SageWorkspaceState
{
    SageWorkflowTabKind m_tabKind = SageWorkflowTabKind::Input;
    QString m_inputPath;
    QString m_outputFolder;
    SageWorkflowResultState m_result;
    QString m_filterKeyword;
    int m_filterCriteria = SAGE_FILTER_CRITERIA_NONE;
    QString m_checkedRowNums;
};

class SageWorkspacePanel : public QWidget
{
    Q_OBJECT

public:
    explicit SageWorkspacePanel(const SageWorkflowRegistry& registry, QWidget* parent = nullptr);

    SageWorkflowTabKind selectedTabKind() const;
    bool isRunning() const;

public slots:
    void showWorkflow(SageWorkflowType workflowType);
    void applyDroppedPaths(const QStringList& paths);

private slots:
    void onTabChanged(int visualIndex);
    void onRunRequested(SageTaskType taskType);
    void onRunFinished(const SageWorkflowRunResult& result);
    void onOpenOutputFolder();
    void onResultTableChanged();
    void onResultSelectionChanged(int selectedCount);
    void onInputResetRequested();

private:
    void createWidgets();
    void createLayout();
    void connectSignals();
    void saveCurrentState();
    void rebuildTabs();
    void selectTabKind(SageWorkflowTabKind tabKind);
    QWidget* panelFor(SageWorkflowTabKind tabKind) const;
    bool validateInputPath(QString& outInputPath);
    bool validateOutputFolder(QString& outOutputFolder);
    void setRunningState(bool running);
    const ISageWorkflowHandler* findCurrentHandler() const;
    SageResultTablePanel& resultTableFor(const ISageWorkflowHandler& handler) const;
    bool isLastResultOf(const ISageWorkflowHandler& handler) const;
    bool isInputTableVisible(const ISageWorkflowHandler& handler) const;
    bool isResultFilterVisible(const ISageWorkflowHandler& handler) const;
    void refreshVisibility();
    void rebuildResultTable(const ISageWorkflowHandler& handler, const QString& checkedRowNums);
    void applyResultTableSchema(const ISageWorkflowHandler& handler, SageTaskType taskType);
    void setResultTableRows(const ISageWorkflowHandler& handler, const QList<SageResultRow>& rows);
    void updateResultSummary(const ISageWorkflowHandler& handler);
    void updateActionButtonState();
    void applyActionButtonState(int selectedCount);
    bool buildSelectedRowNums(const ISageWorkflowHandler& handler, SageTaskType taskType, QString& outRowNums);
    void applyStatusCardResult(const ISageWorkflowHandler* handler, SageTaskType taskType, const QJsonObject& response,
                               bool success, int resultCount);

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
    SageWorkflowController* m_controller = nullptr;
    QList<SageWorkflowTab> m_tabs;
    SageWorkflowTabKind m_selectedTabKind = SageWorkflowTabKind::Input;
    std::optional<SageWorkflowType> m_currentWorkflow;
    QHash<SageWorkflowType, SageWorkspaceState> m_states;
    QString m_lastOutputPath;
};
