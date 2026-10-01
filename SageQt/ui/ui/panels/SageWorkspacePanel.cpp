#include "ui/panels/SageWorkspacePanel.h"

#include "core/workflow/ISageWorkflowHandler.h"
#include "core/workflow/SageWorkflowRegistry.h"
#include "core/workflow/SageWorkflowResultPresenter.h"
#include "ui/dialogs/SageMessageBoxDlg.h"
#include "ui/panels/SageResultTablePanel.h"
#include "ui/panels/SageWorkflowHistoryPanel.h"
#include "ui/panels/SageWorkflowInputPanel.h"
#include "ui/panels/SageWorkflowResultPanel.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/widgets/SageSurface.h"
#include "ui/workflow/SageWorkflowController.h"

#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QList>
#include <QSignalBlocker>
#include <QStackedWidget>
#include <QTabBar>
#include <QUrl>
#include <QVBoxLayout>

#include <optional>
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

bool SageWorkspacePanel::isRunning() const
{
    return m_controller->isRunning();
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
        m_controller->clearResult();
        return;
    }
    m_currentWorkflow = workflowType;
    m_tabs = handler->tabs();
    rebuildTabs();
    const SageWorkspaceState state = m_states.value(workflowType);
    selectTabKind(state.m_tabKind);
    m_controller->restoreResult(state.m_result);
    m_inputPanel->applyHandler(*handler);
    m_inputPanel->setInputPath(state.m_inputPath);
    m_inputPanel->setOutputFolder(state.m_outputFolder);
    applyResultTableSchema(*handler, m_controller->resultState().m_taskType.value_or(SageTaskType::Generate));
}

void SageWorkspacePanel::applyDroppedPaths(const QStringList& paths)
{
    if (paths.isEmpty() || m_controller->isRunning() || !m_currentWorkflow.has_value()) {
        return;
    }
    const ISageWorkflowHandler* handler = m_registry.findHandler(*m_currentWorkflow);
    if (handler == nullptr) {
        return;
    }
    m_inputPanel->setInputPath(paths.constFirst());
    selectTabKind(SageWorkflowTabKind::Input);
    if (handler->hasInputTable()) {
        onRunRequested(SageTaskType::Load);
    }
}

void SageWorkspacePanel::onTabChanged(int visualIndex)
{
    if (visualIndex < 0 || visualIndex >= m_tabs.size()) {
        return;
    }
    m_selectedTabKind = m_tabs.at(visualIndex).m_kind;
    m_panelStack->setCurrentWidget(panelFor(m_selectedTabKind));
}

void SageWorkspacePanel::onRunRequested(SageTaskType taskType)
{
    if (m_controller->isRunning() || !m_currentWorkflow.has_value()) {
        return;
    }
    QString inputPath;
    QString outputFolder;
    if (!validateInputPath(inputPath)) {
        return;
    }
    if (taskType == SageTaskType::Generate && !validateOutputFolder(outputFolder)) {
        return;
    }

    SageWorkflowRunRequest request;
    request.m_workflowType = *m_currentWorkflow;
    request.m_taskType = taskType;
    request.m_inputPath = inputPath;
    request.m_outputFolder = outputFolder;

    QString error;
    if (!m_controller->start(request, error)) {
        SageMessageBoxDlg errorDialog(SageMessageIcon::Warning, error, this);
        errorDialog.exec();
        return;
    }
    setRunningState(true);
}

void SageWorkspacePanel::onRunFinished(const SageWorkflowRunResult& result)
{
    const ISageWorkflowHandler* handler = m_registry.findHandler(result.m_workflowType);
    const bool keepInputTable =
        result.m_taskType == SageTaskType::Generate && handler != nullptr && handler->hasInputTable();
    QList<SageResultRow> rows;
    const bool success = SageWorkflowResultPresenter().buildRows(handler, result.m_taskType, result.m_response, rows);
    m_controller->finish(result, success, keepInputTable);
    if (handler != nullptr && !keepInputTable && !handler->hasInputTable()) {
        applyResultTableSchema(*handler, result.m_taskType);
        m_resultPanel->resultTable().setRows(rows);
    }

    if (handler != nullptr) {
        selectTabKind(handler->hasInputTable() ? SageWorkflowTabKind::Input : SageWorkflowTabKind::DocumentResult);
        const std::optional<QString> completedMessage = handler->generateCompletedMessage();
        if (result.m_taskType == SageTaskType::Generate && success && completedMessage.has_value()) {
            SageMessageBoxDlg completedDialog(SageMessageIcon::Info, *completedMessage, this);
            completedDialog.exec();
        }
    }
    applyStatusCardResult(handler, result.m_taskType, result.m_response, success, static_cast<int>(rows.size()));
    setRunningState(false);
}

void SageWorkspacePanel::onOpenOutputFolder()
{
    if (m_lastOutputPath.isEmpty()) {
        return;
    }
    const QFileInfo outputInfo(m_lastOutputPath);
    if (!outputInfo.exists()) {
        SageMessageBoxDlg missingDialog(SageMessageIcon::Warning, SAGE_UI_OUTPUT_PATH_MISSING, this);
        missingDialog.exec();
        return;
    }
    const QString folder = outputInfo.isDir() ? outputInfo.absoluteFilePath() : outputInfo.absolutePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(folder));
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

    m_contentArea = new QWidget(this);
    m_panelStack = new QStackedWidget(m_contentArea);
    m_inputPanel = new SageWorkflowInputPanel(m_panelStack);
    m_resultPanel = new SageWorkflowResultPanel(m_panelStack);
    m_historyPanel = new SageWorkflowHistoryPanel(m_panelStack);
    m_panelStack->addWidget(m_inputPanel);
    m_panelStack->addWidget(m_resultPanel);
    m_panelStack->addWidget(m_historyPanel);

    m_controller = new SageWorkflowController(m_registry, this);
}

