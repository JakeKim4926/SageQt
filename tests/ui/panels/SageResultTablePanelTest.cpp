#include "ui/panels/SageResultTablePanel.h"

#include "SageDefine.h"
#include "core/workflow/SageResultRow.h"
#include "core/workflow/SageWorkflowResultTable.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageFontCatalog.h"
#include "ui/style/SageFontRegistry.h"
#include "ui/style/SageStyle.h"
#include "ui/widgets/SageButton.h"
#include "ui/widgets/SageLabel.h"
#include "ui/widgets/SageSearchBox.h"
#include "ui/widgets/SageSelectionBar.h"
#include "ui/widgets/SageSummaryBar.h"
#include "ui/widgets/SageTableTotalBar.h"

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QHeaderView>
#include <QImage>
#include <QLineEdit>
#include <QList>
#include <QObject>
#include <QRect>
#include <QScrollBar>
#include <QSignalSpy>
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
    void selectionBarReplacesTitle();
    void placesFilterAtRight();
    void searchFiltersAndClearsChecks();
    void criteriaAndResetRefilter();
    void selectAllTogglesVisibleRows();
    void restoresFilterAndCheckedRows();
    void summaryReplacesTitleAndTotalsFollowColumns();
    void searchIconClickRequestsSearch();

private:
    static QList<SageResultRow> sampleRows();
    static QList<SageWorkflowFilterCriteria> criteria();
    static SageWorkflowResultStyle checkboxStyle();
    static QImage render(SageResultTablePanel& panel);
};

void SageResultTablePanelTest::initTestCase()
{
    QString error;
    QVERIFY2(SageFontRegistry::registerApplicationFonts(error), qPrintable(error));
    QApplication::setStyle(new SageStyle);
    QApplication::setPalette(QApplication::style()->standardPalette());
    QApplication::setFont(SageFontCatalog::font(SageFontRole::Body));
}

QList<SageResultRow> SageResultTablePanelTest::sampleRows()
{
    return {
        {1, QStringLiteral("Alpha"), QStringLiteral("apple"), QStringLiteral("success"), QString()},
        {2, QStringLiteral("Beta"), QStringLiteral("banana"), QStringLiteral("failed"), QStringLiteral("r")},
        {3, QStringLiteral("Gamma"), QStringLiteral("grape"), QStringLiteral("success"), QString()},
    };
}

QList<SageWorkflowFilterCriteria> SageResultTablePanelTest::criteria()
{
    return {{1, SAGE_UI_RESULT_FIELD, SageResultField::Field}, {2, SAGE_UI_RESULT_STATUS, SageResultField::Status}};
}

