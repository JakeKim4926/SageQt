#include "core/workflow/SageWorkflowHistory.h"

#include "SageDefine.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QString>
#include <QTest>

class SageWorkflowHistoryTest : public QObject
{
    Q_OBJECT

private slots:
    void successUsesFilePathThenOutputFolder();
    void successWithoutOutputLeavesPathEmpty();
    void failureUsesMessageThenCode();
    void filesBecomeOneEntryEach();
    void fileStatusFallsBackToRunResult();

private:
    static QJsonObject successResponse(const QJsonObject& payload);
    static QJsonObject failureResponse(const QString& code, const QString& message);
    static const QDateTime& sampleTime();
};

const QDateTime& SageWorkflowHistoryTest::sampleTime()
{
    static const QDateTime time(QDate(2026, 10, 1), QTime(13, 25, 7));
    return time;
}

QJsonObject SageWorkflowHistoryTest::successResponse(const QJsonObject& payload)
{
    return {{SAGE_JSON_KEY_SUCCESS, true}, {SAGE_JSON_KEY_PAYLOAD, payload}};
}

QJsonObject SageWorkflowHistoryTest::failureResponse(const QString& code, const QString& message)
{
    return {{SAGE_JSON_KEY_SUCCESS, false},
            {SAGE_JSON_KEY_ERROR, QJsonObject{{SAGE_JSON_KEY_CODE, code}, {SAGE_JSON_KEY_MESSAGE, message}}}};
}

void SageWorkflowHistoryTest::successUsesFilePathThenOutputFolder()
{
    QList<SageHistoryEntry> entries =
        SageWorkflowHistory::buildEntries(QStringLiteral("C:/in.xlsx"),
                                          successResponse({{SAGE_JSON_KEY_FILE_PATH, QStringLiteral("C:/out/a.xlsx")},
                                                           {SAGE_JSON_KEY_OUTPUT_FOLDER, QStringLiteral("C:/out")}}),
                                          true, sampleTime());
    QCOMPARE(entries.size(), 1);
    QCOMPARE(entries.at(0).m_time, sampleTime());
    QCOMPARE(entries.at(0).m_inputPath, QStringLiteral("C:/in.xlsx"));
    QCOMPARE(entries.at(0).m_outputPath, QStringLiteral("C:/out/a.xlsx"));
    QVERIFY(entries.at(0).m_isSuccessful);
    QVERIFY(entries.at(0).m_reason.isEmpty());

    entries = SageWorkflowHistory::buildEntries(
        QString(), successResponse({{SAGE_JSON_KEY_OUTPUT_FOLDER, QStringLiteral("C:/out")}}), true, sampleTime());
    QCOMPARE(entries.at(0).m_outputPath, QStringLiteral("C:/out"));
}

void SageWorkflowHistoryTest::successWithoutOutputLeavesPathEmpty()
{
    const QList<SageHistoryEntry> entries =
        SageWorkflowHistory::buildEntries(QStringLiteral("C:/in.xlsx"), successResponse({}), true, sampleTime());
    QVERIFY(entries.at(0).m_outputPath.isEmpty());
}

void SageWorkflowHistoryTest::failureUsesMessageThenCode()
{
    QList<SageHistoryEntry> entries = SageWorkflowHistory::buildEntries(
        QStringLiteral("C:/in.xlsx"), failureResponse(QStringLiteral("E1"), QStringLiteral("사유")), false,
        sampleTime());
    QVERIFY(!entries.at(0).m_isSuccessful);
    QCOMPARE(entries.at(0).m_reason, QStringLiteral("사유"));
    QVERIFY(entries.at(0).m_outputPath.isEmpty());

    entries = SageWorkflowHistory::buildEntries(QString(), failureResponse(QStringLiteral("E1"), QString()), false,
                                                sampleTime());
    QCOMPARE(entries.at(0).m_reason, QStringLiteral("E1"));
}

void SageWorkflowHistoryTest::filesBecomeOneEntryEach()
{
    const QJsonArray files{
        QJsonObject{{SAGE_JSON_KEY_STATUS, QStringLiteral("SUCCESS")}, {SAGE_JSON_KEY_FILE_PATH, QStringLiteral("a")}},
        QJsonObject{{SAGE_JSON_KEY_STATUS, QStringLiteral("failed")},
                    {SAGE_JSON_KEY_MESSAGE, QStringLiteral("깨짐")},
                    {SAGE_JSON_KEY_CODE, QStringLiteral("E2")}},
        QJsonObject{{SAGE_JSON_KEY_STATUS, QStringLiteral("failed")}, {SAGE_JSON_KEY_CODE, QStringLiteral("E3")}},
    };
    const QList<SageHistoryEntry> entries = SageWorkflowHistory::buildEntries(
        QStringLiteral("C:/in.xlsx"), successResponse({{SAGE_JSON_KEY_FILES, files}}), true, sampleTime());
    QCOMPARE(entries.size(), 3);
    QVERIFY(entries.at(0).m_isSuccessful);
    QCOMPARE(entries.at(0).m_outputPath, QStringLiteral("a"));
    QVERIFY(!entries.at(1).m_isSuccessful);
    QCOMPARE(entries.at(1).m_reason, QStringLiteral("깨짐"));
    QVERIFY(entries.at(2).m_reason.isEmpty());
}

void SageWorkflowHistoryTest::fileStatusFallsBackToRunResult()
{
    const QJsonArray files{QJsonObject{{SAGE_JSON_KEY_FILE_PATH, QStringLiteral("a")}}};
    QList<SageHistoryEntry> entries = SageWorkflowHistory::buildEntries(
        QString(), successResponse({{SAGE_JSON_KEY_FILES, files}}), true, sampleTime());
    QVERIFY(entries.at(0).m_isSuccessful);
    entries = SageWorkflowHistory::buildEntries(QString(), successResponse({{SAGE_JSON_KEY_FILES, files}}), false,
                                                sampleTime());
    QVERIFY(!entries.at(0).m_isSuccessful);
}

QTEST_GUILESS_MAIN(SageWorkflowHistoryTest)
#include "SageWorkflowHistoryTest.moc"
