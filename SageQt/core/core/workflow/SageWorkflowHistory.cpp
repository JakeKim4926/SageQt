#include "core/workflow/SageWorkflowHistory.h"

#include "SageDefine.h"

#include <QJsonArray>
#include <QJsonValue>

QList<SageHistoryEntry> SageWorkflowHistory::buildEntries(const QString& inputPath, const QJsonObject& response,
                                                          bool success, const QDateTime& time)
{
    const QJsonArray files = response.value(SAGE_JSON_KEY_PAYLOAD).toObject().value(SAGE_JSON_KEY_FILES).toArray();
    if (files.isEmpty()) {
        return {buildRunEntry(inputPath, response, success, time)};
    }
    QList<SageHistoryEntry> entries;
    entries.reserve(files.size());
    for (const QJsonValue& file : files) {
        entries.append(buildFileEntry(inputPath, file.toObject(), success, time));
    }
    return entries;
}

SageHistoryEntry SageWorkflowHistory::buildRunEntry(const QString& inputPath, const QJsonObject& response, bool success,
                                                    const QDateTime& time)
{
    SageHistoryEntry entry;
    entry.m_time = time;
    entry.m_inputPath = inputPath;
    entry.m_success = success;
    if (success) {
        const QJsonObject payload = response.value(SAGE_JSON_KEY_PAYLOAD).toObject();
        entry.m_outputPath = payload.value(SAGE_JSON_KEY_FILE_PATH).toString();
        if (entry.m_outputPath.isEmpty()) {
            entry.m_outputPath = payload.value(SAGE_JSON_KEY_OUTPUT_FOLDER).toString();
        }
        return entry;
    }
    const QJsonObject error = response.value(SAGE_JSON_KEY_ERROR).toObject();
    entry.m_reason = error.value(SAGE_JSON_KEY_MESSAGE).toString();
    if (entry.m_reason.isEmpty()) {
        entry.m_reason = error.value(SAGE_JSON_KEY_CODE).toString();
    }
    return entry;
}

SageHistoryEntry SageWorkflowHistory::buildFileEntry(const QString& inputPath, const QJsonObject& file, bool runSuccess,
                                                     const QDateTime& time)
{
    SageHistoryEntry entry;
    entry.m_time = time;
    entry.m_inputPath = inputPath;
    const QString status = file.value(SAGE_JSON_KEY_STATUS).toString();
    entry.m_success = status.isEmpty() ? runSuccess : status.compare(SAGE_JSON_VALUE_SUCCESS, Qt::CaseInsensitive) == 0;
    if (entry.m_success) {
        entry.m_outputPath = file.value(SAGE_JSON_KEY_FILE_PATH).toString();
        return entry;
    }
    entry.m_reason = file.value(SAGE_JSON_KEY_MESSAGE).toString();
    return entry;
}
