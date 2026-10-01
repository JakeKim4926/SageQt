#include "ui/panels/SageResultTablePanel.h"

#include "SageDefine.h"
#include "core/workflow/SageResultRow.h"
#include "core/workflow/SageWorkflowResultTable.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageFontRegistry.h"
#include "ui/style/SageStyle.h"
#include "ui/widgets/SageLabel.h"

#include <QApplication>
#include <QHeaderView>
#include <QImage>
#include <QList>
#include <QObject>
#include <QRect>
#include <QScrollBar>
#include <QString>
#include <QTableView>
#include <QTest>

class SageResultTablePanelTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void placesTitleAboveTable();
    void usesListRowAndHeaderHeights();
    void keepsFixedColumnWidthsAndStretchesValue();
    void scrollsHorizontallyWhenValueWouldShrink();
    void drawsHeaderAndAlternateRows();
    void drawsSelectionAccentAndGridLine();
    void togglesCheckBoxByClick();
    void paintsHoveredRowAcrossColumns();

private:
    static QList<SageResultRow> sampleRows();
    static QImage render(SageResultTablePanel& panel);
};

void SageResultTablePanelTest::initTestCase()
{
    QString error;
    QVERIFY2(SageFontRegistry::registerApplicationFonts(error), qPrintable(error));
    QApplication::setStyle(new SageStyle);
    QApplication::setPalette(QApplication::style()->standardPalette());
}

QList<SageResultRow> SageResultTablePanelTest::sampleRows()
{
    return {
        {1, QStringLiteral("Alpha"), QStringLiteral("apple"), QStringLiteral("success"), QString()},
        {2, QStringLiteral("Beta"), QStringLiteral("banana"), QStringLiteral("failed"), QStringLiteral("r")},
        {3, QStringLiteral("Gamma"), QStringLiteral("grape"), QStringLiteral("success"), QString()},
    };
}

QImage SageResultTablePanelTest::render(SageResultTablePanel& panel)
{
    panel.resize(1000, 400);
    panel.show();
    if (!QTest::qWaitForWindowExposed(&panel)) {
        return {};
    }
    QTest::qWait(50);
    return panel.grab().toImage();
}

void SageResultTablePanelTest::placesTitleAboveTable()
{
    SageResultTablePanel panel(SAGE_UI_SECTION_RESULT);
    render(panel);

    const SageLabel* title = panel.findChild<SageLabel*>();
    QCOMPARE(title->text(), SAGE_UI_SECTION_RESULT);
    QCOMPARE(title->geometry(), QRect(0, 12, 1000, 25));
    QCOMPARE(panel.findChild<QTableView*>()->geometry().top(), 38);
}

void SageResultTablePanelTest::usesListRowAndHeaderHeights()
{
    SageResultTablePanel panel(SAGE_UI_SECTION_RESULT);
    panel.setColumns(SageWorkflowResultTable::genericColumns(), {});
    panel.setRows(sampleRows());
    render(panel);

    const QTableView* table = panel.findChild<QTableView*>();
    QCOMPARE(panel.rowCount(), 3);
    QCOMPARE(table->horizontalHeader()->height(), 36);
    QCOMPARE(table->rowHeight(0), 34);
    QCOMPARE(table->model()->headerData(1, Qt::Horizontal).toString(), SAGE_UI_RESULT_VALUE);
}

void SageResultTablePanelTest::keepsFixedColumnWidthsAndStretchesValue()
{
    SageResultTablePanel panel(SAGE_UI_SECTION_RESULT);
    panel.setColumns(SageWorkflowResultTable::genericColumns(), {});
    panel.setRows(sampleRows());
    render(panel);

    const QTableView* table = panel.findChild<QTableView*>();
    QCOMPARE(table->columnWidth(0), 140);
    QCOMPARE(table->columnWidth(2), 110);
    QCOMPARE(table->columnWidth(3), 320);
    QCOMPARE(table->columnWidth(1), table->viewport()->width() - 140 - 110 - 320);
}

void SageResultTablePanelTest::scrollsHorizontallyWhenValueWouldShrink()
{
    SageResultTablePanel panel(SAGE_UI_SECTION_RESULT);
    panel.setColumns(SageWorkflowResultTable::genericColumns(), {});
    panel.setRows(sampleRows());
    render(panel);
    panel.resize(600, 400);
    QTest::qWait(50);

    const QTableView* table = panel.findChild<QTableView*>();
    QCOMPARE(table->columnWidth(0), 140);
    QCOMPARE(table->columnWidth(1), 220);
    QCOMPARE(table->columnWidth(2), 110);
    QCOMPARE(table->columnWidth(3), 320);
    QVERIFY(table->horizontalScrollBar()->isVisible());

    panel.resize(1000, 400);
    QTest::qWait(50);
    QCOMPARE(table->columnWidth(1), table->viewport()->width() - 140 - 110 - 320);
    QVERIFY(!table->horizontalScrollBar()->isVisible());
}

