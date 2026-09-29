#include "ui/panels/SageWorkspacePanel.h"

#include "SageDefine.h"
#include "SageTestWorkflowHandler.h"
#include "core/workflow/SageWorkflowRegistry.h"
#include "ui/panels/SageWorkflowHistoryPanel.h"
#include "ui/panels/SageWorkflowInputPanel.h"
#include "ui/panels/SageWorkflowResultPanel.h"
#include "ui/style/SageFontRegistry.h"
#include "ui/style/SageStyle.h"

#include <QApplication>
#include <QColor>
#include <QImage>
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QStackedWidget>
#include <QString>
#include <QTabBar>
#include <QTest>

#include <memory>

class SageWorkspacePanelTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void tabsFollowHandler();
    void tabChangeSwitchesPanel();
    void tabIsRestoredPerWorkflow();
    void unregisteredWorkflowFallsBackToInput();
    void drawsTabRowWithUniformTabs();
    void pathsAreRestoredPerWorkflow();
    void droppedPathSelectsInputTab();
    void hoveredTabTextTurnsBodyColor();

private:
    static QTabBar* tabs(SageWorkspacePanel& panel);
    static QStackedWidget* stack(SageWorkspacePanel& panel);
    static QColor darkestColor(const QImage& image, const QRect& area);

private:
    std::unique_ptr<SageWorkflowRegistry> m_registry;
};

static const SageWorkflowType SAGE_TEST_WORKFLOW = static_cast<SageWorkflowType>(2);
static const SageWorkflowType SAGE_UNKNOWN_WORKFLOW = static_cast<SageWorkflowType>(99);

void SageWorkspacePanelTest::initTestCase()
{
    QString error;
    QVERIFY2(SageFontRegistry::registerApplicationFonts(error), qPrintable(error));
    QApplication::setStyle(new SageStyle);
    QApplication::setPalette(QApplication::style()->standardPalette());
}

void SageWorkspacePanelTest::init()
{
    m_registry = std::make_unique<SageWorkflowRegistry>();
    m_registry->registerHandler(std::make_unique<SageTestWorkflowHandler>(
        SAGE_TEST_WORKFLOW, QStringLiteral("테스트 업무"), QStringLiteral("샘플"), false));
}

QTabBar* SageWorkspacePanelTest::tabs(SageWorkspacePanel& panel)
{
    return panel.findChild<QTabBar*>();
}

QStackedWidget* SageWorkspacePanelTest::stack(SageWorkspacePanel& panel)
{
    return panel.findChild<QStackedWidget*>();
}

void SageWorkspacePanelTest::tabsFollowHandler()
{
    SageWorkspacePanel panel(*m_registry);

    panel.showWorkflow(SageWorkflowType::Sample);

    QCOMPARE(tabs(panel)->count(), 3);
    QCOMPARE(tabs(panel)->tabText(0), QStringLiteral("입력"));
    QCOMPARE(tabs(panel)->tabText(1), QStringLiteral("결과"));
    QCOMPARE(tabs(panel)->tabText(2), QStringLiteral("실행 기록"));
    QCOMPARE(panel.selectedTabKind(), SageWorkflowTabKind::Input);
    QVERIFY(qobject_cast<SageWorkflowInputPanel*>(stack(panel)->currentWidget()) != nullptr);
}

void SageWorkspacePanelTest::tabChangeSwitchesPanel()
{
    SageWorkspacePanel panel(*m_registry);
    panel.showWorkflow(SageWorkflowType::Sample);

    tabs(panel)->setCurrentIndex(1);
    QCOMPARE(panel.selectedTabKind(), SageWorkflowTabKind::DocumentResult);
    QVERIFY(qobject_cast<SageWorkflowResultPanel*>(stack(panel)->currentWidget()) != nullptr);

    tabs(panel)->setCurrentIndex(2);
    QCOMPARE(panel.selectedTabKind(), SageWorkflowTabKind::DocumentHistory);
    QVERIFY(qobject_cast<SageWorkflowHistoryPanel*>(stack(panel)->currentWidget()) != nullptr);
}

void SageWorkspacePanelTest::tabIsRestoredPerWorkflow()
{
    SageWorkspacePanel panel(*m_registry);
    panel.showWorkflow(SageWorkflowType::Sample);
    tabs(panel)->setCurrentIndex(2);

    panel.showWorkflow(SAGE_TEST_WORKFLOW);
    QCOMPARE(panel.selectedTabKind(), SageWorkflowTabKind::Input);
    tabs(panel)->setCurrentIndex(1);

    panel.showWorkflow(SageWorkflowType::Sample);
    QCOMPARE(panel.selectedTabKind(), SageWorkflowTabKind::DocumentHistory);
    QCOMPARE(tabs(panel)->currentIndex(), 2);

    panel.showWorkflow(SAGE_TEST_WORKFLOW);
    QCOMPARE(panel.selectedTabKind(), SageWorkflowTabKind::DocumentResult);
}

void SageWorkspacePanelTest::unregisteredWorkflowFallsBackToInput()
{
    SageWorkspacePanel panel(*m_registry);
    panel.showWorkflow(SageWorkflowType::Sample);
    tabs(panel)->setCurrentIndex(1);

    panel.showWorkflow(SAGE_UNKNOWN_WORKFLOW);

    QCOMPARE(tabs(panel)->count(), 0);
    QCOMPARE(panel.selectedTabKind(), SageWorkflowTabKind::Input);
    QVERIFY(qobject_cast<SageWorkflowInputPanel*>(stack(panel)->currentWidget()) != nullptr);
}

