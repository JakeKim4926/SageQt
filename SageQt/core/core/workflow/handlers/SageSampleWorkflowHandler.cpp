#include "core/workflow/handlers/SageSampleWorkflowHandler.h"

#include "SageDefine.h"
#include "core/workflow/SageWorkflowResponse.h"

#include <QFileInfo>

SageWorkflowType SageSampleWorkflowHandler::workflowType() const
{
    return SageWorkflowType::Sample;
}

QString SageSampleWorkflowHandler::sidebarLabel() const
{
    return SAGE_UI_SAMPLE_NAME;
}

QString SageSampleWorkflowHandler::category() const
{
    return SAGE_UI_SAMPLE_CATEGORY;
}

QString SageSampleWorkflowHandler::headerTitle() const
{
    return SAGE_UI_SAMPLE_NAME;
}

QString SageSampleWorkflowHandler::inputSectionLabel() const
{
    return SAGE_UI_SECTION_INPUT;
}

QString SageSampleWorkflowHandler::actionButtonLabel() const
{
    return SAGE_UI_SAMPLE_ACTION_BUTTON;
}

QList<SageWorkflowTab> SageSampleWorkflowHandler::tabs() const
{
    return {
        {SageWorkflowTabKind::Input, SAGE_UI_TAB_INPUT},
        {SageWorkflowTabKind::DocumentResult, SAGE_UI_TAB_RESULT},
        {SageWorkflowTabKind::DocumentHistory, SAGE_UI_TAB_HISTORY},
    };
}

QList<SageWorkflowColumn> SageSampleWorkflowHandler::resultColumns(SageTaskType) const
{
    return SageWorkflowResultTable::genericColumns();
}

SageWorkflowResultStyle SageSampleWorkflowHandler::resultStyle(SageTaskType) const
{
    return {};
}

bool SageSampleWorkflowHandler::hasCustomResultTable(SageTaskType) const
{
    return false;
}

bool SageSampleWorkflowHandler::buildResultSummary(SageTaskType, const QList<SageResultRow>&, const QJsonObject&,
                                                   QList<SageResultSummaryItem>&) const
{
    return false;
}

bool SageSampleWorkflowHandler::buildResultTotals(SageTaskType, const QList<SageResultRow>&,
                                                  QList<SageResultTotalCell>&) const
{
    return false;
}

QList<SageWorkflowFilterCriteria> SageSampleWorkflowHandler::filterCriteria() const
{
    return {};
}

QString SageSampleWorkflowHandler::inputDialogTitle() const
{
    return SAGE_UI_SAMPLE_INPUT_DIALOG_TITLE;
}

bool SageSampleWorkflowHandler::hasInputTable() const
{
    return false;
}

std::optional<QString> SageSampleWorkflowHandler::generateCompletedMessage() const
{
    return SAGE_UI_SAMPLE_COMPLETED;
}

bool SageSampleWorkflowHandler::validateSelectedRows(int, bool, QString&) const
{
    return true;
}

bool SageSampleWorkflowHandler::isLoginRequired() const
{
    return false;
}

QString SageSampleWorkflowHandler::requestId(SageTaskType) const
{
    return SAGE_REQUEST_SAMPLE_RUN;
}

QJsonObject SageSampleWorkflowHandler::runTask(SageTaskType, const QJsonObject& payload) const
{
    const QFileInfo inputFile(payload.value(SAGE_JSON_KEY_INPUT_PATH).toString());
    QString outputFolder = payload.value(SAGE_JSON_KEY_OUTPUT_FOLDER).toString();
    if (outputFolder.isEmpty()) {
        outputFolder = inputFile.path();
    }

    QJsonObject result;
    result.insert(SAGE_JSON_KEY_STATUS, SAGE_UI_SAMPLE_STATUS_DONE);
    result.insert(SAGE_JSON_KEY_FILE_NAME, inputFile.fileName());
    result.insert(SAGE_JSON_KEY_OUTPUT_FOLDER, outputFolder);
    result.insert(SAGE_JSON_KEY_TOTAL_FILES, SAGE_SAMPLE_TOTAL_FILES);
    result.insert(SAGE_JSON_KEY_PASSED_FILES, SAGE_SAMPLE_PASSED_FILES);
    result.insert(SAGE_JSON_KEY_FAILED_FILES, SAGE_SAMPLE_FAILED_FILES);
    return SageWorkflowResponse::success(SAGE_REQUEST_SAMPLE_RUN, result);
}

bool SageSampleWorkflowHandler::buildResultRows(SageTaskType, const QJsonObject&, QList<SageResultRow>&) const
{
    return false;
}
