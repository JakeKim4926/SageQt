#include "ui/panels/SageWorkspacePanel.h"

#include "SageDefine.h"
#include "SageTestModalDriver.h"
#include "SageTestWorkflowHandler.h"
#include "core/workflow/SageWorkflowRegistry.h"
#include "ui/panels/SageResultTablePanel.h"
#include "ui/panels/SageWorkflowHistoryPanel.h"
#include "ui/panels/SageWorkflowInputPanel.h"
#include "ui/panels/SageWorkflowResultPanel.h"
#include "ui/style/SageFontRegistry.h"
#include "ui/style/SageStyle.h"
#include "ui/widgets/SageButton.h"
#include "ui/widgets/SageLabel.h"
#include "ui/widgets/SageSearchBox.h"
#include "ui/widgets/SageSelectionBar.h"
#include "ui/widgets/SageStatusCard.h"
#include "ui/widgets/SageSummaryBar.h"
#include <QLineEdit>

#include <QApplication>
#include <QColor>
#include <QDesktopServices>
#include <QDialog>
#include <QImage>
#include <QJsonObject>
#include <QLabel>
#include <QList>
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QSemaphore>
#include <QSignalSpy>
#include <QStackedWidget>
#include <QString>
#include <QStringList>
#include <QTabBar>
#include <QTableView>
#include <QTemporaryDir>
#include <QTest>
#include <QThreadPool>
#include <QTimer>
#include <QUrl>
#include <QVariant>

#include <memory>
#include <stdexcept>

class SageTestUrlRecorder : public QObject
{
    Q_OBJECT

public:
    QList<QUrl> urls() const
    {
        return m_urls;
    }

public slots:
    void openUrl(const QUrl& url)
    {
        m_urls.append(url);
    }

private:
    QList<QUrl> m_urls;
};

class SageWorkspacePanelTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void tabsFollowHandler();
    void tabChangeSwitchesPanel();
    void tabIsRestoredPerWorkflow();
    void unregisteredWorkflowFallsBackToInput();
    void drawsTabRowWithUniformTabs();
    void pathsAreRestoredPerWorkflow();
    void droppedPathSelectsInputTab();
    void hoveredTabTextTurnsBodyColor();
    void runWithoutInputShowsWarning();
    void generateWithoutOutputShowsWarning();
    void generateRunsInBackgroundAndShowsResult();
    void droppedPathLoadsInputTableWorkflow();
    void showingWorkflowSetsResultColumns();
    void runningCardAdvancesProgressUpToLimit();
    void completedRunShowsResultAndOpensFolder();
    void failedRunShowsReason();
    void inputTableAppearsAfterLoad();
    void generateSendsCheckedRowNums();
    void generateWithoutSelectionShowsError();
    void filterAndChecksAreRestoredPerWorkflow();
    void filterRefreshesSummary();
    void inputResetClearsTable();
    void statusFollowsRunSteps();
    void failedRunReportsFailedStatus();

private:
    void registerGatedHandler(bool hasInputTable);
    void registerInputTableHandler();
    void loadInputTable(SageWorkspacePanel& panel);
    static SageButton* inputResetButton(SageWorkspacePanel& panel);
    static SageLabel* emptyHint(SageWorkspacePanel& panel);
    static QTabBar* tabs(SageWorkspacePanel& panel);
    static SageButton* runButton(SageWorkspacePanel& panel);
    static SageButton* openFolderButton(SageWorkspacePanel& panel);
    static void startGatedGenerate(SageWorkspacePanel& panel, const QString& outputFolder);
    static SageTestModalDriver::SageModalHandler collectMessages(QStringList& outMessages);
    static QStackedWidget* stack(SageWorkspacePanel& panel);
    static QColor darkestColor(const QImage& image, const QRect& area);

private:
    std::unique_ptr<SageWorkflowRegistry> m_registry;
    QSemaphore m_gate;
    int m_runCount = 0;
    SageTaskType m_lastTaskType = SageTaskType::Generate;
    QJsonObject m_lastPayload;
};

static const SageWorkflowType SAGE_TEST_WORKFLOW = static_cast<SageWorkflowType>(2);
static const SageWorkflowType SAGE_UNKNOWN_WORKFLOW = static_cast<SageWorkflowType>(99);
static const SageWorkflowType SAGE_GATED_WORKFLOW = static_cast<SageWorkflowType>(3);
static const SageWorkflowType SAGE_INPUT_TABLE_WORKFLOW = static_cast<SageWorkflowType>(4);

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
    m_runCount = 0;
}