void SageWorkspacePanelTest::drawsTabRowWithUniformTabs()
{
    SageWorkspacePanel panel(*m_registry);
    panel.showWorkflow(SageWorkflowType::Sample);
    panel.resize(800, 400);
    panel.show();
    QVERIFY(QTest::qWaitForWindowExposed(&panel));
    QTabBar* tabBar = tabs(panel);
    const QRect firstTab = tabBar->tabRect(0).translated(tabBar->mapTo(&panel, QPoint()));

    const QImage image = panel.grab().toImage();

    QVERIFY(tabBar->testAttribute(Qt::WA_Hover));
    QCOMPARE(tabBar->tabRect(0).width(), tabBar->tabRect(2).width());
    QCOMPARE(firstTab.left(), 24);
    QCOMPARE(firstTab.height(), 39);
    QCOMPARE(image.pixelColor(700, 10), QColor(255, 255, 255));
    QCOMPARE(image.pixelColor(700, 39), QColor(220, 214, 205));
    QCOMPARE(image.pixelColor(firstTab.center().x(), 37), QColor(154, 107, 63));
    QCOMPARE(image.pixelColor(firstTab.center().x(), 38), QColor(154, 107, 63));
    QCOMPARE(image.pixelColor(firstTab.center().x(), 36), QColor(255, 255, 255));
    QCOMPARE(image.pixelColor(700, 380), QColor(248, 246, 241));
    QCOMPARE(image.pixelColor(10, 200), QColor(248, 246, 241));
    QCOMPARE(stack(panel)->mapTo(&panel, QPoint()), QPoint(24, 40 + 20));
    QCOMPARE(stack(panel)->width(), 800 - 24 - 24);
}

void SageWorkspacePanelTest::pathsAreRestoredPerWorkflow()
{
    SageWorkspacePanel panel(*m_registry);
    SageWorkflowInputPanel* inputPanel = panel.findChild<SageWorkflowInputPanel*>();
    panel.showWorkflow(SageWorkflowType::Sample);
    inputPanel->setInputPath(QStringLiteral("C:/work/sample.xlsx"));
    inputPanel->setOutputFolder(QStringLiteral("C:/work/out"));

    panel.showWorkflow(SAGE_TEST_WORKFLOW);
    QVERIFY(inputPanel->inputPath().isEmpty());
    inputPanel->setInputPath(QStringLiteral("C:/work/test.xlsx"));

    panel.showWorkflow(SageWorkflowType::Sample);
    QCOMPARE(inputPanel->inputPath(), QStringLiteral("C:/work/sample.xlsx"));
    QCOMPARE(inputPanel->outputFolder(), QStringLiteral("C:/work/out"));

    panel.showWorkflow(SAGE_TEST_WORKFLOW);
    QCOMPARE(inputPanel->inputPath(), QStringLiteral("C:/work/test.xlsx"));
    QVERIFY(inputPanel->outputFolder().isEmpty());
}

void SageWorkspacePanelTest::droppedPathSelectsInputTab()
{
    SageWorkspacePanel panel(*m_registry);
    SageWorkflowInputPanel* inputPanel = panel.findChild<SageWorkflowInputPanel*>();
    panel.showWorkflow(SageWorkflowType::Sample);
    tabs(panel)->setCurrentIndex(2);

    panel.applyDroppedPaths({QStringLiteral("C:/work/first.xlsx"), QStringLiteral("C:/work/second.xlsx")});

    QCOMPARE(inputPanel->inputPath(), QStringLiteral("C:/work/first.xlsx"));
    QCOMPARE(panel.selectedTabKind(), SageWorkflowTabKind::Input);

    panel.applyDroppedPaths({});
    QCOMPARE(inputPanel->inputPath(), QStringLiteral("C:/work/first.xlsx"));
}

void SageWorkspacePanelTest::hoveredTabTextTurnsBodyColor()
{
    SageWorkspacePanel panel(*m_registry);
    panel.showWorkflow(SageWorkflowType::Sample);
    panel.resize(800, 400);
    panel.show();
    QVERIFY(QTest::qWaitForWindowExposed(&panel));
    QTabBar* tabBar = tabs(panel);
    const QRect secondTab = tabBar->tabRect(1);

    const int normal = darkestColor(tabBar->grab().toImage(), secondTab).lightness();
    QVERIFY(normal >= QColor(122, 112, 100).lightness());
    QTest::mouseMove(tabBar, secondTab.center());
    QTRY_VERIFY(darkestColor(tabBar->grab().toImage(), secondTab).lightness() < normal);
}

QColor SageWorkspacePanelTest::darkestColor(const QImage& image, const QRect& area)
{
    QColor darkest(Qt::white);
    for (int y = area.top(); y <= area.bottom(); ++y) {
        for (int x = area.left(); x <= area.right(); ++x) {
            if (image.pixelColor(x, y).lightness() < darkest.lightness()) {
                darkest = image.pixelColor(x, y);
            }
        }
    }
    return darkest;
}

QTEST_MAIN(SageWorkspacePanelTest)

#include "SageWorkspacePanelTest.moc"