void SageWorkspacePanel::createLayout()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_tabRow);
    layout->addWidget(m_contentArea);

    QVBoxLayout* tabRowLayout = new QVBoxLayout(m_tabRow);
    tabRowLayout->setContentsMargins(0, 0, 0, 0);
    tabRowLayout->setSpacing(0);
    tabRowLayout->addWidget(m_tabStrip);
    tabRowLayout->addWidget(m_tabRowLine);

    QHBoxLayout* tabStripLayout = new QHBoxLayout(m_tabStrip);
    tabStripLayout->setContentsMargins(SAGE_CONTENT_PAD_X, 0, SAGE_CONTENT_PAD_X, 0);
    tabStripLayout->addWidget(m_taskTabs);
    tabStripLayout->addStretch();

    QVBoxLayout* contentLayout = new QVBoxLayout(m_contentArea);
    contentLayout->setContentsMargins(SAGE_CONTENT_PAD_X, SAGE_CONTENT_PAD_Y, SAGE_CONTENT_PAD_X, SAGE_CONTENT_PAD_Y);
    contentLayout->addWidget(m_panelStack);
}

void SageWorkspacePanel::connectSignals()
{
    connect(m_taskTabs, &QTabBar::currentChanged, this, &SageWorkspacePanel::onTabChanged);
    connect(m_inputPanel, &SageWorkflowInputPanel::runRequested, this, &SageWorkspacePanel::onRunRequested);
    connect(m_controller, &SageWorkflowController::runFinished, this, &SageWorkspacePanel::onRunFinished);
    connect(m_inputPanel, &SageWorkflowInputPanel::openOutputFolderRequested, this,
            &SageWorkspacePanel::onOpenOutputFolder);
}

void SageWorkspacePanel::saveCurrentState()
{
    if (!m_currentWorkflow.has_value()) {
        return;
    }
    SageWorkspaceState& state = m_states[*m_currentWorkflow];
    state.m_tabKind = m_selectedTabKind;
    state.m_inputPath = m_inputPanel->inputPath();
    state.m_outputFolder = m_inputPanel->outputFolder();
    state.m_result = m_controller->resultState();
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

bool SageWorkspacePanel::validateInputPath(QString& outInputPath)
{
    outInputPath = m_inputPanel->inputPath().trimmed();
    if (!outInputPath.isEmpty()) {
        return true;
    }
    SageMessageBoxDlg requiredDialog(SageMessageIcon::Warning, SAGE_UI_INPUT_REQUIRED, this);
    requiredDialog.exec();
    return false;
}

bool SageWorkspacePanel::validateOutputFolder(QString& outOutputFolder)
{
    outOutputFolder = m_inputPanel->outputFolder().trimmed();
    if (!outOutputFolder.isEmpty()) {
        return true;
    }
    SageMessageBoxDlg requiredDialog(SageMessageIcon::Warning, SAGE_UI_OUTPUT_REQUIRED, this);
    requiredDialog.exec();
    return false;
}

void SageWorkspacePanel::setRunningState(bool running)
{
    m_inputPanel->setRunningState(running);
}

void SageWorkspacePanel::applyStatusCardResult(const ISageWorkflowHandler* handler, SageTaskType taskType,
                                               const QJsonObject& response, bool success, int resultCount)
{
    if (handler == nullptr) {
        return;
    }
    m_lastOutputPath.clear();
    if (!success) {
        const QString message = taskType == SageTaskType::Load
                                    ? SAGE_UI_STATUS_CARD_LOAD_FAILED
                                    : SAGE_UI_STATUS_CARD_FAILED_FORMAT.arg(handler->actionButtonLabel());
        const QJsonObject error = response.value(SAGE_JSON_KEY_ERROR).toObject();
        QString detail = error.value(SAGE_JSON_KEY_MESSAGE).toString();
        if (detail.isEmpty()) {
            detail = error.value(SAGE_JSON_KEY_CODE).toString();
        }
        m_inputPanel->setStatusResult(false, message, detail);
        return;
    }
    const QString message =
        taskType == SageTaskType::Load
            ? SAGE_UI_STATUS_CARD_LOAD_COMPLETED_FORMAT.arg(resultCount)
            : SAGE_UI_STATUS_CARD_COMPLETED_FORMAT.arg(handler->actionButtonLabel()).arg(resultCount);
    const QJsonObject payload = response.value(SAGE_JSON_KEY_PAYLOAD).toObject();
    QString outputPath = payload.value(SAGE_JSON_KEY_FILE_PATH).toString();
    if (outputPath.isEmpty()) {
        outputPath = payload.value(SAGE_JSON_KEY_OUTPUT_FOLDER).toString();
    }
    m_lastOutputPath = outputPath;
    m_inputPanel->setStatusResult(true, message, QDir::toNativeSeparators(outputPath));
}

void SageWorkspacePanel::applyResultTableSchema(const ISageWorkflowHandler& handler, SageTaskType taskType)
{
    if (handler.hasInputTable()) {
        return;
    }
    m_resultPanel->resultTable().setColumns(handler.resultColumns(taskType), handler.resultStyle(taskType));
}
