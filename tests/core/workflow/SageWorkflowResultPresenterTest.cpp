#include "core/workflow/SageWorkflowResultPresenter.h"

#include "SageDefine.h"
#include "core/workflow/ISageWorkflowHandler.h"
#include "core/workflow/SageWorkflowResponse.h"
#include "core/workflow/handlers/SageSampleWorkflowHandler.h"

#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QString>
#include <QTest>

namespace {

class SageFixedRowsWorkflowHandler final : public ISageWorkflowHandler
{
public:
    SageWorkflowType workflowType() const override
    {
        return SageWorkflowType::Sample;
    }

    QString sidebarLabel() const override
    {
        return {};
    }

    QString category() const override
    {
        return {};
    }

    QString headerTitle() const override
    {
        return {};
    }

    QString inputSectionLabel() const override
    {
        return {};
    }

    QString actionButtonLabel() const override
    {
        return {};
    }

    QList<SageWorkflowTab> tabs() const override
    {
        return {};
    }

    QList<SageWorkflowColumn> resultColumns(SageTaskType) const override
    {
        return {};
    }

    SageWorkflowResultStyle resultStyle(SageTaskType) const override
    {
        return {};
    }

    bool hasCustomResultTable(SageTaskType) const override
    {
        return false;
    }

    bool buildResultSummary(SageTaskType, const QList<SageResultRow>&, const QJsonObject&,
                            QList<SageResultSummaryItem>&) const override
    {
        return false;
    }

    bool buildResultTotals(SageTaskType, const QList<SageResultRow>&, QList<SageResultTotalCell>&) const override
    {
        return false;
    }

    QList<SageWorkflowFilterCriteria> filterCriteria() const override
    {
        return {};
    }

    QString inputDialogTitle() const override
    {
        return {};
    }

    QString inputFileFilter() const override
    {
        return {};
    }

    bool hasInputTable() const override
    {
        return false;
    }

    std::optional<QString> generateCompletedMessage() const override
    {
        return std::nullopt;
    }

    bool validateSelectedRows(int, bool, QString&) const override
    {
        return true;
    }

    bool isLoginRequired() const override
    {
        return false;
    }

    QString requestId(SageTaskType) const override
    {
        return {};
    }

    QJsonObject runTask(SageTaskType, const QJsonObject&) const override
    {
        return {};
    }

    bool buildResultRows(SageTaskType, const QJsonObject&, QList<SageResultRow>& outRows) const override
    {
        SageResultRow row;
        row.m_field = QStringLiteral("custom-field");
        row.m_value = QStringLiteral("custom-value");
        outRows.append(row);
        return true;
    }
};

QJsonObject successResponse(const QJsonObject& payload)
{
    return SageWorkflowResponse::success(QStringLiteral("sample-run"), payload);
}

}

class SageWorkflowResultPresenterTest : public QObject
{
    Q_OBJECT

private slots:
    void buildRowsReturnsFailureRowsWhenSuccessIsFalse();
    void buildRowsTreatsMissingSuccessAsFailure();
    void buildRowsUsesHandlerRowsWhenHandlerBuildsThem();
    void buildRowsAddsSummaryRowsInOrder();
    void buildRowsSkipsMissingSummaryKeys();
    void buildRowsKeepsZeroTotalWithMissingCounts();
    void buildRowsIgnoresSummaryKeysOutsidePayload();
    void buildRowsWithoutHandlerUsesGenericRows();
    void buildRowsFromSampleHandlerResponse();

private:
    static void compareRow(const SageResultRow& row, const QString& field, const QString& value, const QString& status,
                           const QString& reason);
};

void SageWorkflowResultPresenterTest::compareRow(const SageResultRow& row, const QString& field, const QString& value,
                                                 const QString& status, const QString& reason)
{
    QCOMPARE(row.m_field, field);
    QCOMPARE(row.m_value, value);
    QCOMPARE(row.m_status, status);
    QCOMPARE(row.m_reason, reason);
}

