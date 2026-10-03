#pragma once

#include <QDateTime>
#include <QJsonObject>
#include <QList>
#include <QString>

struct SageHistoryEntry
{
    QDateTime m_time;
    QString m_inputPath;
    QString m_outputPath;
    QString m_reason;
    bool m_isSuccessful = false;
};

class SageWorkflowHistory
{
public:
    static QList<SageHistoryEntry> buildEntries(const QString& inputPath, const QJsonObject& response, bool success,
                                                const QDateTime& time);

private:
    static SageHistoryEntry buildRunEntry(const QString& inputPath, const QJsonObject& response, bool success,
                                          const QDateTime& time);
    static SageHistoryEntry buildFileEntry(const QString& inputPath, const QJsonObject& file, bool runSuccess,
                                           const QDateTime& time);
};
