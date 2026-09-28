#include "ui/dialogs/SageMessageBoxDlg.h"

#include "SageDefine.h"
#include "ui/style/SageFontCatalog.h"
#include "ui/style/SageFontRegistry.h"
#include "ui/style/SageStyle.h"

#include <QApplication>
#include <QColor>
#include <QImage>
#include <QLabel>
#include <QLayout>
#include <QObject>
#include <QString>
#include <QTest>
#include <QToolButton>

class SageMessageBoxDlgTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void escapeRejects();
    void enterAccepts();
    void captionCloseButtonRejects();
    void isFramelessWithMinimumWidth();
    void titleFollowsIcon();
    void messageIsPlainText();
    void drawsBorderCaptionAndPanel();
    void initialAdminNoticeWrapsAtMinimumWidth();

private:
    static void showAndWait(SageMessageBoxDlg& dialog);
};

void SageMessageBoxDlgTest::initTestCase()
{
    QString error;
    QVERIFY2(SageFontRegistry::registerApplicationFonts(error), qPrintable(error));
    QApplication::setStyle(new SageStyle);
    QApplication::setPalette(QApplication::style()->standardPalette());
    QApplication::setFont(SageFontCatalog::font(SageFontRole::Body));
}

void SageMessageBoxDlgTest::showAndWait(SageMessageBoxDlg& dialog)
{
    dialog.show();
    QVERIFY(QTest::qWaitForWindowExposed(&dialog));
}

void SageMessageBoxDlgTest::escapeRejects()
{
    SageMessageBoxDlg dialog(SageMessageIcon::Warning, QStringLiteral("message"));
    showAndWait(dialog);

    QTest::keyClick(&dialog, Qt::Key_Escape);

    QCOMPARE(dialog.result(), static_cast<int>(QDialog::Rejected));
    QVERIFY(!dialog.isVisible());
}

void SageMessageBoxDlgTest::enterAccepts()
{
    SageMessageBoxDlg dialog(SageMessageIcon::Warning, QStringLiteral("message"));
    showAndWait(dialog);

    QTest::keyClick(&dialog, Qt::Key_Return);

    QCOMPARE(dialog.result(), static_cast<int>(QDialog::Accepted));
    QVERIFY(!dialog.isVisible());
}

void SageMessageBoxDlgTest::captionCloseButtonRejects()
{
    SageMessageBoxDlg dialog(SageMessageIcon::Info, QStringLiteral("message"));
    showAndWait(dialog);
    QToolButton* closeButton = dialog.findChild<QToolButton*>();
    QVERIFY(closeButton != nullptr);

    closeButton->click();

    QCOMPARE(dialog.result(), static_cast<int>(QDialog::Rejected));
    QVERIFY(!dialog.isVisible());
}

void SageMessageBoxDlgTest::isFramelessWithMinimumWidth()
{
    SageMessageBoxDlg dialog(SageMessageIcon::Info, QStringLiteral("message"));
    showAndWait(dialog);

    QVERIFY(dialog.windowFlags().testFlag(Qt::FramelessWindowHint));
    QCOMPARE(dialog.width(), 360);
}

void SageMessageBoxDlgTest::titleFollowsIcon()
{
    QCOMPARE(SageMessageBoxDlg(SageMessageIcon::Info, QString()).windowTitle(), QStringLiteral("알림"));
    QCOMPARE(SageMessageBoxDlg(SageMessageIcon::Warning, QString()).windowTitle(), QStringLiteral("경고"));
    QCOMPARE(SageMessageBoxDlg(SageMessageIcon::Error, QString()).windowTitle(), QStringLiteral("오류"));
}

void SageMessageBoxDlgTest::messageIsPlainText()
{
    const SageMessageBoxDlg dialog(SageMessageIcon::Error, QStringLiteral("<b>message</b>"));

    const QList<QLabel*> labels = dialog.findChildren<QLabel*>();
    bool hasPlainMessage = false;
    for (const QLabel* label : labels) {
        hasPlainMessage = hasPlainMessage ||
                          (label->text() == QStringLiteral("<b>message</b>") && label->textFormat() == Qt::PlainText);
    }
    QVERIFY(hasPlainMessage);
}

void SageMessageBoxDlgTest::drawsBorderCaptionAndPanel()
{
    SageMessageBoxDlg dialog(SageMessageIcon::Info, QStringLiteral("message"));
    showAndWait(dialog);

    const QImage image = dialog.grab().toImage();

    QCOMPARE(image.pixelColor(0, 0), QColor(220, 214, 205));
    QCOMPARE(image.pixelColor(2, 2), QColor(242, 238, 231));
    QCOMPARE(image.pixelColor(2, 40), QColor(220, 214, 205));
    QCOMPARE(image.pixelColor(2, 42), QColor(255, 255, 255));
}

void SageMessageBoxDlgTest::initialAdminNoticeWrapsAtMinimumWidth()
{
    SageMessageBoxDlg dialog(SageMessageIcon::Info, SAGE_UI_INITIAL_ADMIN_PW_FORMAT.arg(
                                                        SAGE_DEFAULT_ADMIN_ID, QStringLiteral("AbCdEfGh234567")));
    showAndWait(dialog);

    QCOMPARE(dialog.width(), 360);
    QCOMPARE(dialog.height(), dialog.layout()->totalHeightForWidth(360));
}

QTEST_MAIN(SageMessageBoxDlgTest)

#include "SageMessageBoxDlgTest.moc"
