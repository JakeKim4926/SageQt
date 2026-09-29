#include "core/workflow/SageWorkflowRunner.h"

#include "SageDefine.h"
#include "SageTestWorkflowHandler.h"
#include "core/workflow/SageWorkflowRegistry.h"

#include <QJsonObject>
#include <QObject>
#include <QTest>

#include <memory>
#include <stdexcept>

class SageWorkflowRunnerTest : public QObject
{
    Q_OBJECT

private slots:
    void payloadOmitsEmptyOptionalKeys();
    void payloadKeepsGivenOptionalKeys();
    void runPassesPayloadToHandler();
    void missingHandlerReturnsNotFound();
    void throwingHandlerReturnsException();
};

static const SageWorkflowType SAGE_TEST_WORKFLOW = static_cast<SageWorkflowType>(2);
static const SageWorkflowType SAGE_UNKNOWN_WORKFLOW = static_cast<SageWorkflowType>(99);

void SageWorkflowRunnerTest::payloadOmitsEmptyOptionalKeys()
{
    const QJsonObject payload =
        SageWorkflowRunner::buildPayload(QStringLiteral("C:/work/in.xlsx"), QString(), QString());

    QCOMPARE(payload.size(), 1);
    QCOMPARE(payload.value(SAGE_JSON_KEY_INPUT_PATH).toString(), QStringLiteral("C:/work/in.xlsx"));
    QVERIFY(!payload.contains(SAGE_JSON_KEY_OUTPUT_FOLDER));
    QVERIFY(!payload.contains(SAGE_JSON_KEY_ROW_NUMS));
}

void SageWorkflowRunnerTest::payloadKeepsGivenOptionalKeys()
{
    const QJsonObject payload = SageWorkflowRunner::buildPayload(QStringLiteral("C:/work/in.xlsx"),
                                                                 QStringLiteral("C:/work/out"), QStringLiteral("2,5"));

    QCOMPARE(payload.size(), 3);
    QCOMPARE(payload.value(SAGE_JSON_KEY_OUTPUT_FOLDER).toString(), QStringLiteral("C:/work/out"));
    QCOMPARE(payload.value(SAGE_JSON_KEY_ROW_NUMS).toString(), QStringLiteral("2,5"));
}

void SageWorkflowRunnerTest::runPassesPayloadToHandler()
{
    const SageWorkflowRegistry registry;
    SageWorkflowRunRequest request;
    request.m_workflowType = SageWorkflowType::Sample;
    request.m_taskType = SageTaskType::Generate;
    request.m_inputPath = QStringLiteral("C:/work/in.xlsx");

    const SageWorkflowRunResult result = SageWorkflowRunner::run(registry, request);

    QCOMPARE(result.m_workflowType, SageWorkflowType::Sample);
    QCOMPARE(result.m_taskType, SageTaskType::Generate);
    QVERIFY(result.m_response.value(SAGE_JSON_KEY_SUCCESS).toBool());
    const QJsonObject payload = result.m_response.value(SAGE_JSON_KEY_PAYLOAD).toObject();
    QCOMPARE(payload.value(SAGE_JSON_KEY_OUTPUT_FOLDER).toString(), QStringLiteral("C:/work"));
}

void SageWorkflowRunnerTest::missingHandlerReturnsNotFound()
{
    const SageWorkflowRegistry registry;
    SageWorkflowRunRequest request;
    request.m_workflowType = SAGE_UNKNOWN_WORKFLOW;

    const SageWorkflowRunResult result = SageWorkflowRunner::run(registry, request);

    QCOMPARE(result.m_workflowType, SAGE_UNKNOWN_WORKFLOW);
    QVERIFY(!result.m_response.value(SAGE_JSON_KEY_SUCCESS).toBool());
    QCOMPARE(result.m_response.value(SAGE_JSON_KEY_REQUEST_ID).toString(), SAGE_REQUEST_UNKNOWN);
    const QJsonObject error = result.m_response.value(SAGE_JSON_KEY_ERROR).toObject();
    QCOMPARE(error.value(SAGE_JSON_KEY_CODE).toString(), SAGE_ERROR_CODE_WORKFLOW_NOT_FOUND);
    QCOMPARE(error.value(SAGE_JSON_KEY_MESSAGE).toString(), SAGE_UI_WORKFLOW_NOT_FOUND);
}

void SageWorkflowRunnerTest::throwingHandlerReturnsException()
{
    SageWorkflowRegistry registry;
    std::unique_ptr<SageTestWorkflowHandler> handler = std::make_unique<SageTestWorkflowHandler>(
        SAGE_TEST_WORKFLOW, QStringLiteral("테스트 업무"), QStringLiteral("샘플"), false);
    handler->setRunTask(
        [](SageTaskType, const QJsonObject&) -> QJsonObject { throw std::runtime_error("handler failure"); });
    registry.registerHandler(std::move(handler));
    SageWorkflowRunRequest request;
    request.m_workflowType = SAGE_TEST_WORKFLOW;

    const SageWorkflowRunResult result = SageWorkflowRunner::run(registry, request);

    QVERIFY(!result.m_response.value(SAGE_JSON_KEY_SUCCESS).toBool());
    QCOMPARE(result.m_response.value(SAGE_JSON_KEY_REQUEST_ID).toString(), SAGE_REQUEST_SAMPLE_RUN);
    const QJsonObject error = result.m_response.value(SAGE_JSON_KEY_ERROR).toObject();
    QCOMPARE(error.value(SAGE_JSON_KEY_CODE).toString(), SAGE_ERROR_CODE_WORKFLOW_EXCEPTION);
    QCOMPARE(error.value(SAGE_JSON_KEY_MESSAGE).toString(), SAGE_UI_WORKFLOW_EXCEPTION);
}

QTEST_GUILESS_MAIN(SageWorkflowRunnerTest)
#include "SageWorkflowRunnerTest.moc"
