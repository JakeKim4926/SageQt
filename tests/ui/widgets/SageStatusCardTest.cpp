#include "ui/widgets/SageStatusCard.h"

#include "SageDefine.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageFontRegistry.h"
#include "ui/style/SageStyle.h"
#include "ui/widgets/SageButton.h"

#include <QApplication>
#include <QColor>
#include <QImage>
#include <QObject>
#include <QProgressBar>
#include <QSignalSpy>
#include <QString>
#include <QTest>

class SageStatusCardTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void startsIdleWithoutBarAndButton();
    void runningShowsProgressBar();
    void progressIsIgnoredWhenNotRunning();
    void completedWithPathShowsOpenFolder();
    void failedHidesOpenFolder();
    void openFolderClickIsSignaled();
    void drawsStateSurfaceAndBorder();
    void drawsProgressFill();

private:
    static QImage render(SageStatusCard& card);
};

void SageStatusCardTest::initTestCase()
{
    QString error;
    QVERIFY2(SageFontRegistry::registerApplicationFonts(error), qPrintable(error));
    QApplication::setStyle(new SageStyle);
    QApplication::setPalette(QApplication::style()->standardPalette());
}

QImage SageStatusCardTest::render(SageStatusCard& card)
{
    card.resize(600, card.height());
    card.show();
    if (!QTest::qWaitForWindowExposed(&card)) {
        return {};
    }
    return card.grab().toImage();
}

void SageStatusCardTest::startsIdleWithoutBarAndButton()
{
    SageStatusCard card;
    card.setIdle(SAGE_UI_STATUS_CARD_IDLE);

    QCOMPARE(card.height(), 70);
    QCOMPARE(card.variant(), SageStatusCard::SageStatusCardVariant::Idle);
    QCOMPARE(card.message(), SAGE_UI_STATUS_CARD_IDLE);
    QVERIFY(card.findChild<QProgressBar*>()->isHidden());
    QVERIFY(card.findChild<SageButton*>()->isHidden());
}

void SageStatusCardTest::runningShowsProgressBar()
{
    SageStatusCard card;
    QSignalSpy variantSpy(&card, &SageStatusCard::variantChanged);
    card.setRunning(SAGE_UI_STATUS_CARD_RUNNING);
    card.setProgressPercent(42);
    render(card);

    QCOMPARE(variantSpy.count(), 1);
    QCOMPARE(card.progressPercent(), 42);
    const QProgressBar* bar = card.findChild<QProgressBar*>();
    QVERIFY(bar->isVisible());
    QCOMPARE(bar->geometry(), QRect(16, 48, 600 - 32, 6));
    QVERIFY(card.findChild<SageButton*>()->isHidden());
}

void SageStatusCardTest::progressIsIgnoredWhenNotRunning()
{
    SageStatusCard card;
    card.setProgressPercent(30);
    QCOMPARE(card.progressPercent(), 0);

    card.setRunning(SAGE_UI_STATUS_CARD_RUNNING);
    card.setProgressPercent(30);
    card.setRunning(SAGE_UI_STATUS_CARD_RUNNING);
    QCOMPARE(card.progressPercent(), 0);
}

void SageStatusCardTest::completedWithPathShowsOpenFolder()
{
    SageStatusCard card;
    card.setResult(true, QStringLiteral("실행이 완료되었습니다 · 3건"), QStringLiteral("C:/work/out"));
    render(card);

    QCOMPARE(card.variant(), SageStatusCard::SageStatusCardVariant::Completed);
    const SageButton* button = card.findChild<SageButton*>();
    QVERIFY(button->isVisible());
    QCOMPARE(button->text(), SAGE_UI_STATUS_CARD_OPEN_FOLDER);
    QCOMPARE(button->geometry().right(), 600 - 16 - 1);
    QCOMPARE(button->geometry().center().y(), 70 / 2 - 1);
    QVERIFY(card.findChild<QProgressBar*>()->isHidden());

    card.setResult(true, QStringLiteral("실행이 완료되었습니다 · 3건"), QString());
    QVERIFY(button->isHidden());
}

void SageStatusCardTest::failedHidesOpenFolder()
{
    SageStatusCard card;
    card.setResult(false, QStringLiteral("실행에 실패했습니다"), QStringLiteral("사유"));

    QCOMPARE(card.variant(), SageStatusCard::SageStatusCardVariant::Failed);
    QCOMPARE(card.detail(), QStringLiteral("사유"));
    QVERIFY(card.findChild<SageButton*>()->isHidden());
}

void SageStatusCardTest::openFolderClickIsSignaled()
{
    SageStatusCard card;
    QSignalSpy openSpy(&card, &SageStatusCard::openFolderRequested);
    card.setResult(true, QStringLiteral("완료"), QStringLiteral("C:/work/out"));

    card.findChild<SageButton*>()->click();

    QCOMPARE(openSpy.count(), 1);
}

void SageStatusCardTest::drawsStateSurfaceAndBorder()
{
    SageStatusCard card;
    card.setIdle(SAGE_UI_STATUS_CARD_IDLE);
    QImage image = render(card);
    QCOMPARE(image.pixelColor(300, 2), SAGE_COLOR_PANEL);
    QCOMPARE(image.pixelColor(300, 0), SAGE_COLOR_BORDER);

    card.setResult(true, QStringLiteral("완료"), QString());
    image = card.grab().toImage();
    QCOMPARE(image.pixelColor(300, 2), SAGE_COLOR_STATUS_CARD_BG_SUCCESS);
    QCOMPARE(image.pixelColor(300, 0), SAGE_COLOR_STATUS_CARD_BORDER_SUCCESS);

    card.setResult(false, QStringLiteral("실패"), QString());
    image = card.grab().toImage();
    QCOMPARE(image.pixelColor(300, 2), SAGE_COLOR_STATUS_CARD_BG_ERROR);
    QCOMPARE(image.pixelColor(0, 35), SAGE_COLOR_DANGER_BORDER);
}

void SageStatusCardTest::drawsProgressFill()
{
    SageStatusCard card;
    card.setRunning(SAGE_UI_STATUS_CARD_RUNNING);
    card.setProgressPercent(50);
    const QImage image = render(card);

    QCOMPARE(image.pixelColor(16 + 10, 50), SAGE_COLOR_PRIMARY);
    QCOMPARE(image.pixelColor(600 - 16 - 10, 50), SAGE_COLOR_LIST_GRID);
}

QTEST_MAIN(SageStatusCardTest)
#include "SageStatusCardTest.moc"
