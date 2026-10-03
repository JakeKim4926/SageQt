#include "ui/workflow/SageWorkflowController.h"

#include "SageDefine.h"
#include "SageTestWorkflowHandler.h"
#include "core/workflow/SageWorkflowRegistry.h"

#include <QJsonObject>
#include <QObject>
#include <QSemaphore>
#include <QSignalSpy>
#include <QString>
#include <QTest>
#include <QThreadPool>

#include <memory>

class SageWorkflowControllerTest : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void runsInBackgroundAndRejectsSecondStart();
    void finishKeepsPreviousResultWhenAsked();
    void restoreAndClearResult();

private:
    static SageWorkflowRunResult runResult(SageTaskType taskType, const QString& requestId);

private:
    std::unique_ptr<SageWorkflowRegistry> m_registry;
    QSemaphore m_gate;
};

static const SageWorkflowType SAGE_GATED_WORKFLOW = static_cast<SageWorkflowType>(3);

void SageWorkflowControllerTest::init()
{
    m_registry = std::make_unique<SageWorkflowRegistry>();
    std::unique_ptr<SageTestWorkflowHandler> handler = std::make_unique<SageTestWorkflowHandler>(
        SAGE_GATED_WORKFLOW, QStringLiteral("대기 업무"), QStringLiteral("샘플"), false);
    QSemaphore* gate = &m_gate;
    handler->setRunTask([gate](SageTaskType, const QJsonObject& payload) {
        gate->acquire();
        QJsonObject response;
        response.insert(SAGE_JSON_KEY_SUCCESS, true);
        response.insert(SAGE_JSON_KEY_PAYLOAD, payload);
        return response;
    });
    m_registry->registerHandler(std::move(handler));
}

void SageWorkflowControllerTest::cleanup()
{
    m_gate.release();
    QThreadPool::globalInstance()->waitForDone();
    m_gate.acquire(m_gate.available());
}

SageWorkflowRunResult SageWorkflowControllerTest::runResult(SageTaskType taskType, const QString& requestId)
{
    SageWorkflowRunResult result;
    result.m_workflowType = SageWorkflowType::Sample;
    result.m_taskType = taskType;
    result.m_response.insert(SAGE_JSON_KEY_REQUEST_ID, requestId);
    return result;
}

void SageWorkflowControllerTest::runsInBackgroundAndRejectsSecondStart()
{
    SageWorkflowController controller(*m_registry);
    QSignalSpy finishedSpy(&controller, &SageWorkflowController::runFinished);
    SageWorkflowRunRequest request;
    request.m_workflowType = SAGE_GATED_WORKFLOW;
    request.m_taskType = SageTaskType::Load;
    request.m_inputPath = QStringLiteral("C:/work/in.xlsx");

    QString error;
    QVERIFY(controller.start(request, error));
    QVERIFY(controller.isRunning());
    QCOMPARE(controller.resultState().m_inputPath, QStringLiteral("C:/work/in.xlsx"));

    QVERIFY(!controller.start(request, error));
    QCOMPARE(error, SAGE_UI_WORKFLOW_ALREADY_RUNNING);
    QTest::qWait(50);
    QCOMPARE(finishedSpy.count(), 0);

    m_gate.release();
    QTRY_COMPARE(finishedSpy.count(), 1);
    const SageWorkflowRunResult result = finishedSpy.at(0).at(0).value<SageWorkflowRunResult>();
    QCOMPARE(result.m_workflowType, SAGE_GATED_WORKFLOW);
    QCOMPARE(result.m_taskType, SageTaskType::Load);
    QCOMPARE(result.m_response.value(SAGE_JSON_KEY_PAYLOAD).toObject().value(SAGE_JSON_KEY_INPUT_PATH).toString(),
             QStringLiteral("C:/work/in.xlsx"));
    QVERIFY(controller.isRunning());

    controller.finish(result, true, false);
    QVERIFY(!controller.isRunning());
    QVERIFY(controller.start(request, error));
}

void SageWorkflowControllerTest::finishKeepsPreviousResultWhenAsked()
{
    SageWorkflowController controller(*m_registry);
    controller.finish(runResult(SageTaskType::Load, QStringLiteral("load")), true, false);
    controller.finish(runResult(SageTaskType::Generate, QStringLiteral("generate")), false, true);

    const SageWorkflowResultState& state = controller.resultState();
    QCOMPARE(state.m_workflowType, SageWorkflowType::Sample);
    QCOMPARE(state.m_taskType, SageTaskType::Load);
    QVERIFY(!state.m_isSuccessful);
    QCOMPARE(state.m_response.value(SAGE_JSON_KEY_REQUEST_ID).toString(), QStringLiteral("load"));

    controller.finish(runResult(SageTaskType::Generate, QStringLiteral("generate")), true, false);
    QCOMPARE(controller.resultState().m_taskType, SageTaskType::Generate);
    QCOMPARE(controller.resultState().m_response.value(SAGE_JSON_KEY_REQUEST_ID).toString(),
             QStringLiteral("generate"));
}

void SageWorkflowControllerTest::restoreAndClearResult()
{
    SageWorkflowController controller(*m_registry);
    SageWorkflowResultState saved;
    saved.m_workflowType = SageWorkflowType::Sample;
    saved.m_taskType = SageTaskType::Generate;
    saved.m_isSuccessful = true;
    saved.m_inputPath = QStringLiteral("C:/work/in.xlsx");

    controller.restoreResult(saved);
    QCOMPARE(controller.resultState().m_taskType, SageTaskType::Generate);
    QCOMPARE(controller.resultState().m_inputPath, QStringLiteral("C:/work/in.xlsx"));

    controller.clearResult();
    QVERIFY(!controller.resultState().m_workflowType.has_value());
    QVERIFY(!controller.resultState().m_taskType.has_value());
    QVERIFY(!controller.resultState().m_isSuccessful);
    QVERIFY(controller.resultState().m_inputPath.isEmpty());
}

QTEST_GUILESS_MAIN(SageWorkflowControllerTest)
#include "SageWorkflowControllerTest.moc"
