#include "ui/panels/SageWorkspacePanel.h"

#include "core/workflow/ISageWorkflowHandler.h"
#include "core/workflow/SageWorkflowHistory.h"
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

#include <QDateTime>
#include <QDesktopServices>
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
    resultTableFor(*handler).restoreFilter(state.m_filterKeyword, state.m_filterCriteria);
    rebuildResultTable(*handler, state.m_checkedRowNums);
    if (!m_controller->isRunning()) {
        emit statusChanged(SAGE_UI_READY);
    }
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
    emit statusChanged(SAGE_UI_DROP_RECEIVED);
    if (handler->hasInputTable()) {
        onInputPanelRunRequested(SageTaskType::Load);
    }
}

void SageWorkspacePanel::onTaskTabsCurrentChanged(int visualIndex)
{
    if (visualIndex < 0 || visualIndex >= m_tabs.size()) {
        return;
    }
    m_selectedTabKind = m_tabs.at(visualIndex).m_kind;
    m_panelStack->setCurrentWidget(panelFor(m_selectedTabKind));
    refreshVisibility();
}

void SageWorkspacePanel::onInputPanelRunRequested(SageTaskType taskType)
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
    const ISageWorkflowHandler* handler = findCurrentHandler();
    QString selectedRowNums;
    if (handler == nullptr || !buildSelectedRowNums(*handler, taskType, selectedRowNums)) {
        return;
    }

    SageWorkflowRunRequest request;
    request.m_workflowType = *m_currentWorkflow;
    request.m_taskType = taskType;
    request.m_inputPath = inputPath;
    request.m_outputFolder = outputFolder;
    request.m_selectedRowNums = selectedRowNums;

    QString error;
    if (!m_controller->start(request, error)) {
        SageMessageBoxDlg errorDialog(SageMessageIcon::Warning, error, this);
        errorDialog.exec();
        return;
    }
    setRunningState(true);
}

void SageWorkspacePanel::onControllerRunFinished(const SageWorkflowRunResult& result)
{
    const ISageWorkflowHandler* handler = m_registry.findHandler(result.m_workflowType);
    const bool keepInputTable =
        result.m_taskType == SageTaskType::Generate && handler != nullptr && handler->hasInputTable();
    QList<SageResultRow> rows;
    const bool success = SageWorkflowResultPresenter().buildRows(handler, result.m_taskType, result.m_response, rows);
    const QString runningInputPath = m_controller->resultState().m_inputPath;
    m_controller->finish(result, success, keepInputTable);
    if (handler != nullptr && !keepInputTable) {
        applyResultTableSchema(*handler, result.m_taskType);
    }
    m_historyPanel->appendEntries(
        SageWorkflowHistory::buildEntries(runningInputPath, result.m_response, success, QDateTime::currentDateTime()));
    if (handler != nullptr && !keepInputTable) {
        setResultTableRows(*handler, rows);
    }

    if (handler != nullptr) {
        selectTabKind(handler->hasInputTable() ? SageWorkflowTabKind::Input : SageWorkflowTabKind::DocumentResult);
        refreshVisibility();
        const std::optional<QString> completedMessage = handler->generateCompletedMessage();
        if (result.m_taskType == SageTaskType::Generate && success && completedMessage.has_value()) {
            SageMessageBoxDlg completedDialog(SageMessageIcon::Info, *completedMessage, this);
            completedDialog.exec();
        }
    }
    const int resultCount =
        keepInputTable ? m_inputPanel->inputTablePanel().checkedRowCount() : static_cast<int>(rows.size());
    applyStatusCardResult(handler, result.m_taskType, result.m_response, success, resultCount);
    emit statusChanged(success ? SAGE_UI_COMPLETED : SAGE_UI_FAILED);
    setRunningState(false);
}