void SageResultTablePanelTest::drawsHeaderAndAlternateRows()
{
    SageResultTablePanel panel(SAGE_UI_SECTION_RESULT);
    panel.setColumns(SageWorkflowResultTable::genericColumns(), {});
    panel.setRows(sampleRows());
    const QImage image = render(panel);

    const int tableTop = 38;
    QCOMPARE(image.pixelColor(500, tableTop + 3), SAGE_COLOR_LIST_HEADER);
    QCOMPARE(image.pixelColor(500, tableTop + 1 + 36 + 2), SAGE_COLOR_PANEL);
    QCOMPARE(image.pixelColor(500, tableTop + 1 + 36 + 34 + 2), SAGE_COLOR_LIST_ROW_ALT);
    QCOMPARE(image.pixelColor(0, tableTop + 100), SAGE_COLOR_BORDER);
    QCOMPARE(image.pixelColor(1, tableTop + 40), SAGE_COLOR_PANEL);
    QCOMPARE(image.pixelColor(500, tableTop), SAGE_COLOR_BORDER);
    QCOMPARE(image.pixelColor(500, tableTop + 1), SAGE_COLOR_LIST_HEADER);
    QCOMPARE(image.pixelColor(500, tableTop + 36), SAGE_COLOR_LIST_HEADER);
    QCOMPARE(image.pixelColor(500, tableTop + 37), SAGE_COLOR_PANEL);
    QCOMPARE(image.pixelColor(500, 12 + 5), SAGE_COLOR_LIST_HEADER);
    QCOMPARE(image.pixelColor(500, 12 + 25), SAGE_COLOR_BORDER);
}

void SageResultTablePanelTest::drawsSelectionAccentAndGridLine()
{
    SageResultTablePanel panel(SAGE_UI_SECTION_RESULT);
    SageWorkflowResultStyle style;
    style.m_hasGridLines = true;
    panel.setColumns(SageWorkflowResultTable::genericColumns(), style);
    panel.setRows(sampleRows());
    QTableView* table = panel.findChild<QTableView*>();
    render(panel);
    table->selectRow(0);
    QTest::qWait(50);
    const QImage image = panel.grab().toImage();

    const int rowTop = 38 + 1 + 36;
    QCOMPARE(image.pixelColor(1 + 1, rowTop + 10), SAGE_COLOR_PRIMARY);
    QCOMPARE(image.pixelColor(1 + 10, rowTop + 10), SAGE_COLOR_LIST_ROW_SELECTED);
    QCOMPARE(image.pixelColor(500, rowTop + 33), SAGE_COLOR_LIST_GRID);
    QCOMPARE(image.pixelColor(500, rowTop + 34 + 33), SAGE_COLOR_LIST_GRID);
}

void SageResultTablePanelTest::togglesCheckBoxByClick()
{
    SageResultTablePanel panel(SAGE_UI_SECTION_RESULT);
    SageWorkflowResultStyle style;
    style.m_hasCheckbox = true;
    panel.setColumns(SageWorkflowResultTable::genericColumns(), style);
    panel.setRows(sampleRows());
    QTableView* table = panel.findChild<QTableView*>();
    render(panel);

    const QRect cell = table->visualRect(table->model()->index(1, 0));
    const QPoint boxCenter(cell.left() + 6 + 7, cell.center().y());
    QTest::mouseClick(table->viewport(), Qt::LeftButton, {}, boxCenter);
    QCOMPARE(table->model()->index(1, 0).data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Checked);

    QTest::qWait(50);
    const QImage image = table->viewport()->grab().toImage();
    QCOMPARE(image.pixelColor(cell.left() + 6 + 1, cell.top() + 10 + 1), SAGE_COLOR_PRIMARY);

    QTest::mouseClick(table->viewport(), Qt::LeftButton, {}, QPoint(cell.left() + 60, cell.center().y()));
    QCOMPARE(table->model()->index(1, 0).data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Checked);
    QTest::mouseClick(table->viewport(), Qt::LeftButton, {}, boxCenter);
    QCOMPARE(table->model()->index(1, 0).data(Qt::CheckStateRole).value<Qt::CheckState>(), Qt::Unchecked);
}

void SageResultTablePanelTest::paintsHoveredRowAcrossColumns()
{
    SageResultTablePanel panel(SAGE_UI_SECTION_RESULT);
    panel.setColumns(SageWorkflowResultTable::genericColumns(), {});
    panel.setRows(sampleRows());
    QTableView* table = panel.findChild<QTableView*>();
    render(panel);

    const QRect cell = table->visualRect(table->model()->index(1, 1));
    QTest::mouseMove(table->viewport(), cell.center());
    QTest::qWait(50);
    QImage image = table->viewport()->grab().toImage();
    QCOMPARE(image.pixelColor(cell.center().x(), cell.top() + 3), SAGE_COLOR_LIST_ROW_HOVER);
    QCOMPARE(image.pixelColor(10 + 50, cell.top() + 3), SAGE_COLOR_LIST_ROW_HOVER);
    QCOMPARE(image.pixelColor(cell.center().x(), cell.top() - 34 + 3), SAGE_COLOR_PANEL);

    table->selectRow(1);
    QTest::qWait(50);
    image = table->viewport()->grab().toImage();
    QCOMPARE(image.pixelColor(cell.center().x(), cell.top() + 3), SAGE_COLOR_LIST_ROW_SELECTED);
}

QTEST_MAIN(SageResultTablePanelTest)
#include "SageResultTablePanelTest.moc"