void SageWorkflowResultPresenterTest::buildRowsReturnsFailureRowsWhenSuccessIsFalse()
{
    const SageSampleWorkflowHandler handler;
    const SageWorkflowResultPresenter presenter;
    const QJsonObject response =
        SageWorkflowResponse::failure(QStringLiteral("sample-run"), QStringLiteral("E001"), QStringLiteral("boom"));
    QList<SageResultRow> rows;

    QCOMPARE(presenter.buildRows(&handler, SageTaskType::Generate, response, rows), false);

    QCOMPARE(rows.size(), 2);
    compareRow(rows.at(0), SAGE_UI_RESULT_STATUS, SAGE_UI_FAILED, SAGE_RESULT_STATUS_FAILED, QString());
    compareRow(rows.at(1), SAGE_UI_RESULT_ERROR, QStringLiteral("E001"), SAGE_RESULT_STATUS_ERROR,
               QStringLiteral("boom"));
}

void SageWorkflowResultPresenterTest::buildRowsTreatsMissingSuccessAsFailure()
{
    const SageWorkflowResultPresenter presenter;
    QList<SageResultRow> rows;

    QCOMPARE(presenter.buildRows(nullptr, SageTaskType::Generate, QJsonObject(), rows), false);

    QCOMPARE(rows.size(), 2);
    compareRow(rows.at(0), SAGE_UI_RESULT_STATUS, SAGE_UI_FAILED, SAGE_RESULT_STATUS_FAILED, QString());
    compareRow(rows.at(1), SAGE_UI_RESULT_ERROR, QString(), SAGE_RESULT_STATUS_ERROR, QString());
}

void SageWorkflowResultPresenterTest::buildRowsUsesHandlerRowsWhenHandlerBuildsThem()
{
    const SageFixedRowsWorkflowHandler handler;
    const SageWorkflowResultPresenter presenter;
    QList<SageResultRow> rows;

    QCOMPARE(presenter.buildRows(&handler, SageTaskType::Generate, successResponse(QJsonObject()), rows), true);

    QCOMPARE(rows.size(), 1);
    compareRow(rows.at(0), QStringLiteral("custom-field"), QStringLiteral("custom-value"), QString(), QString());
}

void SageWorkflowResultPresenterTest::buildRowsAddsSummaryRowsInOrder()
{
    QJsonObject payload;
    payload.insert(QStringLiteral("status"), QStringLiteral("완료"));
    payload.insert(QStringLiteral("fileName"), QStringLiteral("input.xlsx"));
    payload.insert(QStringLiteral("outputFolder"), QStringLiteral("/data/out"));
    payload.insert(QStringLiteral("totalFiles"), 3);
    payload.insert(QStringLiteral("passedFiles"), 2);
    payload.insert(QStringLiteral("failedFiles"), 1);
    const SageSampleWorkflowHandler handler;
    const SageWorkflowResultPresenter presenter;
    QList<SageResultRow> rows;

    QCOMPARE(presenter.buildRows(&handler, SageTaskType::Generate, successResponse(payload), rows), true);

    QCOMPARE(rows.size(), 5);
    compareRow(rows.at(0), SAGE_UI_RESULT_STATUS, SAGE_UI_COMPLETED, SAGE_RESULT_STATUS_SUCCESS, QString());
    compareRow(rows.at(1), SAGE_UI_RESULT_RESULT_LABEL, QStringLiteral("완료"), QStringLiteral("완료"), QString());
    compareRow(rows.at(2), SAGE_UI_RESULT_TOTAL_LABEL, QStringLiteral("3"), SAGE_RESULT_STATUS_SUMMARY,
               QStringLiteral("Passed 2, Failed 1"));
    compareRow(rows.at(3), SAGE_UI_RESULT_FILE, QStringLiteral("input.xlsx"), SAGE_RESULT_STATUS_OUTPUT, QString());
    compareRow(rows.at(4), SAGE_UI_RESULT_FOLDER, QStringLiteral("/data/out"), SAGE_RESULT_STATUS_OUTPUT, QString());
}

void SageWorkflowResultPresenterTest::buildRowsSkipsMissingSummaryKeys()
{
    QJsonObject payload;
    payload.insert(QStringLiteral("fileName"), QStringLiteral("input.xlsx"));
    const SageWorkflowResultPresenter presenter;
    QList<SageResultRow> rows;

    QCOMPARE(presenter.buildRows(nullptr, SageTaskType::Generate, successResponse(payload), rows), true);

    QCOMPARE(rows.size(), 2);
    compareRow(rows.at(0), SAGE_UI_RESULT_STATUS, SAGE_UI_COMPLETED, SAGE_RESULT_STATUS_SUCCESS, QString());
    compareRow(rows.at(1), SAGE_UI_RESULT_FILE, QStringLiteral("input.xlsx"), SAGE_RESULT_STATUS_OUTPUT, QString());
}