SageWorkflowResultStyle SageResultTablePanelTest::checkboxStyle()
{
    SageWorkflowResultStyle style;
    style.m_hasCheckbox = true;
    return style;
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
    QCOMPARE(QRect(title->mapTo(&panel, QPoint()), title->size()), QRect(0, 12, 1000, 25));
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

void SageResultTablePanelTest::selectionBarReplacesTitle()
{
    SageResultTablePanel panel(SAGE_UI_SECTION_RESULT);
    panel.setColumns(SageWorkflowResultTable::genericColumns(), checkboxStyle());
    panel.setRows(sampleRows());
    panel.showSelectAll(true);
    render(panel);

    const SageSelectionBar* bar = panel.findChild<SageSelectionBar*>();
    QVERIFY(bar->isVisible());
    QVERIFY(!panel.findChild<SageLabel*>()->isVisible());
    QCOMPARE(bar->mapTo(&panel, QPoint()).y(), 10);
    QCOMPARE(bar->height(), 32);
    QCOMPARE(panel.findChild<QTableView*>()->geometry().top(), 52);
}

void SageResultTablePanelTest::placesFilterAtRight()
{
    SageResultTablePanel panel(SAGE_UI_SECTION_RESULT);
    panel.setColumns(SageWorkflowResultTable::genericColumns(), {});
    panel.setFilterCriteria(criteria());
    panel.showFilter(true);
    render(panel);

    const SageSearchBox* searchBox = panel.findChild<SageSearchBox*>();
    const SageButton* resetButton = nullptr;
    const QList<SageButton*> buttons = panel.findChildren<SageButton*>();
    for (SageButton* button : buttons) {
        if (button->text() == SAGE_UI_RESULT_RESET_BTN) {
            resetButton = button;
        }
    }
    QVERIFY(resetButton != nullptr);
    QCOMPARE(QRect(searchBox->mapTo(&panel, QPoint()), searchBox->size()), QRect(1000 - 274, 10, 274, 32));
    QCOMPARE(QRect(resetButton->mapTo(&panel, QPoint()), resetButton->size()), QRect(1000 - 274 - 8 - 84, 10, 84, 32));
    QCOMPARE(panel.findChild<SageLabel*>()->width(), 1000 - 274 - 8 - 84 - 10);
    const QComboBox* combo = searchBox->findChild<QComboBox*>();
    QCOMPARE(combo->count(), 2);
    QCOMPARE(combo->currentText(), SAGE_UI_RESULT_FIELD);
}

void SageResultTablePanelTest::searchFiltersAndClearsChecks()
{
    SageResultTablePanel panel(SAGE_UI_SECTION_RESULT);
    panel.setColumns(SageWorkflowResultTable::genericColumns(), checkboxStyle());
    panel.setFilterCriteria(criteria());
    panel.showFilter(true);
    panel.setRows(sampleRows());
    render(panel);
    QSignalSpy filterSpy(&panel, &SageResultTablePanel::filterChanged);
    QSignalSpy selectionSpy(&panel, &SageResultTablePanel::selectionChanged);
    QTableView* table = panel.findChild<QTableView*>();
    table->model()->setData(table->model()->index(0, 0), Qt::Checked, Qt::CheckStateRole);
    QCOMPARE(selectionSpy.count(), 1);
    QCOMPARE(selectionSpy.last().at(0).toInt(), 1);

    QLineEdit* edit = panel.findChild<SageSearchBox*>()->findChild<QLineEdit*>();
    edit->setText(QStringLiteral("  gam "));
    QTest::keyClick(edit, Qt::Key_Return);

    QCOMPARE(filterSpy.count(), 1);
    QCOMPARE(panel.rowCount(), 1);
    QCOMPARE(panel.filterKeyword(), QStringLiteral("gam"));
    QCOMPARE(panel.checkedRowCount(), 0);
    QCOMPARE(selectionSpy.count(), 1);
    QCOMPARE(panel.visibleRows().constFirst().m_field, QStringLiteral("Gamma"));
}

void SageResultTablePanelTest::criteriaAndResetRefilter()
{
    SageResultTablePanel panel(SAGE_UI_SECTION_RESULT);
    panel.setColumns(SageWorkflowResultTable::genericColumns(), {});
    panel.setFilterCriteria(criteria());
    panel.showFilter(true);
    panel.setRows(sampleRows());
    render(panel);
    QSignalSpy filterSpy(&panel, &SageResultTablePanel::filterChanged);
    SageSearchBox* searchBox = panel.findChild<SageSearchBox*>();
    QLineEdit* edit = searchBox->findChild<QLineEdit*>();
    edit->setText(QStringLiteral("success"));
    QTest::keyClick(edit, Qt::Key_Return);
    QCOMPARE(panel.rowCount(), 0);

    QComboBox* combo = searchBox->findChild<QComboBox*>();
    combo->setCurrentIndex(1);
    emit combo->activated(1);
    QCOMPARE(panel.filterCriteria(), 2);
    QCOMPARE(panel.rowCount(), 2);

    const QList<SageButton*> buttons = panel.findChildren<SageButton*>();
    for (SageButton* button : buttons) {
        if (button->text() == SAGE_UI_RESULT_RESET_BTN) {
            button->click();
        }
    }
    QCOMPARE(panel.rowCount(), 3);
    QVERIFY(edit->text().isEmpty());
    QCOMPARE(panel.filterKeyword(), QString());
    QCOMPARE(filterSpy.count(), 3);
}

void SageResultTablePanelTest::selectAllTogglesVisibleRows()
{
    SageResultTablePanel panel(SAGE_UI_SECTION_RESULT);
    panel.setColumns(SageWorkflowResultTable::genericColumns(), checkboxStyle());
    panel.setRows(sampleRows());
    panel.showSelectAll(true);
    render(panel);
    QSignalSpy selectionSpy(&panel, &SageResultTablePanel::selectionChanged);
    QCheckBox* selectAll = panel.findChild<SageSelectionBar*>()->findChild<QCheckBox*>();

    selectAll->click();
    QCOMPARE(panel.checkedRowCount(), 3);
    QVERIFY(selectAll->isChecked());
    QCOMPARE(selectionSpy.count(), 1);
    QCOMPARE(selectionSpy.last().at(0).toInt(), 3);
    QCOMPARE(panel.checkedRowNums(), QStringLiteral("1,2,3"));

    selectAll->click();
    QCOMPARE(panel.checkedRowCount(), 0);
    QVERIFY(!selectAll->isChecked());

    QTableView* table = panel.findChild<QTableView*>();
    table->model()->setData(table->model()->index(1, 0), Qt::Checked, Qt::CheckStateRole);
    QVERIFY(!selectAll->isChecked());
    panel.findChild<SageSelectionBar*>()->findChild<SageButton*>()->click();
    QCOMPARE(panel.checkedRowCount(), 0);
    QCOMPARE(selectionSpy.last().at(0).toInt(), 0);
}

void SageResultTablePanelTest::restoresFilterAndCheckedRows()
{
    SageResultTablePanel panel(SAGE_UI_SECTION_RESULT);
    panel.setColumns(SageWorkflowResultTable::genericColumns(), checkboxStyle());
    panel.setFilterCriteria(criteria());
    panel.restoreFilter(QStringLiteral("success"), 2);
    panel.setRows(sampleRows());
    QSignalSpy selectionSpy(&panel, &SageResultTablePanel::selectionChanged);

    panel.restoreCheckedRowNums(QStringLiteral("1,2,3"));

    QCOMPARE(panel.rowCount(), 2);
    QCOMPARE(panel.checkedRowNums(), QStringLiteral("1,3"));
    QCOMPARE(selectionSpy.count(), 1);
    QCOMPARE(panel.findChild<SageSearchBox*>()->keyword(), QStringLiteral("success"));
    panel.restoreCheckedRowNums(QString());
    QCOMPARE(selectionSpy.count(), 1);
}

void SageResultTablePanelTest::summaryReplacesTitleAndTotalsFollowColumns()
{
    SageResultTablePanel panel(SAGE_UI_SECTION_RESULT);
    panel.setColumns(SageWorkflowResultTable::genericColumns(), {});
    panel.setRows(sampleRows());
    panel.setSummaryItems({{QStringLiteral("합계"), QStringLiteral("3"), QStringLiteral("건"), true, false},
                           {QStringLiteral("주의"), QStringLiteral("1"), QStringLiteral("건"), false, true}});
    panel.setTotalCells({{0, QStringLiteral("합계"), SageResultTotalRole::Label},
                         {1, QStringLiteral("3"), SageResultTotalRole::Count}});
    const QImage image = render(panel);

    QVERIFY(panel.findChild<SageSummaryBar*>()->isVisible());
    QVERIFY(!panel.findChild<SageLabel*>()->isVisible());
    QCOMPARE(panel.findChild<QTableView*>()->geometry().top(), 52);
    const SageTableTotalBar* totalBar = panel.findChild<SageTableTotalBar*>();
    QVERIFY(totalBar->isVisible());
    QCOMPARE(totalBar->height(), 40);
    QCOMPARE(totalBar->geometry().top(), panel.findChild<QTableView*>()->geometry().bottom() + 1);
    QCOMPARE(image.pixelColor(500, totalBar->geometry().top()), SAGE_COLOR_BORDER);
    QCOMPARE(image.pixelColor(500, totalBar->geometry().top() + 3), SAGE_COLOR_LIST_HEADER);

    panel.clearSummary();
    panel.clearTotals();
    QTest::qWait(50);
    QVERIFY(panel.findChild<SageLabel*>()->isVisible());
    QVERIFY(!totalBar->isVisible());
    QCOMPARE(panel.findChild<QTableView*>()->geometry().top(), 38);
}

void SageResultTablePanelTest::searchIconClickRequestsSearch()
{
    SageResultTablePanel panel(SAGE_UI_SECTION_RESULT);
    panel.setColumns(SageWorkflowResultTable::genericColumns(), {});
    panel.setFilterCriteria(criteria());
    panel.showFilter(true);
    panel.setRows(sampleRows());
    const QImage image = render(panel);
    QSignalSpy filterSpy(&panel, &SageResultTablePanel::filterChanged);
    SageSearchBox* searchBox = panel.findChild<SageSearchBox*>();
    searchBox->findChild<QLineEdit*>()->setText(QStringLiteral("beta"));

    QTest::mouseClick(searchBox, Qt::LeftButton, {}, QPoint(searchBox->width() - 16, 16));
    QCOMPARE(filterSpy.count(), 1);
    QCOMPARE(panel.rowCount(), 1);

    const QPoint boxTopLeft = searchBox->mapTo(&panel, QPoint());
    QCOMPARE(image.pixelColor(boxTopLeft + QPoint(150, 0)), SAGE_COLOR_BUTTON_BORDER);
    QCOMPARE(image.pixelColor(boxTopLeft + QPoint(92, 16)), SAGE_COLOR_LIST_HEADER_BORDER);
    QCOMPARE(image.pixelColor(boxTopLeft + QPoint(274 - 32, 5)), SAGE_COLOR_LIST_HEADER_BORDER);
    QCOMPARE(image.pixelColor(boxTopLeft + QPoint(274 - 4, 5)), SAGE_COLOR_APP_BACKGROUND);
}

QTEST_MAIN(SageResultTablePanelTest)
#include "SageResultTablePanelTest.moc"
