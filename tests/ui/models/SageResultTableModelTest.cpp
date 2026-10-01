#include "ui/models/SageResultTableModel.h"
#include "ui/models/SageResultFilterProxyModel.h"

#include "SageDefine.h"
#include "core/workflow/SageResultRow.h"
#include "core/workflow/SageWorkflowResultTable.h"

#include <QAbstractItemModelTester>
#include <QList>
#include <QObject>
#include <QSignalSpy>
#include <QString>
#include <QTest>

namespace {
constexpr int SAGE_TEST_CRITERIA_FIELD = 1;
constexpr int SAGE_TEST_CRITERIA_VALUE = 2;
constexpr int SAGE_TEST_CRITERIA_STATUS = 3;
}

class SageResultTableModelTest : public QObject
{
    Q_OBJECT

private slots:
    void showsColumnsAndRowText();
    void alignsCellsByColumnAndCentersFirstColumn();
    void hasNoRowsWithoutColumns();
    void checksOnlyFirstColumnWhenStyleHasCheckbox();
    void newColumnsClearRows();
    void filtersByFieldIgnoringCase();
    void emptyKeywordShowsAllRows();
    void filterChangeClearsChecks();
    void visibleRowsFollowFilter();
    void checkedRowNumsSkipZeroIndexAndUseSourceIndex();
    void restoreChecksOnlyVisibleRows();
    void setAllRowsCheckedTouchesVisibleRowsOnly();
    void unknownCriteriaFallsBackToFirstOrValue();

private:
    static QList<SageResultRow> sampleRows();
    static SageWorkflowResultStyle checkboxStyle();
    static QList<SageWorkflowFilterCriteria> testCriteria();
};

QList<SageWorkflowFilterCriteria> SageResultTableModelTest::testCriteria()
{
    return {{SAGE_TEST_CRITERIA_FIELD, QStringLiteral("항목"), SageResultField::Field},
            {SAGE_TEST_CRITERIA_VALUE, QStringLiteral("값"), SageResultField::Value},
            {SAGE_TEST_CRITERIA_STATUS, QStringLiteral("상태"), SageResultField::Status}};
}

QList<SageResultRow> SageResultTableModelTest::sampleRows()
{
    return {
        {3, QStringLiteral("Alpha"), QStringLiteral("apple"), QStringLiteral("success"), QString()},
        {0, QStringLiteral("Beta"), QStringLiteral("Banana"), QStringLiteral("failed"), QStringLiteral("r")},
        {7, QStringLiteral("Gamma"), QStringLiteral("grape APPLE"), QStringLiteral("success"), QString()},
    };
}

SageWorkflowResultStyle SageResultTableModelTest::checkboxStyle()
{
    SageWorkflowResultStyle style;
    style.m_hasCheckbox = true;
    return style;
}

void SageResultTableModelTest::showsColumnsAndRowText()
{
    SageResultTableModel model;
    QAbstractItemModelTester tester(&model);
    model.setColumns(SageWorkflowResultTable::genericColumns(), {});
    model.setRows(sampleRows());

    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(model.columnCount(), 4);
    QCOMPARE(model.headerData(0, Qt::Horizontal).toString(), SAGE_UI_RESULT_FIELD);
    QCOMPARE(model.headerData(3, Qt::Horizontal).toString(), SAGE_UI_RESULT_REASON);
    QCOMPARE(model.index(1, 0).data().toString(), QStringLiteral("Beta"));
    QCOMPARE(model.index(1, 1).data().toString(), QStringLiteral("Banana"));
    QCOMPARE(model.index(1, 2).data().toString(), QStringLiteral("failed"));
    QCOMPARE(model.index(1, 3).data().toString(), QStringLiteral("r"));
    QCOMPARE(model.index(2, 0).data(static_cast<int>(SageTableRole::SourceRowIndex)).toInt(), 7);
    QVERIFY(!model.index(0, 0).data(Qt::CheckStateRole).isValid());
}