void SageWorkflowResultPresenterTest::buildRowsKeepsZeroTotalWithMissingCounts()
{
    QJsonObject payload;
    payload.insert(QStringLiteral("totalFiles"), 0);
    const SageWorkflowResultPresenter presenter;
    QList<SageResultRow> rows;

    QCOMPARE(presenter.buildRows(nullptr, SageTaskType::Generate, successResponse(payload), rows), true);

    QCOMPARE(rows.size(), 2);
    compareRow(rows.at(1), SAGE_UI_RESULT_TOTAL_LABEL, QStringLiteral("0"), SAGE_RESULT_STATUS_SUMMARY,
               QStringLiteral("Passed , Failed "));
}

void SageWorkflowResultPresenterTest::buildRowsIgnoresSummaryKeysOutsidePayload()
{
    QJsonObject response = successResponse(QJsonObject());
    response.insert(QStringLiteral("status"), QStringLiteral("top-level"));
    response.insert(QStringLiteral("fileName"), QStringLiteral("top-level.xlsx"));
    const SageWorkflowResultPresenter presenter;
    QList<SageResultRow> rows;

    QCOMPARE(presenter.buildRows(nullptr, SageTaskType::Generate, response, rows), true);

    QCOMPARE(rows.size(), 1);
    compareRow(rows.at(0), SAGE_UI_RESULT_STATUS, SAGE_UI_COMPLETED, SAGE_RESULT_STATUS_SUCCESS, QString());
}

void SageWorkflowResultPresenterTest::buildRowsWithoutHandlerUsesGenericRows()
{
    QJsonObject payload;
    payload.insert(QStringLiteral("status"), QStringLiteral("완료"));
    const SageWorkflowResultPresenter presenter;
    QList<SageResultRow> rows;

    QCOMPARE(presenter.buildRows(nullptr, SageTaskType::Generate, successResponse(payload), rows), true);

    QCOMPARE(rows.size(), 2);
    compareRow(rows.at(1), SAGE_UI_RESULT_RESULT_LABEL, QStringLiteral("완료"), QStringLiteral("완료"), QString());
}

void SageWorkflowResultPresenterTest::buildRowsFromSampleHandlerResponse()
{
    const SageSampleWorkflowHandler handler;
    const SageWorkflowResultPresenter presenter;
    QJsonObject payload;
    payload.insert(QStringLiteral("inputPath"), QStringLiteral("/data/in/input.xlsx"));
    payload.insert(QStringLiteral("outputFolder"), QStringLiteral("/data/out"));
    QList<SageResultRow> rows;

    QCOMPARE(
        presenter.buildRows(&handler, SageTaskType::Generate, handler.runTask(SageTaskType::Generate, payload), rows),
        true);

    QCOMPARE(rows.size(), 5);
    compareRow(rows.at(0), SAGE_UI_RESULT_STATUS, SAGE_UI_COMPLETED, SAGE_RESULT_STATUS_SUCCESS, QString());
    compareRow(rows.at(1), SAGE_UI_RESULT_RESULT_LABEL, QStringLiteral("완료"), QStringLiteral("완료"), QString());
    compareRow(rows.at(2), SAGE_UI_RESULT_TOTAL_LABEL, QStringLiteral("1"), SAGE_RESULT_STATUS_SUMMARY,
               QStringLiteral("Passed 1, Failed 0"));
    compareRow(rows.at(3), SAGE_UI_RESULT_FILE, QStringLiteral("input.xlsx"), SAGE_RESULT_STATUS_OUTPUT, QString());
    compareRow(rows.at(4), SAGE_UI_RESULT_FOLDER, QStringLiteral("/data/out"), SAGE_RESULT_STATUS_OUTPUT, QString());
}

QTEST_GUILESS_MAIN(SageWorkflowResultPresenterTest)

#include "SageWorkflowResultPresenterTest.moc"
