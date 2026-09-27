#include "core/workflow/SageWorkflowResponse.h"

#include "SageDefine.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QTest>

class SageWorkflowResponseTest : public QObject
{
    Q_OBJECT

private slots:
    void successContainsPayloadAndNullError();
    void successKeepsEmptyPayloadAsObject();
    void failureContainsErrorAndNullPayload();
};

void SageWorkflowResponseTest::successContainsPayloadAndNullError()
{
    QJsonObject payload;
    payload.insert(QStringLiteral("fileName"), QStringLiteral("input.xlsx"));

    const QJsonObject response = SageWorkflowResponse::success(QStringLiteral("sample-run"), payload);

    QCOMPARE(response.size(), 5);
    QCOMPARE(response.value(SAGE_JSON_KEY_TYPE).toString(), QStringLiteral("response"));
    QCOMPARE(response.value(SAGE_JSON_KEY_REQUEST_ID).toString(), QStringLiteral("sample-run"));
    QCOMPARE(response.value(SAGE_JSON_KEY_SUCCESS).toBool(), true);
    QCOMPARE(response.value(SAGE_JSON_KEY_PAYLOAD).toObject(), payload);
    QVERIFY(response.value(SAGE_JSON_KEY_ERROR).isNull());
}

void SageWorkflowResponseTest::successKeepsEmptyPayloadAsObject()
{
    const QJsonObject response = SageWorkflowResponse::success(QStringLiteral("sample-run"), QJsonObject());

    QVERIFY(response.value(SAGE_JSON_KEY_PAYLOAD).isObject());
    QVERIFY(response.value(SAGE_JSON_KEY_PAYLOAD).toObject().isEmpty());
}

void SageWorkflowResponseTest::failureContainsErrorAndNullPayload()
{
    const QJsonObject response =
        SageWorkflowResponse::failure(QStringLiteral("unknown"), QStringLiteral("E001"), QStringLiteral("boom"));

    QCOMPARE(response.size(), 5);
    QCOMPARE(response.value(SAGE_JSON_KEY_TYPE).toString(), QStringLiteral("response"));
    QCOMPARE(response.value(SAGE_JSON_KEY_REQUEST_ID).toString(), QStringLiteral("unknown"));
    QCOMPARE(response.value(SAGE_JSON_KEY_SUCCESS).toBool(), false);
    QVERIFY(response.value(SAGE_JSON_KEY_PAYLOAD).isNull());

    const QJsonObject error = response.value(SAGE_JSON_KEY_ERROR).toObject();
    QCOMPARE(error.size(), 2);
    QCOMPARE(error.value(SAGE_JSON_KEY_CODE).toString(), QStringLiteral("E001"));
    QCOMPARE(error.value(SAGE_JSON_KEY_MESSAGE).toString(), QStringLiteral("boom"));
}

QTEST_GUILESS_MAIN(SageWorkflowResponseTest)

#include "SageWorkflowResponseTest.moc"