void SageResultTableModelTest::alignsCellsByColumnAndCentersFirstColumn()
{
    SageResultTableModel model;
    model.setColumns({{QStringLiteral("a"), SageColumnAlign::Left, false, SageResultField::Field},
                      {QStringLiteral("b"), SageColumnAlign::Right, true, SageResultField::Value},
                      {QStringLiteral("c"), SageColumnAlign::Left, false, SageResultField::Status}},
                     {});
    model.setRows(sampleRows());

    QCOMPARE(model.index(0, 0).data(Qt::TextAlignmentRole).value<Qt::Alignment>(), Qt::AlignHCenter | Qt::AlignVCenter);
    QCOMPARE(model.index(0, 1).data(Qt::TextAlignmentRole).value<Qt::Alignment>(), Qt::AlignRight | Qt::AlignVCenter);
    QCOMPARE(model.index(0, 2).data(Qt::TextAlignmentRole).value<Qt::Alignment>(), Qt::AlignLeft | Qt::AlignVCenter);
}

void SageResultTableModelTest::hasNoRowsWithoutColumns()
{
    SageResultTableModel model;
    model.setRows(sampleRows());
    QCOMPARE(model.rowCount(), 0);
}

void SageResultTableModelTest::checksOnlyFirstColumnWhenStyleHasCheckbox()
{
    SageResultTableModel model;
    QAbstractItemModelTester tester(&model);
    model.setColumns(SageWorkflowResultTable::genericColumns(), checkboxStyle());
    model.setRows(sampleRows());

    QVERIFY(model.flags(model.index(0, 0)).testFlag(Qt::ItemIsUserCheckable));
    QVERIFY(!model.flags(model.index(0, 1)).testFlag(Qt::ItemIsUserCheckable));
    QCOMPARE(model.index(0, 0).data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
    QVERIFY(!model.setData(model.index(0, 1), Qt::Checked, Qt::CheckStateRole));
    QSignalSpy changedSpy(&model, &SageResultTableModel::dataChanged);
    QVERIFY(model.setData(model.index(0, 0), Qt::Checked, Qt::CheckStateRole));
    QCOMPARE(model.index(0, 0).data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Checked);
    QCOMPARE(changedSpy.count(), 1);

    model.setRows(sampleRows());
    QCOMPARE(model.index(0, 0).data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
}

void SageResultTableModelTest::newColumnsClearRows()
{
    SageResultTableModel model;
    model.setColumns(SageWorkflowResultTable::genericColumns(), {});
    model.setRows(sampleRows());
    model.setColumns(SageWorkflowResultTable::genericColumns(), checkboxStyle());

    QCOMPARE(model.rowCount(), 0);
    QVERIFY(model.resultStyle().m_hasCheckbox);
}

void SageResultTableModelTest::filtersByFieldIgnoringCase()
{
    SageResultTableModel model;
    SageResultFilterProxyModel proxy(model);
    proxy.setFilterCriteria(testCriteria());
    QAbstractItemModelTester tester(&proxy);
    model.setColumns(SageWorkflowResultTable::genericColumns(), {});
    model.setRows(sampleRows());

    proxy.setFilter(QStringLiteral("  Apple "), SAGE_TEST_CRITERIA_VALUE);
    QCOMPARE(proxy.rowCount(), 2);
    QCOMPARE(proxy.index(0, 0).data().toString(), QStringLiteral("Alpha"));
    QCOMPARE(proxy.index(1, 0).data().toString(), QStringLiteral("Gamma"));

    proxy.setFilter(QStringLiteral("apple"), SAGE_TEST_CRITERIA_FIELD);
    QCOMPARE(proxy.rowCount(), 0);

    proxy.setFilter(QStringLiteral("FAIL"), SAGE_TEST_CRITERIA_STATUS);
    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.index(0, 0).data().toString(), QStringLiteral("Beta"));
}

void SageResultTableModelTest::emptyKeywordShowsAllRows()
{
    SageResultTableModel model;
    SageResultFilterProxyModel proxy(model);
    proxy.setFilterCriteria(testCriteria());
    model.setColumns(SageWorkflowResultTable::genericColumns(), {});
    model.setRows(sampleRows());

    proxy.setFilter(QStringLiteral("   "), SAGE_TEST_CRITERIA_VALUE);
    QCOMPARE(proxy.rowCount(), 3);
}

void SageResultTableModelTest::filterChangeClearsChecks()
{
    SageResultTableModel model;
    SageResultFilterProxyModel proxy(model);
    proxy.setFilterCriteria(testCriteria());
    model.setColumns(SageWorkflowResultTable::genericColumns(), checkboxStyle());
    model.setRows(sampleRows());
    proxy.setAllRowsChecked(true);
    QCOMPARE(proxy.checkedRowCount(), 3);

    proxy.setFilter(QString(), SAGE_TEST_CRITERIA_VALUE);

    QCOMPARE(proxy.checkedRowCount(), 0);
    for (int row = 0; row < model.rowCount(); ++row) {
        QCOMPARE(model.index(row, 0).data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
    }
}

void SageResultTableModelTest::visibleRowsFollowFilter()
{
    SageResultTableModel model;
    SageResultFilterProxyModel proxy(model);
    proxy.setFilterCriteria(testCriteria());
    model.setColumns(SageWorkflowResultTable::genericColumns(), {});
    model.setRows(sampleRows());
    proxy.setFilter(QStringLiteral("success"), SAGE_TEST_CRITERIA_STATUS);

    const QList<SageResultRow> rows = proxy.visibleRows();
    QCOMPARE(rows.size(), 2);
    QCOMPARE(rows.at(0).m_field, QStringLiteral("Alpha"));
    QCOMPARE(rows.at(1).m_field, QStringLiteral("Gamma"));
}

void SageResultTableModelTest::checkedRowNumsSkipZeroIndexAndUseSourceIndex()
{
    SageResultTableModel model;
    SageResultFilterProxyModel proxy(model);
    proxy.setFilterCriteria(testCriteria());
    model.setColumns(SageWorkflowResultTable::genericColumns(), checkboxStyle());
    model.setRows(sampleRows());
    proxy.setAllRowsChecked(true);

    QCOMPARE(proxy.checkedRowCount(), 3);
    QCOMPARE(proxy.checkedRowNums(), QStringLiteral("3,7"));

    proxy.setFilter(QStringLiteral("gamma"), SAGE_TEST_CRITERIA_FIELD);
    proxy.setRowChecked(0, true);
    QCOMPARE(proxy.checkedRowNums(), QStringLiteral("7"));
}

void SageResultTableModelTest::restoreChecksOnlyVisibleRows()
{
    SageResultTableModel model;
    SageResultFilterProxyModel proxy(model);
    proxy.setFilterCriteria(testCriteria());
    model.setColumns(SageWorkflowResultTable::genericColumns(), checkboxStyle());
    model.setRows(sampleRows());
    proxy.setFilter(QStringLiteral("gamma"), SAGE_TEST_CRITERIA_FIELD);

    proxy.restoreCheckedRowNums(QStringLiteral(" 3 ,, 7,9"));

    QCOMPARE(proxy.checkedRowNums(), QStringLiteral("7"));
    QCOMPARE(model.index(0, 0).data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);

    proxy.setFilter(QString(), SAGE_TEST_CRITERIA_FIELD);
    proxy.restoreCheckedRowNums(QStringLiteral("3,7"));
    QCOMPARE(proxy.checkedRowNums(), QStringLiteral("3,7"));
    QVERIFY(!proxy.isRowChecked(1));
}

void SageResultTableModelTest::setAllRowsCheckedTouchesVisibleRowsOnly()
{
    SageResultTableModel model;
    SageResultFilterProxyModel proxy(model);
    proxy.setFilterCriteria(testCriteria());
    model.setColumns(SageWorkflowResultTable::genericColumns(), checkboxStyle());
    model.setRows(sampleRows());
    proxy.setFilter(QStringLiteral("a"), SAGE_TEST_CRITERIA_STATUS);

    proxy.setAllRowsChecked(true);

    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.checkedRowCount(), 1);
    QCOMPARE(model.index(0, 0).data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
    QCOMPARE(model.index(1, 0).data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Checked);
}

void SageResultTableModelTest::unknownCriteriaFallsBackToFirstOrValue()
{
    SageResultTableModel model;
    SageResultFilterProxyModel proxy(model);
    model.setColumns(SageWorkflowResultTable::genericColumns(), {});
    model.setRows(sampleRows());

    proxy.setFilter(QStringLiteral("banana"), SAGE_FILTER_CRITERIA_NONE);
    QCOMPARE(proxy.effectiveCriteria(), SAGE_FILTER_CRITERIA_NONE);
    QCOMPARE(proxy.rowCount(), 1);

    proxy.setFilterCriteria(testCriteria());
    proxy.setFilter(QStringLiteral("beta"), 99);
    QCOMPARE(proxy.criteria(), 99);
    QCOMPARE(proxy.effectiveCriteria(), SAGE_TEST_CRITERIA_FIELD);
    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.keyword(), QStringLiteral("beta"));
}

QTEST_GUILESS_MAIN(SageResultTableModelTest)
#include "SageResultTableModelTest.moc"
