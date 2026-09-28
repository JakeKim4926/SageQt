#include "ui/window/SageMainWindow.h"

#include "SageTestAuth.h"
#include "core/auth/SageAuthSession.h"
#include "core/auth/SageUserService.h"
#include "core/workflow/SageWorkflowRegistry.h"
#include "ui/panels/SageHeaderPanel.h"
#include "ui/style/SageFontRegistry.h"
#include "ui/style/SageStyle.h"
#include "ui/widgets/SageLabel.h"

#include <QApplication>
#include <QColor>
#include <QImage>
#include <QList>
#include <QObject>
#include <QPoint>
#include <QString>
#include <QTest>

class SageMainWindowTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void startsAtInitialSize();
    void headerShowsSelectedWorkflow();
    void drawsSidebarDividerAndAlignedLines();
};

void SageMainWindowTest::initTestCase()
{
    QString error;
    QVERIFY2(SageFontRegistry::registerApplicationFonts(error), qPrintable(error));
    QApplication::setStyle(new SageStyle);
    QApplication::setPalette(QApplication::style()->standardPalette());
}

void SageMainWindowTest::startsAtInitialSize()
{
    const SageWorkflowRegistry registry;
    const SageTestUserRepository repository;
    const SageTestPasswordHasher hasher;
    const SageUserService userService(repository, hasher);
    SageAuthSession session;
    const SageMainWindow window(registry, userService, session);

    QCOMPARE(window.width(), 1280);
    QCOMPARE(window.height(), 800);
}

void SageMainWindowTest::headerShowsSelectedWorkflow()
{
    const SageWorkflowRegistry registry;
    const SageTestUserRepository repository;
    const SageTestPasswordHasher hasher;
    const SageUserService userService(repository, hasher);
    SageAuthSession session;
    const SageMainWindow window(registry, userService, session);

    const SageHeaderPanel* header = window.findChild<SageHeaderPanel*>();
    const QList<SageLabel*> labels = header->findChildren<SageLabel*>();
    bool hasTitle = false;
    for (const SageLabel* label : labels) {
        hasTitle = hasTitle || (label->variant() == SageLabel::SageLabelVariant::Title &&
                                label->text() == QStringLiteral("샘플 업무"));
    }
    QVERIFY(hasTitle);
}

void SageMainWindowTest::drawsSidebarDividerAndAlignedLines()
{
    const SageWorkflowRegistry registry;
    const SageTestUserRepository repository;
    const SageTestPasswordHasher hasher;
    const SageUserService userService(repository, hasher);
    SageAuthSession session;
    SageMainWindow window(registry, userService, session);
    window.show();
    QVERIFY(QTest::qWaitForWindowExposed(&window));
    const SageHeaderPanel* header = window.findChild<SageHeaderPanel*>();

    const QImage image = window.grab().toImage();

    QCOMPARE(header->mapTo(&window, QPoint()), QPoint(221, 0));
    QCOMPARE(image.pixelColor(220, 400), QColor(220, 214, 205));
    QCOMPARE(image.pixelColor(100, 56), QColor(51, 44, 37));
    QCOMPARE(image.pixelColor(600, 56), QColor(220, 214, 205));
    QCOMPARE(image.pixelColor(600, 400), QColor(248, 246, 241));
}

QTEST_MAIN(SageMainWindowTest)

#include "SageMainWindowTest.moc"