void SageWorkspacePanelTest::cleanup()
{
    m_gate.release();
    QThreadPool::globalInstance()->waitForDone();
    m_gate.acquire(m_gate.available());
}

void SageWorkspacePanelTest::registerGatedHandler(bool hasInputTable)
{
    std::unique_ptr<SageTestWorkflowHandler> handler = std::make_unique<SageTestWorkflowHandler>(
        SAGE_GATED_WORKFLOW, QStringLiteral("대기 업무"), QStringLiteral("샘플"), false);
    handler->setHasInputTable(hasInputTable);
    handler->setRunTask([this](SageTaskType taskType, const QJsonObject& payload) {
        m_gate.acquire();
        ++m_runCount;
        m_lastTaskType = taskType;
        return SageSampleWorkflowHandler().runTask(taskType, payload);
    });
    m_registry->registerHandler(std::move(handler));
}

SageButton* SageWorkspacePanelTest::runButton(SageWorkspacePanel& panel)
{
    const QList<SageButton*> buttons = panel.findChildren<SageButton*>();
    for (SageButton* button : buttons) {
        if (button->text() == SAGE_UI_SAMPLE_ACTION_BUTTON) {
            return button;
        }
    }
    return nullptr;
}

SageButton* SageWorkspacePanelTest::openFolderButton(SageWorkspacePanel& panel)
{
    return panel.findChild<SageStatusCard*>()->findChild<SageButton*>();
}

void SageWorkspacePanelTest::startGatedGenerate(SageWorkspacePanel& panel, const QString& outputFolder)
{
    SageWorkflowInputPanel* inputPanel = panel.findChild<SageWorkflowInputPanel*>();
    panel.showWorkflow(SAGE_GATED_WORKFLOW);
    inputPanel->setInputPath(QStringLiteral("C:/work/in.xlsx"));
    inputPanel->setOutputFolder(outputFolder);
    runButton(panel)->click();
}

