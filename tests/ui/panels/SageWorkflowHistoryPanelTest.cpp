#include "ui/panels/SageWorkflowHistoryPanel.h"

#include "SageDefine.h"
#include "core/workflow/SageWorkflowHistory.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageFontCatalog.h"
#include "ui/style/SageFontRegistry.h"
#include "ui/style/SageStyle.h"
#include "ui/widgets/SageEmptyState.h"
#include "ui/widgets/SageFilterPillBar.h"
#include "ui/widgets/SageTableView.h"

#include <QAbstractItemModel>
#include <QApplication>
#include <QDateTime>
#include <QFontMetrics>
#include <QImage>
#include <QList>
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QString>
#include <QTest>

class SageWorkflowHistoryPanelTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void startsEmpty();
    void newestEntryComesFirst();
    void filterPillsSwitchRows();
    void drawsFailedRowAndBadges();
    void tableSitsBelowPills();

private:
    static QList<SageHistoryEntry> twoEntries();
    static QImage render(SageWorkflowHistoryPanel& panel);
};

void SageWorkflowHistoryPanelTest::initTestCase()
{
    QString error;
    QVERIFY2(SageFontRegistry::registerApplicationFonts(error), qPrintable(error));
    QApplication::setStyle(new SageStyle);
    QApplication::setPalette(QApplication::style()->standardPalette());
    QApplication::setFont(SageFontCatalog::font(SageFontRole::Body));
}

QList<SageHistoryEntry> SageWorkflowHistoryPanelTest::twoEntries()
{
    const QDateTime time(QDate(2026, 10, 1), QTime(13, 25, 7));
    return {{time, QStringLiteral("C:/in.xlsx"), QString(), QString(), true},
            {time, QString(), QString(), QStringLiteral("사유"), false}};
}

QImage SageWorkflowHistoryPanelTest::render(SageWorkflowHistoryPanel& panel)
{
    panel.resize(1100, 400);
    panel.show();
    if (!QTest::qWaitForWindowExposed(&panel)) {
        return {};
    }
    QTest::qWait(50);
    return panel.grab().toImage();
}

void SageWorkflowHistoryPanelTest::startsEmpty()
{
    SageWorkflowHistoryPanel panel;
    render(panel);

    QVERIFY(!panel.findChild<SageFilterPillBar*>()->isVisible());
    QVERIFY(!panel.findChild<SageTableView*>()->isVisible());
    const SageEmptyState* emptyState = panel.findChild<SageEmptyState*>();
    QVERIFY(emptyState->isVisible());
    QCOMPARE(emptyState->title(), SAGE_UI_HISTORY_EMPTY_TITLE);
    QCOMPARE(emptyState->description(), SAGE_UI_HISTORY_EMPTY_DESC);
    QCOMPARE(emptyState->geometry().top(), 0);
}

void SageWorkflowHistoryPanelTest::newestEntryComesFirst()
{
    SageWorkflowHistoryPanel panel;
    const QDateTime earlier(QDate(2026, 10, 1), QTime(9, 0, 0));
    panel.appendEntries({{earlier, QStringLiteral("first.xlsx"), QStringLiteral("C:/out"), QString(), true}});
    panel.appendEntries(twoEntries());

    const QAbstractItemModel* model = panel.findChild<SageTableView*>()->model();
    QCOMPARE(panel.visibleRowCount(), 3);
    QCOMPARE(model->index(0, 0).data().toString(), QStringLiteral("10-01 13:25:07"));
    QCOMPARE(model->index(0, 1).data().toString(), SAGE_UI_HISTORY_SUCCESS);
    QCOMPARE(model->index(0, 3).data().toString(), SAGE_UI_HISTORY_NO_OUTPUT);
    QCOMPARE(model->index(0, 4).data().toString(), SAGE_UI_AMOUNT_EMPTY_MARK);
    QCOMPARE(model->index(1, 1).data().toString(), SAGE_UI_HISTORY_FAILED);
    QCOMPARE(model->index(1, 2).data().toString(), SAGE_UI_AMOUNT_EMPTY_MARK);
    QCOMPARE(model->index(1, 3).data().toString(), SAGE_UI_AMOUNT_EMPTY_MARK);
    QCOMPARE(model->index(1, 4).data().toString(), QStringLiteral("사유"));
    QCOMPARE(model->index(2, 2).data().toString(), QStringLiteral("first.xlsx"));
    QCOMPARE(model->index(2, 3).data().toString(), QStringLiteral("C:/out"));
}

