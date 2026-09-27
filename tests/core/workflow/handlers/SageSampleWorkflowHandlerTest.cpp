#include "core/workflow/handlers/SageSampleWorkflowHandler.h"

#include "SageDefine.h"

#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QString>
#include <QTest>

class SageSampleWorkflowHandlerTest : public QObject
{
    Q_OBJECT

private slots:
    void describesSampleWorkflow();
    void hasThreeTabsInVisualOrder();
    void usesGenericResultColumns();
    void runTaskUsesGivenOutputFolder();
    void runTaskFallsBackToInputFolder();
};

void SageSampleWorkflowHandlerTest::describesSampleWorkflow()
{
    const SageSampleWorkflowHandler handler;

    QCOMPARE(handler.workflowType(), SageWorkflowType::Sample);
    QCOMPARE(handler.sidebarLabel(), QStringLiteral("샘플 업무"));
    QCOMPARE(handler.category(), QStringLiteral("샘플"));
    QCOMPARE(handler.headerTitle(), QStringLiteral("샘플 업무"));
    QCOMPARE(handler.inputSectionLabel(), QStringLiteral("입력 파일"));
    QCOMPARE(handler.actionButtonLabel(), QStringLiteral("실행"));
    QCOMPARE(handler.inputDialogTitle(), QStringLiteral("샘플 입력 파일 선택"));
    QCOMPARE(handler.generateCompletedMessage(), std::optional<QString>(QStringLiteral("샘플 업무가 완료되었습니다.")));
    QCOMPARE(handler.requestId(SageTaskType::Generate), QStringLiteral("sample-run"));
    QCOMPARE(handler.isLoginRequired(), false);
    QCOMPARE(handler.hasInputTable(), false);
    QCOMPARE(handler.hasCustomResultTable(SageTaskType::Generate), false);
    QVERIFY(handler.filterCriteria().isEmpty());

    QString error;
    QCOMPARE(handler.validateSelectedRows(0, false, error), true);
    QVERIFY(error.isEmpty());
}

void SageSampleWorkflowHandlerTest::hasThreeTabsInVisualOrder()
{
    const SageSampleWorkflowHandler handler;

    const QList<SageWorkflowTab> tabs = handler.tabs();

    QCOMPARE(tabs.size(), 3);
    QCOMPARE(tabs.at(0).m_kind, SageWorkflowTabKind::Input);
    QCOMPARE(tabs.at(0).m_label, QStringLiteral("입력"));
    QCOMPARE(tabs.at(1).m_kind, SageWorkflowTabKind::DocumentResult);
    QCOMPARE(tabs.at(1).m_label, QStringLiteral("결과"));
    QCOMPARE(tabs.at(2).m_kind, SageWorkflowTabKind::DocumentHistory);
    QCOMPARE(tabs.at(2).m_label, QStringLiteral("실행 기록"));
}

void SageSampleWorkflowHandlerTest::usesGenericResultColumns()
{
    const SageSampleWorkflowHandler handler;

    const QList<SageWorkflowColumn> columns = handler.resultColumns(SageTaskType::Generate);

    QCOMPARE(columns.size(), 4);
    QCOMPARE(columns.at(0).m_label, QStringLiteral("항목"));
    QCOMPARE(columns.at(1).m_label, QStringLiteral("값"));
    QCOMPARE(columns.at(2).m_label, QStringLiteral("상태"));
    QCOMPARE(columns.at(3).m_label, QStringLiteral("사유"));
    for (const SageWorkflowColumn& column : columns) {
        QCOMPARE(column.m_align, SageColumnAlign::Center);
        QCOMPARE(column.m_isStretch, column.m_field == SageResultField::Value);
    }
}

void SageSampleWorkflowHandlerTest::runTaskUsesGivenOutputFolder()
{
    const SageSampleWorkflowHandler handler;
    QJsonObject payload;
    payload.insert(QStringLiteral("inputPath"), QStringLiteral("/data/in/input.xlsx"));
    payload.insert(QStringLiteral("outputFolder"), QStringLiteral("/data/out"));

    const QJsonObject response = handler.runTask(SageTaskType::Generate, payload);

    QCOMPARE(response.value(QStringLiteral("success")).toBool(), true);
    QCOMPARE(response.value(QStringLiteral("requestId")).toString(), QStringLiteral("sample-run"));
    const QJsonObject result = response.value(QStringLiteral("payload")).toObject();
    QCOMPARE(result.size(), 6);
    QCOMPARE(result.value(QStringLiteral("status")).toString(), QStringLiteral("완료"));
    QCOMPARE(result.value(QStringLiteral("fileName")).toString(), QStringLiteral("input.xlsx"));
    QCOMPARE(result.value(QStringLiteral("outputFolder")).toString(), QStringLiteral("/data/out"));
    QCOMPARE(result.value(QStringLiteral("totalFiles")).toInt(), 1);
    QCOMPARE(result.value(QStringLiteral("passedFiles")).toInt(), 1);
    QCOMPARE(result.value(QStringLiteral("failedFiles")).toInt(), 0);
}

void SageSampleWorkflowHandlerTest::runTaskFallsBackToInputFolder()
{
    const SageSampleWorkflowHandler handler;
    QJsonObject payload;
    payload.insert(QStringLiteral("inputPath"), QStringLiteral("/data/in/input.xlsx"));

    const QJsonObject response = handler.runTask(SageTaskType::Generate, payload);

    const QJsonObject result = response.value(QStringLiteral("payload")).toObject();
    QCOMPARE(result.value(QStringLiteral("outputFolder")).toString(), QStringLiteral("/data/in"));
}

QTEST_GUILESS_MAIN(SageSampleWorkflowHandlerTest)

#include "SageSampleWorkflowHandlerTest.moc"
