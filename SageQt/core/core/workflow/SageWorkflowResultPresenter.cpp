#include "core/workflow/SageWorkflowResultPresenter.h"

#include "core/workflow/ISageWorkflowHandler.h"

#include <QJsonValue>

bool SageWorkflowResultPresenter::buildRows(const ISageWorkflowHandler* handler, SageTaskType taskType,
                                            const QJsonObject& response, QList<SageResultRow>& outRows) const
{
    outRows.clear();

    if (!response.value(SAGE_JSON_KEY_SUCCESS).toBool()) {
        const QJsonObject error = response.value(SAGE_JSON_KEY_ERROR).toObject();
        addRow(outRows, SAGE_UI_RESULT_STATUS, SAGE_UI_FAILED, SAGE_RESULT_STATUS_FAILED, QString());
        addRow(outRows, SAGE_UI_RESULT_ERROR, error.value(SAGE_JSON_KEY_CODE).toString(), SAGE_RESULT_STATUS_ERROR,
               error.value(SAGE_JSON_KEY_MESSAGE).toString());
        return false;
    }

    if (handler != nullptr && handler->buildResultRows(taskType, response, outRows)) {
        return true;
    }

    addRow(outRows, SAGE_UI_RESULT_STATUS, SAGE_UI_COMPLETED, SAGE_RESULT_STATUS_SUCCESS, QString());
    addSummaryRows(response.value(SAGE_JSON_KEY_PAYLOAD).toObject(), outRows);
    return true;
}

void SageWorkflowResultPresenter::addRow(QList<SageResultRow>& outRows, const QString& field, const QString& value,
                                         const QString& status, const QString& reason)
{
    SageResultRow row;
    row.m_field = field;
    row.m_value = value;
    row.m_status = status;
    row.m_reason = reason;
    outRows.append(row);
}

void SageWorkflowResultPresenter::addSummaryRows(const QJsonObject& payload, QList<SageResultRow>& outRows)
{
    const QString status = payload.value(SAGE_JSON_KEY_STATUS).toString();
    const QString fileName = payload.value(SAGE_JSON_KEY_FILE_NAME).toString();
    const QString folder = payload.value(SAGE_JSON_KEY_OUTPUT_FOLDER).toString();
    const QString total = integerText(payload, SAGE_JSON_KEY_TOTAL_FILES);
    const QString passed = integerText(payload, SAGE_JSON_KEY_PASSED_FILES);
    const QString failed = integerText(payload, SAGE_JSON_KEY_FAILED_FILES);

    if (!status.isEmpty()) {
        addRow(outRows, SAGE_UI_RESULT_RESULT_LABEL, status, status, QString());
    }
    if (!total.isEmpty()) {
        addRow(outRows, SAGE_UI_RESULT_TOTAL_LABEL, total, SAGE_RESULT_STATUS_SUMMARY,
               SAGE_UI_RESULT_PASSED_PREFIX + passed + SAGE_UI_RESULT_FAILED_SUFFIX + failed);
    }
    if (!fileName.isEmpty()) {
        addRow(outRows, SAGE_UI_RESULT_FILE, fileName, SAGE_RESULT_STATUS_OUTPUT, QString());
    }
    if (!folder.isEmpty()) {
        addRow(outRows, SAGE_UI_RESULT_FOLDER, folder, SAGE_RESULT_STATUS_OUTPUT, QString());
    }
}

QString SageWorkflowResultPresenter::integerText(const QJsonObject& object, const QString& key)
{
    const QJsonValue value = object.value(key);
    if (!value.isDouble()) {
        return QString();
    }
    return QString::number(value.toInteger());
}