void SageWorkspacePanel::onInputPanelOpenOutputFolderRequested()
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
    connect(m_taskTabs, &QTabBar::currentChanged, this, &SageWorkspacePanel::onTaskTabsCurrentChanged);
    connect(m_inputPanel, &SageWorkflowInputPanel::runRequested, this, &SageWorkspacePanel::onInputPanelRunRequested);
    connect(m_controller, &SageWorkflowController::runFinished, this, &SageWorkspacePanel::onControllerRunFinished);
    connect(m_inputPanel, &SageWorkflowInputPanel::openOutputFolderRequested, this,
            &SageWorkspacePanel::onInputPanelOpenOutputFolderRequested);
    connect(m_inputPanel, &SageWorkflowInputPanel::inputResetRequested, this,
            &SageWorkspacePanel::onInputPanelInputResetRequested);
    for (SageResultTablePanel* resultTablePanel :
         {&m_inputPanel->inputTablePanel(), &m_resultPanel->resultTablePanel()}) {
        connect(resultTablePanel, &SageResultTablePanel::filterChanged, this,
                &SageWorkspacePanel::onTablePanelFilterChanged);
        connect(resultTablePanel, &SageResultTablePanel::selectionChanged, this,
                &SageWorkspacePanel::onTablePanelSelectionChanged);
    }
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
    const ISageWorkflowHandler* handler = m_registry.findHandler(*m_currentWorkflow);
    state.m_checkedRowNums.clear();
    if (handler == nullptr) {
        return;
    }
    const SageResultTablePanel& resultTablePanel = resultTableFor(*handler);
    state.m_filterKeyword = resultTablePanel.filterKeyword();
    state.m_filterCriteria = resultTablePanel.filterCriteria();
    if (isInputTableVisible(*handler)) {
        state.m_checkedRowNums = resultTablePanel.checkedRowNums();
    }
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
    refreshVisibility();
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
    updateActionButtonState();
    refreshVisibility();
    if (running) {
        emit statusChanged(SAGE_UI_RUNNING);
    }
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
    m_inputPanel->setStatusResult(true, message, outputPath);
}

void SageWorkspacePanel::onTablePanelFilterChanged()
{
    const ISageWorkflowHandler* handler = findCurrentHandler();
    if (handler == nullptr) {
        return;
    }
    updateResultSummary(*handler);
    updateActionButtonState();
}

void SageWorkspacePanel::onTablePanelSelectionChanged(int selectedCount)
{
    applyActionButtonState(selectedCount);
}

void SageWorkspacePanel::onInputPanelInputResetRequested()
{
    const ISageWorkflowHandler* handler = findCurrentHandler();
    if (handler == nullptr || !handler->hasInputTable()) {
        return;
    }
    SageResultTablePanel& inputTablePanel = m_inputPanel->inputTablePanel();
    m_inputPanel->setInputPath(QString());
    m_inputPanel->resetStatusCard();
    inputTablePanel.restoreFilter(QString(), inputTablePanel.filterCriteria());
    inputTablePanel.clearRows();
    m_controller->clearResult();
    applyResultTableSchema(*handler, SageTaskType::Generate);
    refreshVisibility();
    emit statusChanged(SAGE_UI_READY);
}

const ISageWorkflowHandler* SageWorkspacePanel::findCurrentHandler() const
{
    if (!m_currentWorkflow.has_value()) {
        return nullptr;
    }
    return m_registry.findHandler(*m_currentWorkflow);
}

SageResultTablePanel& SageWorkspacePanel::resultTableFor(const ISageWorkflowHandler& handler) const
{
    return handler.hasInputTable() ? m_inputPanel->inputTablePanel() : m_resultPanel->resultTablePanel();
}

bool SageWorkspacePanel::isLastResultOf(const ISageWorkflowHandler& handler) const
{
    const SageWorkflowResultState& resultState = m_controller->resultState();
    return resultState.m_workflowType == handler.workflowType() && resultState.m_taskType.has_value();
}

bool SageWorkspacePanel::isInputTableVisible(const ISageWorkflowHandler& handler) const
{
    return handler.hasInputTable() && isResultFilterVisible(handler);
}

bool SageWorkspacePanel::isResultFilterVisible(const ISageWorkflowHandler& handler) const
{
    return isLastResultOf(handler) && handler.hasCustomResultTable(*m_controller->resultState().m_taskType);
}