void SageWorkflowHistoryPanelTest::filterPillsSwitchRows()
{
    SageWorkflowHistoryPanel panel;
    panel.appendEntries(twoEntries());
    render(panel);
    SageFilterPillBar* pills = panel.findChild<SageFilterPillBar*>();
    QVERIFY(pills->isVisible());

    const QFontMetrics metrics(SageFontCatalog::font(SageFontRole::Caption));
    const int allWidth = metrics.horizontalAdvance(SAGE_UI_HISTORY_FILTER_ALL.arg(2)) + 24;
    const int successWidth = metrics.horizontalAdvance(SAGE_UI_HISTORY_FILTER_SUCCESS.arg(1)) + 24;
    QTest::mouseClick(pills, Qt::LeftButton, {}, QPoint(allWidth + 8 + successWidth / 2, 14));
    QCOMPARE(pills->selectedIndex(), 1);
    QCOMPARE(panel.visibleRowCount(), 1);

    QTest::mouseClick(pills, Qt::LeftButton, {}, QPoint(allWidth + 8 + successWidth + 8 + 10, 14));
    QCOMPARE(pills->selectedIndex(), 2);
    QCOMPARE(panel.visibleRowCount(), 1);
    QCOMPARE(panel.findChild<SageTableView*>()->model()->index(0, 1).data().toString(), SAGE_UI_HISTORY_FAILED);

    panel.appendEntries({{QDateTime::currentDateTime(), QString(), QString(), QString(), true}});
    QCOMPARE(panel.visibleRowCount(), 1);
}

void SageWorkflowHistoryPanelTest::drawsFailedRowAndBadges()
{
    SageWorkflowHistoryPanel panel;
    panel.appendEntries(twoEntries());
    const QImage image = render(panel);

    const SageTableView* table = panel.findChild<SageTableView*>();
    const QPoint viewportOffset = table->viewport()->mapTo(&panel, QPoint());
    const QRect successRow = table->visualRect(table->model()->index(0, 1)).translated(viewportOffset);
    const QRect failedRow = table->visualRect(table->model()->index(1, 0)).translated(viewportOffset);
    QCOMPARE(image.pixelColor(failedRow.left() + 5, failedRow.top() + 3), SAGE_COLOR_STATUS_CARD_BG_ERROR);
    const QFontMetrics captionMetrics(SageFontCatalog::font(SageFontRole::Caption));
    const int successBadgeLeft =
        successRow.left() + (successRow.width() - captionMetrics.horizontalAdvance(SAGE_UI_HISTORY_SUCCESS) - 16) / 2;
    QCOMPARE(image.pixelColor(successBadgeLeft + 4, successRow.center().y()), SAGE_COLOR_BADGE_BG_SUCCESS);
    const QRect failedBadgeCell = table->visualRect(table->model()->index(1, 1)).translated(viewportOffset);
    const int failedBadgeLeft =
        failedBadgeCell.left() +
        (failedBadgeCell.width() - captionMetrics.horizontalAdvance(SAGE_UI_HISTORY_FAILED) - 16) / 2;
    QCOMPARE(image.pixelColor(failedBadgeLeft + 4, failedBadgeCell.center().y()), SAGE_COLOR_STATUS_BG_ERROR);
    QCOMPARE(image.pixelColor(successRow.center().x(), successRow.bottom()), SAGE_COLOR_LIST_GRID);
}

void SageWorkflowHistoryPanelTest::tableSitsBelowPills()
{
    SageWorkflowHistoryPanel panel;
    panel.appendEntries(twoEntries());
    render(panel);
    QCOMPARE(panel.findChild<SageFilterPillBar*>()->geometry(), QRect(0, 0, 1100, 28));
    QCOMPARE(panel.findChild<SageTableView*>()->geometry().top(), 28 + 12);
    const SageTableView* table = panel.findChild<SageTableView*>();
    QCOMPARE(table->columnWidth(0), 124);
    QCOMPARE(table->columnWidth(1), 88);
}

QTEST_MAIN(SageWorkflowHistoryPanelTest)
#include "SageWorkflowHistoryPanelTest.moc"
