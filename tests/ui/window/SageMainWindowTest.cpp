#include "ui/window/SageMainWindow.h"

#include "SageTestAuth.h"
#include "core/auth/SageAuthSession.h"
#include "core/auth/SageUserService.h"
#include "core/workflow/SageWorkflowRegistry.h"
#include "ui/panels/SageHeaderPanel.h"
#include "ui/panels/SageSidebarPanel.h"
#include "ui/panels/SageWorkflowInputPanel.h"
#include "ui/panels/SageWorkflowResultPanel.h"
#include "ui/panels/SageWorkspacePanel.h"
#include "ui/style/SageFontRegistry.h"
#include "ui/style/SageStyle.h"
#include "ui/widgets/SageLabel.h"
#include "ui/widgets/SageLineEdit.h"

#include <QApplication>
#include <QColor>
#include <QDir>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QImage>
#include <QList>
#include <QMimeData>
#include <QObject>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QString>
#include <QTabBar>
#include <QTest>
#include <QTreeView>
#include <QUrl>

class SageMainWindowTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void startsAtInitialSize();
    void headerShowsSelectedWorkflow();
    void drawsSidebarDividerAndAlignedLines();
    void fileDroppedOnInputPathEditBecomesInputPath();
    void fileDroppedOnResultAreaBecomesInputPath();
    void fileDroppedOnSidebarBecomesInputPath();

private:
    static void dropFile(QWidget& target, const QString& localPath);
    static void verifyDropOn(const QString& target);
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

void SageMainWindowTest::dropFile(QWidget& target, const QString& localPath)
{
    QMimeData mimeData;
    mimeData.setUrls({QUrl::fromLocalFile(localPath)});
    const QPointF position = QRectF(target.rect()).center();
    QDragEnterEvent enterEvent(position.toPoint(), Qt::CopyAction, &mimeData, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(&target, &enterEvent);
    QDropEvent dropEvent(position, Qt::CopyAction, &mimeData, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(&target, &dropEvent);
}

void SageMainWindowTest::fileDroppedOnInputPathEditBecomesInputPath()
{
    verifyDropOn(QStringLiteral("inputPathEdit"));
}

void SageMainWindowTest::fileDroppedOnResultAreaBecomesInputPath()
{
    verifyDropOn(QStringLiteral("resultArea"));
}

void SageMainWindowTest::fileDroppedOnSidebarBecomesInputPath()
{
    verifyDropOn(QStringLiteral("sidebar"));
}

void SageMainWindowTest::verifyDropOn(const QString& target)
{
    const SageWorkflowRegistry registry;
    const SageTestUserRepository repository;
    const SageTestPasswordHasher hasher;
    const SageUserService userService(repository, hasher);
    SageAuthSession session;
    SageMainWindow window(registry, userService, session);
    window.show();
    QVERIFY(QTest::qWaitForWindowExposed(&window));
    SageWorkspacePanel* workspace = window.findChild<SageWorkspacePanel*>();
    SageWorkflowInputPanel* inputPanel = window.findChild<SageWorkflowInputPanel*>();
    workspace->findChild<QTabBar*>()->setCurrentIndex(1);

    QWidget* dropTarget = nullptr;
    if (target == QStringLiteral("inputPathEdit")) {
        dropTarget = inputPanel->findChild<SageLineEdit*>();
    } else if (target == QStringLiteral("resultArea")) {
        dropTarget = window.findChild<SageWorkflowResultPanel*>();
    } else {
        dropTarget = window.findChild<SageSidebarPanel*>()->findChild<QTreeView*>()->viewport();
    }
    const QString droppedPath = QDir::temp().filePath(QStringLiteral("input.xlsx"));

    dropFile(*dropTarget, droppedPath);

    QCOMPARE(inputPanel->inputPath(), droppedPath);
    QCOMPARE(workspace->selectedTabKind(), SageWorkflowTabKind::Input);
}

QTEST_MAIN(SageMainWindowTest)

#include "SageMainWindowTest.moc"
