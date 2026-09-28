#include "ui/style/SageStyle.h"

#include "ui/widgets/SageButton.h"

#include <QColor>
#include <QImage>
#include <QObject>
#include <QPalette>
#include <QPushButton>
#include <QSize>
#include <QStyle>
#include <QTest>

class SageStyleTest : public QObject
{
    Q_OBJECT

private slots:
    void baseStyleIsFusion();
    void paletteUsesSageSdiColors();
    void pushButtonHeightIsFixed();
    void primaryButtonFollowsState();
    void secondaryButtonDrawsBorder();
    void standardIconsAreDrawn();
    void messageIconHasTransparentBackground();

private:
    static QColor faceColor(QPushButton& button);
};

void SageStyleTest::baseStyleIsFusion()
{
    const SageStyle style;

    QCOMPARE(style.baseStyle()->name().compare(QStringLiteral("fusion"), Qt::CaseInsensitive), 0);
}

void SageStyleTest::paletteUsesSageSdiColors()
{
    const SageStyle style;

    const QPalette palette = style.standardPalette();

    QCOMPARE(palette.color(QPalette::Window), QColor(248, 246, 241));
    QCOMPARE(palette.color(QPalette::Base), QColor(255, 255, 255));
    QCOMPARE(palette.color(QPalette::AlternateBase), QColor(250, 248, 244));
    QCOMPARE(palette.color(QPalette::WindowText), QColor(47, 42, 36));
    QCOMPARE(palette.color(QPalette::Text), QColor(47, 42, 36));
    QCOMPARE(palette.color(QPalette::ButtonText), QColor(47, 42, 36));
    QCOMPARE(palette.color(QPalette::PlaceholderText), QColor(180, 171, 160));
    QCOMPARE(palette.color(QPalette::Button), QColor(255, 255, 255));
    QCOMPARE(palette.color(QPalette::Highlight), QColor(241, 227, 205));
    QCOMPARE(palette.color(QPalette::HighlightedText), QColor(47, 42, 36));
    QCOMPARE(palette.color(QPalette::Accent), QColor(154, 107, 63));
}

void SageStyleTest::pushButtonHeightIsFixed()
{
    SageStyle style;
    QPushButton button(QStringLiteral("확인"));
    button.setStyle(&style);

    QCOMPARE(button.sizeHint().height(), 32);
}

void SageStyleTest::primaryButtonFollowsState()
{
    SageStyle style;
    SageButton button(QStringLiteral("확인"));
    button.setStyle(&style);
    button.setVariant(SageButton::SageButtonVariant::Primary);

    QCOMPARE(faceColor(button), QColor(154, 107, 63));
    button.setDown(true);
    QCOMPARE(faceColor(button), QColor(118, 80, 42));
    button.setDown(false);
    button.setEnabled(false);
    QCOMPARE(faceColor(button), QColor(220, 214, 205));
}

void SageStyleTest::secondaryButtonDrawsBorder()
{
    SageStyle style;
    SageButton button(QStringLiteral("취소"));
    button.setStyle(&style);
    button.resize(button.sizeHint());

    const QImage image = button.grab().toImage();

    QCOMPARE(image.pixelColor(0, 0), QColor(201, 191, 177));
    QCOMPARE(image.pixelColor(1, 1), QColor(255, 255, 255));
}

void SageStyleTest::standardIconsAreDrawn()
{
    const SageStyle style;

    QVERIFY(!style.standardIcon(QStyle::SP_TitleBarCloseButton).isNull());
    QVERIFY(!style.standardIcon(QStyle::SP_MessageBoxInformation).isNull());
    QVERIFY(!style.standardIcon(QStyle::SP_MessageBoxWarning).isNull());
    QVERIFY(!style.standardIcon(QStyle::SP_MessageBoxCritical).isNull());
}

void SageStyleTest::messageIconHasTransparentBackground()
{
    const SageStyle style;

    const QImage image = style.standardIcon(QStyle::SP_MessageBoxInformation).pixmap(QSize(22, 22), 1.0).toImage();

    QCOMPARE(image.pixelColor(0, 0).alpha(), 0);
    QCOMPARE(image.pixelColor(11, 1).rgb(), QColor(154, 107, 63).rgb());
}

QColor SageStyleTest::faceColor(QPushButton& button)
{
    button.resize(button.sizeHint());
    const QImage image = button.grab().toImage();
    return image.pixelColor(1, 1);
}

QTEST_MAIN(SageStyleTest)

#include "SageStyleTest.moc"
