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

private:
    static QList<SageResultRow> sampleRows();
    static SageWorkflowResultStyle checkboxStyle();
};

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
    QCOMPARE(model.index(2, 0).data(static_cast<int>(SageResultTableRole::SourceRowIndex)).toInt(), 7);
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
    QAbstractItemModelTester tester(&proxy);
    model.setColumns(SageWorkflowResultTable::genericColumns(), {});
    model.setRows(sampleRows());

    proxy.setFilter(QStringLiteral("  Apple "), SageResultField::Value);
    QCOMPARE(proxy.rowCount(), 2);
    QCOMPARE(proxy.index(0, 0).data().toString(), QStringLiteral("Alpha"));
    QCOMPARE(proxy.index(1, 0).data().toString(), QStringLiteral("Gamma"));

    proxy.setFilter(QStringLiteral("apple"), SageResultField::Field);
    QCOMPARE(proxy.rowCount(), 0);

    proxy.setFilter(QStringLiteral("FAIL"), SageResultField::Status);
    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.index(0, 0).data().toString(), QStringLiteral("Beta"));
}

void SageResultTableModelTest::emptyKeywordShowsAllRows()
{
    SageResultTableModel model;
    SageResultFilterProxyModel proxy(model);
    model.setColumns(SageWorkflowResultTable::genericColumns(), {});
    model.setRows(sampleRows());

    proxy.setFilter(QStringLiteral("   "), SageResultField::Value);
    QCOMPARE(proxy.rowCount(), 3);
}

void SageResultTableModelTest::filterChangeClearsChecks()
{
    SageResultTableModel model;
    SageResultFilterProxyModel proxy(model);
    model.setColumns(SageWorkflowResultTable::genericColumns(), checkboxStyle());
    model.setRows(sampleRows());
    proxy.setAllRowsChecked(true);
    QCOMPARE(proxy.checkedRowCount(), 3);

    proxy.setFilter(QString(), SageResultField::Value);

    QCOMPARE(proxy.checkedRowCount(), 0);
    for (int row = 0; row < model.rowCount(); ++row) {
        QCOMPARE(model.index(row, 0).data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
    }
}

void SageResultTableModelTest::visibleRowsFollowFilter()
{
    SageResultTableModel model;
    SageResultFilterProxyModel proxy(model);
    model.setColumns(SageWorkflowResultTable::genericColumns(), {});
    model.setRows(sampleRows());
    proxy.setFilter(QStringLiteral("success"), SageResultField::Status);

    const QList<SageResultRow> rows = proxy.visibleRows();
    QCOMPARE(rows.size(), 2);
    QCOMPARE(rows.at(0).m_field, QStringLiteral("Alpha"));
    QCOMPARE(rows.at(1).m_field, QStringLiteral("Gamma"));
}

void SageResultTableModelTest::checkedRowNumsSkipZeroIndexAndUseSourceIndex()
{
    SageResultTableModel model;
    SageResultFilterProxyModel proxy(model);
    model.setColumns(SageWorkflowResultTable::genericColumns(), checkboxStyle());
    model.setRows(sampleRows());
    proxy.setAllRowsChecked(true);

    QCOMPARE(proxy.checkedRowCount(), 3);
    QCOMPARE(proxy.checkedRowNums(), QStringLiteral("3,7"));

    proxy.setFilter(QStringLiteral("gamma"), SageResultField::Field);
    proxy.setRowChecked(0, true);
    QCOMPARE(proxy.checkedRowNums(), QStringLiteral("7"));
}

void SageResultTableModelTest::restoreChecksOnlyVisibleRows()
{
    SageResultTableModel model;
    SageResultFilterProxyModel proxy(model);
    model.setColumns(SageWorkflowResultTable::genericColumns(), checkboxStyle());
    model.setRows(sampleRows());
    proxy.setFilter(QStringLiteral("gamma"), SageResultField::Field);

    proxy.restoreCheckedRowNums(QStringLiteral(" 3 ,, 7,9"));

    QCOMPARE(proxy.checkedRowNums(), QStringLiteral("7"));
    QCOMPARE(model.index(0, 0).data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);

    proxy.setFilter(QString(), SageResultField::Field);
    proxy.restoreCheckedRowNums(QStringLiteral("3,7"));
    QCOMPARE(proxy.checkedRowNums(), QStringLiteral("3,7"));
    QVERIFY(!proxy.isRowChecked(1));
}

void SageResultTableModelTest::setAllRowsCheckedTouchesVisibleRowsOnly()
{
    SageResultTableModel model;
    SageResultFilterProxyModel proxy(model);
    model.setColumns(SageWorkflowResultTable::genericColumns(), checkboxStyle());
    model.setRows(sampleRows());
    proxy.setFilter(QStringLiteral("a"), SageResultField::Status);

    proxy.setAllRowsChecked(true);

    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.checkedRowCount(), 1);
    QCOMPARE(model.index(0, 0).data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
    QCOMPARE(model.index(1, 0).data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Checked);
}

QTEST_GUILESS_MAIN(SageResultTableModelTest)
#include "SageResultTableModelTest.moc"
