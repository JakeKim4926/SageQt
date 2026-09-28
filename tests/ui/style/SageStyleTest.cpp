#include "ui/style/SageStyle.h"

#include <QColor>
#include <QObject>
#include <QPalette>
#include <QStyle>
#include <QTest>

class SageStyleTest : public QObject
{
    Q_OBJECT

private slots:
    void baseStyleIsFusion();
    void paletteUsesSageSdiColors();
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

QTEST_MAIN(SageStyleTest)

#include "SageStyleTest.moc"
