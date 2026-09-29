#include "ui/style/SageStyle.h"

#include "ui/widgets/SageButton.h"
#include "ui/widgets/SageLineEdit.h"

#include <QPainter>

#include <QColor>
#include <QImage>
#include <QObject>
#include <QPalette>
#include <QPushButton>
#include <QSize>
#include <QStyle>
#include <QStyleOptionTab>
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
    void lineEditTextStartsAfterBorderAndPad();
    void readOnlyLineEditTextStartsAfterPanelPad();
    void tabLabelFollowsSelectionAndHover();
    void lineEditBorderFollowsFocusAndError();

private:
    static QColor faceColor(QPushButton& button);
    static int firstDarkColumn(const QImage& image, int fromColumn);
    static int lineEditTextStart(bool isReadOnly);
    static QColor darkestTabLabelColor(QStyle::State state);
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
    QCOMPARE(palette.color(QPalette::Mid), QColor(220, 214, 205));
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

void SageStyleTest::lineEditTextStartsAfterBorderAndPad()
{
    QCOMPARE(lineEditTextStart(false), 5);
}

void SageStyleTest::readOnlyLineEditTextStartsAfterPanelPad()
{
    QCOMPARE(lineEditTextStart(true), 11);
}

int SageStyleTest::lineEditTextStart(bool isReadOnly)
{
    SageStyle style;
    SageLineEdit edit;
    edit.setStyle(&style);
    edit.setReadOnly(isReadOnly);
    edit.setText(QStringLiteral("I"));
    edit.resize(200, 32);
    QImage plain(200, 32, QImage::Format_RGB32);
    plain.fill(Qt::white);
    QPainter painter(&plain);
    painter.setFont(edit.font());
    painter.setPen(Qt::black);
    painter.drawText(0, 20, QStringLiteral("I"));
    painter.end();
    return firstDarkColumn(edit.grab().toImage(), 2) - firstDarkColumn(plain, 0);
}

void SageStyleTest::lineEditBorderFollowsFocusAndError()
{
    SageStyle style;
    SageLineEdit edit;
    edit.setStyle(&style);
    edit.resize(200, 32);
    edit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&edit));

    edit.activateWindow();
    edit.setFocus();
    QVERIFY(QTest::qWaitForWindowActive(&edit));
    QTRY_VERIFY(edit.hasFocus());
    QCOMPARE(edit.grab().toImage().pixelColor(0, 10), QColor(154, 107, 63));

    edit.clearFocus();
    QTRY_VERIFY(!edit.hasFocus());
    QCOMPARE(edit.grab().toImage().pixelColor(0, 10), QColor(220, 214, 205));

    edit.setFocus();
    QTRY_VERIFY(edit.hasFocus());

    edit.setVariant(SageLineEdit::SageLineEditVariant::Error);
    QCOMPARE(edit.grab().toImage().pixelColor(0, 10), QColor(184, 92, 74));
}

int SageStyleTest::firstDarkColumn(const QImage& image, int fromColumn)
{
    for (int x = fromColumn; x < image.width(); ++x) {
        for (int y = 2; y < image.height() - 2; ++y) {
            if (image.pixelColor(x, y).lightness() < 128) {
                return x;
            }
        }
    }
    return -1;
}

void SageStyleTest::tabLabelFollowsSelectionAndHover()
{
    const int normal = darkestTabLabelColor(QStyle::State_Enabled).lightness();
    const int hovered = darkestTabLabelColor(QStyle::State_Enabled | QStyle::State_MouseOver).lightness();
    const int selected = darkestTabLabelColor(QStyle::State_Enabled | QStyle::State_Selected).lightness();

    QVERIFY(normal >= QColor(122, 112, 100).lightness());
    QVERIFY(hovered >= QColor(47, 42, 36).lightness());
    QVERIFY(hovered < normal);
    QVERIFY(selected < normal);
}

QColor SageStyleTest::darkestTabLabelColor(QStyle::State state)
{
    const SageStyle style;
    QImage image(120, 39, QImage::Format_RGB32);
    image.fill(Qt::white);
    QStyleOptionTab option;
    option.rect = image.rect();
    option.text = QStringLiteral("결과");
    option.state = state;
    QPainter painter(&image);
    style.drawControl(QStyle::CE_TabBarTabLabel, &option, &painter);
    painter.end();
    QColor darkest(Qt::white);
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            if (image.pixelColor(x, y).lightness() < darkest.lightness()) {
                darkest = image.pixelColor(x, y);
            }
        }
    }
    return darkest;
}

QTEST_MAIN(SageStyleTest)

#include "SageStyleTest.moc"