SageTestModalDriver::SageModalHandler SageWorkspacePanelTest::collectMessages(QStringList& outMessages)
{
    return [&outMessages](QDialog& dialog) {
        const QList<QLabel*> labels = dialog.findChildren<QLabel*>();
        for (const QLabel* label : labels) {
            outMessages.append(label->text());
        }
        dialog.accept();
    };
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

void SageWorkspacePanelTest::runWithoutInputShowsWarning()
{
    SageWorkspacePanel panel(*m_registry);
    panel.showWorkflow(SageWorkflowType::Sample);
    QStringList messages;
    const SageTestModalDriver driver(collectMessages(messages));

    runButton(panel)->click();

    QVERIFY(messages.contains(SAGE_UI_INPUT_REQUIRED));
    QVERIFY(!panel.isRunning());
}

void SageWorkspacePanelTest::generateWithoutOutputShowsWarning()
{
    SageWorkspacePanel panel(*m_registry);
    panel.showWorkflow(SageWorkflowType::Sample);
    panel.findChild<SageWorkflowInputPanel*>()->setInputPath(QStringLiteral("C:/work/in.xlsx"));
    QStringList messages;
    const SageTestModalDriver driver(collectMessages(messages));

    runButton(panel)->click();

    QVERIFY(messages.contains(SAGE_UI_OUTPUT_REQUIRED));
    QVERIFY(!messages.contains(SAGE_UI_INPUT_REQUIRED));
    QVERIFY(!panel.isRunning());
}

void SageWorkspacePanelTest::generateRunsInBackgroundAndShowsResult()
{
    registerGatedHandler(false);
    SageWorkspacePanel panel(*m_registry);
    SageWorkflowInputPanel* inputPanel = panel.findChild<SageWorkflowInputPanel*>();
    panel.showWorkflow(SAGE_GATED_WORKFLOW);
    inputPanel->setInputPath(QStringLiteral("C:/work/in.xlsx"));
    inputPanel->setOutputFolder(QStringLiteral("C:/work/out"));
    QStringList messages;
    const SageTestModalDriver driver(collectMessages(messages));

    runButton(panel)->click();

    QVERIFY(panel.isRunning());
    QVERIFY(!runButton(panel)->isEnabled());
    panel.applyDroppedPaths({QStringLiteral("C:/work/other.xlsx")});
    QCOMPARE(inputPanel->inputPath(), QStringLiteral("C:/work/in.xlsx"));

    m_gate.release();
    QTRY_VERIFY(!panel.isRunning());
    QCOMPARE(m_runCount, 1);
    QCOMPARE(m_lastTaskType, SageTaskType::Generate);
    QVERIFY(messages.contains(SAGE_UI_SAMPLE_COMPLETED));
    QCOMPARE(panel.selectedTabKind(), SageWorkflowTabKind::DocumentResult);
    QVERIFY(runButton(panel)->isEnabled());
}

void SageWorkspacePanelTest::droppedPathLoadsInputTableWorkflow()
{
    registerGatedHandler(true);
    SageWorkspacePanel panel(*m_registry);
    panel.showWorkflow(SAGE_GATED_WORKFLOW);
    tabs(panel)->setCurrentIndex(2);
    QStringList messages;
    const SageTestModalDriver driver(collectMessages(messages));

    panel.applyDroppedPaths({QStringLiteral("C:/work/in.xlsx")});

    QVERIFY(panel.isRunning());
    m_gate.release();
    QTRY_VERIFY(!panel.isRunning());
    QCOMPARE(m_runCount, 1);
    QCOMPARE(m_lastTaskType, SageTaskType::Load);
    QVERIFY(messages.isEmpty());
    QCOMPARE(panel.selectedTabKind(), SageWorkflowTabKind::Input);
}

void SageWorkspacePanelTest::showingWorkflowSetsResultColumns()
{
    registerGatedHandler(false);
    SageWorkspacePanel panel(*m_registry);
    panel.showWorkflow(SAGE_GATED_WORKFLOW);

    const QTableView* table = panel.findChild<SageWorkflowResultPanel*>()->resultTable().findChild<QTableView*>();
    QCOMPARE(table->model()->columnCount(), 4);
    QCOMPARE(table->model()->rowCount(), 0);
}

void SageWorkspacePanelTest::runningCardAdvancesProgressUpToLimit()
{
    registerGatedHandler(false);
    SageWorkspacePanel panel(*m_registry);
    QStringList messages;
    const SageTestModalDriver driver(collectMessages(messages));
    startGatedGenerate(panel, QStringLiteral("C:/work/out"));

    SageStatusCard* card = panel.findChild<SageStatusCard*>();
    QTimer* progressTimer = panel.findChild<SageWorkflowInputPanel*>()->findChild<QTimer*>();
    QCOMPARE(card->variant(), SageStatusCard::SageStatusCardVariant::Running);
    QCOMPARE(card->message(), SAGE_UI_STATUS_CARD_RUNNING);
    QVERIFY(progressTimer->isActive());
    QCOMPARE(progressTimer->interval(), 300);
    QMetaObject::invokeMethod(progressTimer, "timeout");
    QCOMPARE(card->progressPercent(), 3);
    for (int tick = 0; tick < 40; ++tick) {
        QMetaObject::invokeMethod(progressTimer, "timeout");
    }
    QCOMPARE(card->progressPercent(), 95);

    m_gate.release();
    QTRY_VERIFY(!panel.isRunning());
    QVERIFY(!progressTimer->isActive());
}

void SageWorkspacePanelTest::completedRunShowsResultAndOpensFolder()
{
    registerGatedHandler(false);
    QTemporaryDir outputDirectory;
    QVERIFY(outputDirectory.isValid());
    SageTestUrlRecorder recorder;
    QDesktopServices::setUrlHandler(QStringLiteral("file"), &recorder, "openUrl");
    SageWorkspacePanel panel(*m_registry);
    QStringList messages;
    const SageTestModalDriver driver(collectMessages(messages));
    startGatedGenerate(panel, outputDirectory.path());
    m_gate.release();
    QTRY_VERIFY(!panel.isRunning());

    SageStatusCard* card = panel.findChild<SageStatusCard*>();
    QCOMPARE(card->variant(), SageStatusCard::SageStatusCardVariant::Completed);
    QVERIFY(card->message().startsWith(SAGE_UI_SAMPLE_ACTION_BUTTON + QStringLiteral("이 완료되었습니다 · ")));
    QCOMPARE(card->detail(), outputDirectory.path());
    QVERIFY(panel.findChild<SageWorkflowResultPanel*>()->resultTable().rowCount() > 0);
    QCOMPARE(panel.findChild<SageWorkflowHistoryPanel*>()->visibleRowCount(), 1);

    openFolderButton(panel)->click();
    QCOMPARE(recorder.urls().size(), 1);
    QCOMPARE(recorder.urls().constFirst(), QUrl::fromLocalFile(outputDirectory.path()));

    QVERIFY(outputDirectory.remove());
    openFolderButton(panel)->click();
    QCOMPARE(recorder.urls().size(), 1);
    QVERIFY(messages.contains(SAGE_UI_OUTPUT_PATH_MISSING));
    QDesktopServices::unsetUrlHandler(QStringLiteral("file"));
}

void SageWorkspacePanelTest::failedRunShowsReason()
{
    std::unique_ptr<SageTestWorkflowHandler> handler = std::make_unique<SageTestWorkflowHandler>(
        SAGE_GATED_WORKFLOW, QStringLiteral("실패 업무"), QStringLiteral("샘플"), false);
    handler->setRunTask(
        [](SageTaskType, const QJsonObject&) -> QJsonObject { throw std::runtime_error("handler failure"); });
    m_registry->registerHandler(std::move(handler));
    SageWorkspacePanel panel(*m_registry);
    QStringList messages;
    const SageTestModalDriver driver(collectMessages(messages));
    startGatedGenerate(panel, QStringLiteral("C:/work/out"));
    QTRY_VERIFY(!panel.isRunning());

    SageStatusCard* card = panel.findChild<SageStatusCard*>();
    QCOMPARE(card->variant(), SageStatusCard::SageStatusCardVariant::Failed);
    QCOMPARE(card->message(), SAGE_UI_STATUS_CARD_FAILED_FORMAT.arg(SAGE_UI_SAMPLE_ACTION_BUTTON));
    QCOMPARE(card->detail(), SAGE_UI_WORKFLOW_EXCEPTION);
    QVERIFY(openFolderButton(panel)->isHidden());
    QVERIFY(!messages.contains(SAGE_UI_SAMPLE_COMPLETED));
}

void SageWorkspacePanelTest::registerInputTableHandler()
{
    std::unique_ptr<SageTestWorkflowHandler> handler = std::make_unique<SageTestWorkflowHandler>(
        SAGE_INPUT_TABLE_WORKFLOW, QStringLiteral("입력 표 업무"), QStringLiteral("샘플"), false);
    handler->setHasInputTable(true);
    SageWorkflowResultStyle style;
    style.m_hasCheckbox = true;
    handler->setCustomResultTable(
        style, {{1, SAGE_UI_RESULT_FIELD, SageResultField::Field}, {2, SAGE_UI_RESULT_STATUS, SageResultField::Status}},
        {{3, QStringLiteral("Alpha"), QStringLiteral("apple"), QStringLiteral("success"), QString()},
         {0, QStringLiteral("Beta"), QStringLiteral("banana"), QStringLiteral("failed"), QString()},
         {7, QStringLiteral("Gamma"), QStringLiteral("grape"), QStringLiteral("success"), QString()}});
    handler->setSummaryLabel(QStringLiteral("보이는 행"));
    handler->setSelectionError(QStringLiteral("행을 선택하세요"));
    handler->setRunTask([this](SageTaskType taskType, const QJsonObject& payload) {
        ++m_runCount;
        m_lastTaskType = taskType;
        m_lastPayload = payload;
        QJsonObject response;
        response.insert(SAGE_JSON_KEY_SUCCESS, true);
        response.insert(SAGE_JSON_KEY_PAYLOAD, QJsonObject());
        return response;
    });
    m_registry->registerHandler(std::move(handler));
}

void SageWorkspacePanelTest::loadInputTable(SageWorkspacePanel& panel)
{
    SageWorkflowInputPanel* inputPanel = panel.findChild<SageWorkflowInputPanel*>();
    panel.showWorkflow(SAGE_INPUT_TABLE_WORKFLOW);
    inputPanel->setInputPath(QStringLiteral("C:/work/in.xlsx"));
    emit inputPanel->runRequested(SageTaskType::Load);
    QTRY_VERIFY(!panel.isRunning());
}

void SageWorkspacePanelTest::inputTableAppearsAfterLoad()
{
    registerInputTableHandler();
    SageWorkspacePanel panel(*m_registry);
    panel.resize(1000, 900);
    panel.show();
    QVERIFY(QTest::qWaitForWindowExposed(&panel));
    SageWorkflowInputPanel* inputPanel = panel.findChild<SageWorkflowInputPanel*>();
    SageResultTablePanel& inputTable = inputPanel->inputTable();
    panel.showWorkflow(SAGE_INPUT_TABLE_WORKFLOW);
    QVERIFY(!inputTable.isVisible());
    QVERIFY(emptyHint(panel)->isVisible());

    loadInputTable(panel);

    QCOMPARE(m_lastTaskType, SageTaskType::Load);
    QVERIFY(inputTable.isVisible());
    QVERIFY(!emptyHint(panel)->isVisible());
    QCOMPARE(inputTable.rowCount(), 3);
    QVERIFY(inputTable.findChild<SageSelectionBar*>()->isVisible());
    QVERIFY(inputTable.findChild<SageSearchBox*>()->isVisible());
    QVERIFY(inputResetButton(panel)->isVisible());
    QVERIFY(!runButton(panel)->isEnabled());
    QCOMPARE(panel.selectedTabKind(), SageWorkflowTabKind::Input);
}

void SageWorkspacePanelTest::generateSendsCheckedRowNums()
{
    registerInputTableHandler();
    SageWorkspacePanel panel(*m_registry);
    QStringList messages;
    const SageTestModalDriver driver(collectMessages(messages));
    loadInputTable(panel);
    SageWorkflowInputPanel* inputPanel = panel.findChild<SageWorkflowInputPanel*>();
    SageResultTablePanel& inputTable = inputPanel->inputTable();
    const QTableView* table = inputTable.findChild<QTableView*>();
    table->model()->setData(table->model()->index(0, 0), Qt::Checked, Qt::CheckStateRole);
    table->model()->setData(table->model()->index(2, 0), Qt::Checked, Qt::CheckStateRole);
    QVERIFY(runButton(panel)->isEnabled());
    inputPanel->setOutputFolder(QStringLiteral("C:/work/out"));

    runButton(panel)->click();
    QTRY_VERIFY(!panel.isRunning());

    QCOMPARE(m_lastTaskType, SageTaskType::Generate);
    QCOMPARE(m_lastPayload.value(SAGE_JSON_KEY_ROW_NUMS).toString(), QStringLiteral("3,7"));
    QCOMPARE(inputTable.rowCount(), 3);
    QCOMPARE(inputTable.checkedRowCount(), 2);
    QCOMPARE(panel.findChild<SageStatusCard*>()->message(),
             SAGE_UI_STATUS_CARD_COMPLETED_FORMAT.arg(SAGE_UI_SAMPLE_ACTION_BUTTON).arg(2));
}

void SageWorkspacePanelTest::generateWithoutSelectionShowsError()
{
    registerInputTableHandler();
    SageWorkspacePanel panel(*m_registry);
    QStringList messages;
    const SageTestModalDriver driver(collectMessages(messages));
    loadInputTable(panel);
    SageWorkflowInputPanel* inputPanel = panel.findChild<SageWorkflowInputPanel*>();
    inputPanel->setOutputFolder(QStringLiteral("C:/work/out"));
    const int runCount = m_runCount;

    emit inputPanel->runRequested(SageTaskType::Generate);

    QVERIFY(messages.contains(QStringLiteral("행을 선택하세요")));
    QVERIFY(!panel.isRunning());
    QCOMPARE(m_runCount, runCount);
}

void SageWorkspacePanelTest::filterAndChecksAreRestoredPerWorkflow()
{
    registerInputTableHandler();
    SageWorkspacePanel panel(*m_registry);
    loadInputTable(panel);
    SageResultTablePanel& inputTable = panel.findChild<SageWorkflowInputPanel*>()->inputTable();
    inputTable.restoreFilter(QStringLiteral("success"), 2);
    QCOMPARE(inputTable.rowCount(), 2);
    const QTableView* table = inputTable.findChild<QTableView*>();
    table->model()->setData(table->model()->index(1, 0), Qt::Checked, Qt::CheckStateRole);
    QCOMPARE(inputTable.checkedRowNums(), QStringLiteral("7"));

    panel.showWorkflow(SAGE_TEST_WORKFLOW);
    QVERIFY(!inputTable.isVisible());
    panel.showWorkflow(SAGE_INPUT_TABLE_WORKFLOW);

    QCOMPARE(inputTable.filterKeyword(), QStringLiteral("success"));
    QCOMPARE(inputTable.filterCriteria(), 2);
    QCOMPARE(inputTable.rowCount(), 2);
    QCOMPARE(inputTable.checkedRowNums(), QStringLiteral("7"));
    QVERIFY(runButton(panel)->isEnabled());
}

void SageWorkspacePanelTest::filterRefreshesSummary()
{
    registerInputTableHandler();
    SageWorkspacePanel panel(*m_registry);
    loadInputTable(panel);
    SageResultTablePanel& inputTable = panel.findChild<SageWorkflowInputPanel*>()->inputTable();
    inputTable.showSelectAll(false);
    SageSummaryBar* summaryBar = inputTable.findChild<SageSummaryBar*>();
    QVERIFY(summaryBar->hasItems());

    QLineEdit* edit = inputTable.findChild<SageSearchBox*>()->findChild<QLineEdit*>();
    edit->setText(QStringLiteral("gamma"));
    QTest::keyClick(edit, Qt::Key_Return);

    QCOMPARE(inputTable.rowCount(), 1);
    QCOMPARE(inputTable.visibleRows().size(), 1);
    QVERIFY(summaryBar->hasItems());
}

void SageWorkspacePanelTest::inputResetClearsTable()
{
    registerInputTableHandler();
    SageWorkspacePanel panel(*m_registry);
    panel.resize(1000, 900);
    panel.show();
    QVERIFY(QTest::qWaitForWindowExposed(&panel));
    loadInputTable(panel);
    SageWorkflowInputPanel* inputPanel = panel.findChild<SageWorkflowInputPanel*>();

    inputResetButton(panel)->click();

    QVERIFY(inputPanel->inputPath().isEmpty());
    QVERIFY(!inputPanel->inputTable().isVisible());
    QVERIFY(emptyHint(panel)->isVisible());
    QVERIFY(!inputResetButton(panel)->isVisible());
    QCOMPARE(inputPanel->inputTable().rowCount(), 0);
    QCOMPARE(panel.findChild<SageStatusCard*>()->variant(), SageStatusCard::SageStatusCardVariant::Idle);
}

SageButton* SageWorkspacePanelTest::inputResetButton(SageWorkspacePanel& panel)
{
    const QList<SageButton*> buttons = panel.findChild<SageWorkflowInputPanel*>()->findChildren<SageButton*>();
    for (SageButton* button : buttons) {
        if (button->text() == SAGE_UI_INPUT_RESET_BTN && button->icon().isNull()) {
            return button;
        }
    }
    return nullptr;
}

SageLabel* SageWorkspacePanelTest::emptyHint(SageWorkspacePanel& panel)
{
    const QList<SageLabel*> labels = panel.findChildren<SageLabel*>();
    for (SageLabel* label : labels) {
        if (label->text() == SAGE_UI_EMPTY_STATE_HINT) {
            return label;
        }
    }
    return nullptr;
}

void SageWorkspacePanelTest::statusFollowsRunSteps()
{
    registerInputTableHandler();
    SageWorkspacePanel panel(*m_registry);
    QSignalSpy statusSpy(&panel, &SageWorkspacePanel::statusChanged);
    panel.showWorkflow(SAGE_INPUT_TABLE_WORKFLOW);
    QCOMPARE(statusSpy.takeFirst().at(0).toString(), SAGE_UI_READY);

    panel.applyDroppedPaths({QStringLiteral("C:/work/in.xlsx")});
    QTRY_VERIFY(!panel.isRunning());
    QStringList statuses;
    for (const QList<QVariant>& arguments : std::as_const(statusSpy)) {
        statuses.append(arguments.at(0).toString());
    }
    QCOMPARE(statuses, QStringList({SAGE_UI_DROP_RECEIVED, SAGE_UI_RUNNING, SAGE_UI_COMPLETED}));

    statusSpy.clear();
    inputResetButton(panel)->click();
    QCOMPARE(statusSpy.count(), 1);
    QCOMPARE(statusSpy.takeFirst().at(0).toString(), SAGE_UI_READY);
}

void SageWorkspacePanelTest::failedRunReportsFailedStatus()
{
    std::unique_ptr<SageTestWorkflowHandler> handler = std::make_unique<SageTestWorkflowHandler>(
        SAGE_GATED_WORKFLOW, QStringLiteral("실패 업무"), QStringLiteral("샘플"), false);
    handler->setRunTask(
        [](SageTaskType, const QJsonObject&) -> QJsonObject { throw std::runtime_error("handler failure"); });
    m_registry->registerHandler(std::move(handler));
    SageWorkspacePanel panel(*m_registry);
    QStringList messages;
    const SageTestModalDriver driver(collectMessages(messages));
    QSignalSpy statusSpy(&panel, &SageWorkspacePanel::statusChanged);
    startGatedGenerate(panel, QStringLiteral("C:/work/out"));
    QTRY_VERIFY(!panel.isRunning());

    QCOMPARE(statusSpy.last().at(0).toString(), SAGE_UI_FAILED);
}

QTEST_MAIN(SageWorkspacePanelTest)

#include "SageWorkspacePanelTest.moc"