void SageWorkspacePanel::refreshVisibility()
{
    const ISageWorkflowHandler* handler = findCurrentHandler();
    const bool inputTabSelected = m_selectedTabKind == SageWorkflowTabKind::Input;
    const bool inputTableVisible = handler != nullptr && isInputTableVisible(*handler);
    const bool filterVisible = handler != nullptr && isResultFilterVisible(*handler);
    m_inputPanel->setInputResetVisible(!m_controller->isRunning() && inputTabSelected && inputTableVisible);
    m_inputPanel->setInputTableVisible(inputTabSelected && inputTableVisible, filterVisible);
    m_resultPanel->setFilterVisible(m_selectedTabKind == SageWorkflowTabKind::DocumentResult && filterVisible);
}

void SageWorkspacePanel::rebuildResultTable(const ISageWorkflowHandler& handler, const QString& checkedRowNums)
{
    applyResultTableSchema(handler, m_controller->resultState().m_taskType.value_or(SageTaskType::Generate));
    if (isResultFilterVisible(handler)) {
        const SageWorkflowResultState& resultState = m_controller->resultState();
        QList<SageResultRow> rows;
        SageWorkflowResultPresenter().buildRows(&handler, *resultState.m_taskType, resultState.m_response, rows);
        setResultTableRows(handler, rows);
        if (isInputTableVisible(handler)) {
            resultTableFor(handler).restoreCheckedRowNums(checkedRowNums);
        }
    }
    updateActionButtonState();
    refreshVisibility();
}

void SageWorkspacePanel::applyResultTableSchema(const ISageWorkflowHandler& handler, SageTaskType taskType)
{
    SageResultTablePanel& resultTablePanel = resultTableFor(handler);
    resultTablePanel.setColumns(handler.resultColumns(taskType), handler.resultStyle(taskType));
    resultTablePanel.setFilterCriteria(handler.filterCriteria());
}

void SageWorkspacePanel::setResultTableRows(const ISageWorkflowHandler& handler, const QList<SageResultRow>& rows)
{
    resultTableFor(handler).setRows(rows);
    updateResultSummary(handler);
    updateActionButtonState();
}

void SageWorkspacePanel::updateResultSummary(const ISageWorkflowHandler& handler)
{
    SageResultTablePanel& resultTablePanel = resultTableFor(handler);
    const SageWorkflowResultState& resultState = m_controller->resultState();
    QList<SageResultSummaryItem> items;
    if (!isLastResultOf(handler) || !handler.buildResultSummary(*resultState.m_taskType, resultTablePanel.visibleRows(),
                                                                resultState.m_response, items)) {
        resultTablePanel.clearSummary();
        resultTablePanel.clearTotals();
        return;
    }
    resultTablePanel.setSummaryItems(items);
    QList<SageResultTotalCell> cells;
    if (!handler.buildResultTotals(*resultState.m_taskType, resultTablePanel.visibleRows(), cells)) {
        resultTablePanel.clearTotals();
        return;
    }
    resultTablePanel.setTotalCells(cells);
}

void SageWorkspacePanel::updateActionButtonState()
{
    const ISageWorkflowHandler* handler = findCurrentHandler();
    if (handler == nullptr) {
        return;
    }
    applyActionButtonState(handler->hasInputTable() ? m_inputPanel->inputTablePanel().checkedRowCount() : 0);
}

void SageWorkspacePanel::applyActionButtonState(int selectedCount)
{
    const ISageWorkflowHandler* handler = findCurrentHandler();
    if (handler == nullptr) {
        return;
    }
    bool enabled = !m_controller->isRunning();
    if (enabled && handler->hasInputTable()) {
        enabled = selectedCount > 0;
    }
    m_inputPanel->setGenerateEnabled(enabled);
}

bool SageWorkspacePanel::buildSelectedRowNums(const ISageWorkflowHandler& handler, SageTaskType taskType,
                                              QString& outRowNums)
{
    outRowNums.clear();
    if (!handler.hasInputTable() || taskType != SageTaskType::Generate) {
        return true;
    }
    const SageResultTablePanel& inputTablePanel = m_inputPanel->inputTablePanel();
    outRowNums = inputTablePanel.checkedRowNums();
    QString selectionError;
    if (handler.validateSelectedRows(inputTablePanel.checkedRowCount(), !outRowNums.isEmpty(), selectionError)) {
        return true;
    }
    SageMessageBoxDlg errorDialog(SageMessageIcon::Warning, selectionError, this);
    errorDialog.exec();
    return false;
}
