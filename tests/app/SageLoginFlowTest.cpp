#include "SageDefine.h"
#include "core/auth/SageAuthSession.h"
#include "core/auth/SageUserService.h"
#include "core/workflow/SageWorkflowRegistry.h"
#include "infra/auth/SagePbkdf2PasswordHasher.h"
#include "infra/db/SageDbConfig.h"
#include "infra/db/SageSchemaInitializer.h"
#include "infra/db/SageUserRepository.h"
#include "ui/dialogs/SageLoginDlg.h"
#include "ui/dialogs/SageMessageBoxDlg.h"
#include "ui/dialogs/SagePasswordChangeDlg.h"
#include "ui/panels/SageHeaderPanel.h"
#include "ui/panels/SageSidebarPanel.h"
#include "ui/style/SageFontRegistry.h"
#include "ui/style/SageStyle.h"
#include "ui/widgets/SageBadge.h"
#include "ui/widgets/SageButton.h"
#include "ui/widgets/SageInlineMessage.h"
#include "ui/widgets/SageLabel.h"
#include "ui/widgets/SageLineEdit.h"
#include "ui/window/SageMainWindow.h"

#include <QAbstractItemModel>
#include <QApplication>
#include <QDialog>
#include <QDir>
#include <QElapsedTimer>
#include <QLabel>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTemporaryDir>
#include <QTest>
#include <QTimer>
#include <QTreeView>

#include <functional>
#include <optional>

struct SageFlowStep
{
    QString m_name;
    std::function<bool()> m_isReady;
    std::function<void()> m_act;
};

class SageFlowRunner : public QObject
{
    Q_OBJECT

public:
    explicit SageFlowRunner(QObject* parent = nullptr)
        : QObject(parent)
    {
        m_timer.setInterval(SAGE_FLOW_POLL_MS);
        connect(&m_timer, &QTimer::timeout, this, &SageFlowRunner::tick);
    }

    void start(const QList<SageFlowStep>& steps)
    {
        m_steps = steps;
        m_next = 0;
        m_timer.start();
    }

    QStringList done() const
    {
        return m_done;
    }

    QString pending() const
    {
        return m_next < m_steps.size() ? m_steps.at(m_next).m_name : QString();
    }

private slots:
    void tick()
    {
        if (m_next >= m_steps.size()) {
            m_timer.stop();
            return;
        }
        const SageFlowStep& step = m_steps.at(m_next);
        if (!step.m_isReady()) {
            return;
        }
        ++m_next;
        m_done.append(step.m_name);
        step.m_act();
    }

private:
    static constexpr int SAGE_FLOW_POLL_MS = 20;

    QTimer m_timer;
    QList<SageFlowStep> m_steps;
    qsizetype m_next = 0;
    QStringList m_done;
};

class SageLoginFlowTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void loginFlowMatchesSageSdiSteps();

private:
    static QDialog* activeDialog();
    template <typename T> static T* activeModal()
    {
        return qobject_cast<T*>(activeDialog());
    }
    static bool isMessage(const QString& title, const QString& text);
    static void acceptActive();
    static SageButton* button(QWidget& root, const QString& text);
    static QString inlineMessage(QWidget& root);
    static SageLineEdit* edit(QWidget& root, int index);
    void save(QWidget& widget, const QString& name) const;

private:
    QString m_screenshotDirectory;
};

void SageLoginFlowTest::initTestCase()
{
    QString error;
    QVERIFY2(SageFontRegistry::registerApplicationFonts(error), qPrintable(error));
    QApplication::setStyle(new SageStyle);
    QApplication::setPalette(QApplication::style()->standardPalette());
    m_screenshotDirectory = QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("login-flow"));
    QDir().mkpath(m_screenshotDirectory);
}

QDialog* SageLoginFlowTest::activeDialog()
{
    return qobject_cast<QDialog*>(QApplication::activeModalWidget());
}

bool SageLoginFlowTest::isMessage(const QString& title, const QString& text)
{
    const SageMessageBoxDlg* message = activeModal<SageMessageBoxDlg>();
    if (message == nullptr || message->windowTitle() != title) {
        return false;
    }
    const QList<QLabel*> labels = message->findChildren<QLabel*>();
    for (const QLabel* label : labels) {
        if (label->text() == text) {
            return true;
        }
    }
    return false;
}

void SageLoginFlowTest::acceptActive()
{
    activeDialog()->accept();
}

SageButton* SageLoginFlowTest::button(QWidget& root, const QString& text)
{
    const QList<SageButton*> buttons = root.findChildren<SageButton*>();
    for (SageButton* candidate : buttons) {
        if (candidate->text() == text && candidate->isVisibleTo(&root)) {
            return candidate;
        }
    }
    return nullptr;
}

QString SageLoginFlowTest::inlineMessage(QWidget& root)
{
    return root.findChild<SageInlineMessage*>()->message();
}

SageLineEdit* SageLoginFlowTest::edit(QWidget& root, int index)
{
    return root.findChildren<SageLineEdit*>().at(index);
}

void SageLoginFlowTest::save(QWidget& widget, const QString& name) const
{
    widget.grab().save(QDir(m_screenshotDirectory).filePath(name + QStringLiteral(".png")));
}

void SageLoginFlowTest::loginFlowMatchesSageSdiSteps()
{
    const QTemporaryDir temporaryDirectory;
    SageDbConfig config;
    config.m_databasePath = temporaryDirectory.filePath(QStringLiteral("flow.db"));
    QString error;
    QVERIFY2(SageSchemaInitializer(config).prepare(error), qPrintable(error));
    const SageUserRepository repository(config);
    const SagePbkdf2PasswordHasher hasher;
    const SageUserService userService(repository, hasher);
    std::optional<QString> initialPassword;
    QVERIFY2(userService.ensureDefaultAdmin(initialPassword, error), qPrintable(error));
    QVERIFY(initialPassword.has_value());
    const QString newPassword = QStringLiteral("Sage1234");

    SageAuthSession session;
    const SageWorkflowRegistry registry;
    SageMainWindow window(registry, userService, session);
    window.show();
    QVERIFY(QTest::qWaitForWindowExposed(&window));
    save(window, QStringLiteral("00-main-logged-out"));

    int uiTicksDuringLogin = 0;
    QElapsedTimer loginClock;
    qint64 loginMilliseconds = 0;
    QTimer uiHeartbeat;
    uiHeartbeat.setInterval(10);
    connect(&uiHeartbeat, &QTimer::timeout, this, [&uiTicksDuringLogin]() { ++uiTicksDuringLogin; });
    bool loggedInAtCanceledWarning = true;
    bool passwordClearedAfterCancel = false;
    bool wrongPasswordSelected = false;
    bool idEditMarkedError = false;

    SageFlowRunner runner;
    runner.start({
        {QStringLiteral("1 login dialog opens"), [] { return activeModal<SageLoginDlg>() != nullptr; },
         [this] {
             save(*activeDialog(), QStringLiteral("01-login"));
             button(*activeDialog(), SAGE_UI_LOGIN_OK)->click();
         }},
        {QStringLiteral("2-3 empty id"), [] { return inlineMessage(*activeDialog()) == SAGE_UI_LOGIN_EMPTY_ID; },
         [this, &idEditMarkedError] {
             idEditMarkedError = edit(*activeDialog(), 0)->variant() == SageLineEdit::SageLineEditVariant::Error;
             save(*activeDialog(), QStringLiteral("02-empty-id"));
             edit(*activeDialog(), 0)->setText(QStringLiteral("  admin  "));
             button(*activeDialog(), SAGE_UI_LOGIN_OK)->click();
         }},
        {QStringLiteral("4 empty password"), [] { return inlineMessage(*activeDialog()) == SAGE_UI_LOGIN_EMPTY_PW; },
         [this, &uiHeartbeat, &loginClock] {
             save(*activeDialog(), QStringLiteral("03-empty-password"));
             edit(*activeDialog(), 1)->setText(QStringLiteral("wrong"));
             uiHeartbeat.start();
             loginClock.start();
             button(*activeDialog(), SAGE_UI_LOGIN_OK)->click();
         }},
        {QStringLiteral("6 wrong password"), [] { return inlineMessage(*activeDialog()) == SAGE_UI_LOGIN_FAILED; },
         [this, &uiHeartbeat, &loginClock, &loginMilliseconds, &wrongPasswordSelected, &initialPassword] {
             loginMilliseconds = loginClock.elapsed();
             uiHeartbeat.stop();
             wrongPasswordSelected = edit(*activeDialog(), 1)->selectedText() == QStringLiteral("wrong");
             save(*activeDialog(), QStringLiteral("04-wrong-password"));
             edit(*activeDialog(), 1)->setText(*initialPassword);
             button(*activeDialog(), SAGE_UI_LOGIN_OK)->click();
         }},
        {QStringLiteral("8 must change warning"),
         [] { return isMessage(SAGE_UI_MSGBOX_TITLE_WARNING, SAGE_UI_MUST_CHANGE_PW_REQUIRED); },
         [this] {
             save(*activeDialog(), QStringLiteral("05-must-change"));
             acceptActive();
         }},
        {QStringLiteral("8 change dialog then cancel"), [] { return activeModal<SagePasswordChangeDlg>() != nullptr; },
         [this] {
             save(*activeDialog(), QStringLiteral("06-change-dialog"));
             button(*activeDialog(), SAGE_UI_CHANGE_PW_CANCEL)->click();
         }},
        {QStringLiteral("8 canceled warning after logout"),
         [] { return isMessage(SAGE_UI_MSGBOX_TITLE_WARNING, SAGE_UI_MUST_CHANGE_PW_CANCELED); },
         [this, &session, &loggedInAtCanceledWarning] {
             loggedInAtCanceledWarning = session.isLoggedIn();
             save(*activeDialog(), QStringLiteral("07-canceled"));
             acceptActive();
         }},
        {QStringLiteral("8 back to login with empty password"), [] { return activeModal<SageLoginDlg>() != nullptr; },
         [&passwordClearedAfterCancel, &initialPassword] {
             passwordClearedAfterCancel = edit(*activeDialog(), 1)->text().isEmpty();
             edit(*activeDialog(), 1)->setText(*initialPassword);
             button(*activeDialog(), SAGE_UI_LOGIN_OK)->click();
         }},
        {QStringLiteral("8 must change warning again"),
         [] { return isMessage(SAGE_UI_MSGBOX_TITLE_WARNING, SAGE_UI_MUST_CHANGE_PW_REQUIRED); },
         [] { acceptActive(); }},
        {QStringLiteral("8 change password"), [] { return activeModal<SagePasswordChangeDlg>() != nullptr; },
         [&initialPassword, &newPassword] {
             edit(*activeDialog(), 0)->setText(*initialPassword);
             edit(*activeDialog(), 1)->setText(newPassword);
             edit(*activeDialog(), 2)->setText(newPassword);
             button(*activeDialog(), SAGE_UI_CHANGE_PW_OK)->click();
         }},
        {QStringLiteral("8 change completed"),
         [] { return isMessage(SAGE_UI_MSGBOX_TITLE_INFO, SAGE_UI_CHANGE_PW_COMPLETED); },
         [this] {
             save(*activeDialog(), QStringLiteral("08-changed"));
             acceptActive();
         }},
    });
    button(window, SAGE_UI_LOGIN_BTN)->click();

    QCOMPARE(runner.pending(), QString());
    QVERIFY(idEditMarkedError);
    QVERIFY(wrongPasswordSelected);
    QVERIFY(uiTicksDuringLogin > 0);
    QVERIFY(!loggedInAtCanceledWarning);
    QVERIFY(passwordClearedAfterCancel);
    QVERIFY(session.isLoggedIn());
    QVERIFY(session.isAdmin());
    QVERIFY(!session.currentUser().m_isPasswordChangeRequired);
    QVERIFY(button(window, SAGE_UI_LOGOUT_BTN) != nullptr);
    QVERIFY(button(window, SAGE_UI_LOGIN_BTN) == nullptr);
    QCOMPARE(window.findChild<SageBadge*>()->text(), SAGE_UI_ROLE_ADMIN);
    save(window, QStringLiteral("09-main-logged-in"));
    qInfo().noquote()
        << QStringLiteral("login check %1 ms, UI ticks %2").arg(loginMilliseconds).arg(uiTicksDuringLogin);

    QTreeView* sidebarTree = window.findChild<SageSidebarPanel*>()->findChild<QTreeView*>();
    const QAbstractItemModel* sidebarModel = sidebarTree->model();
    const QModelIndex sampleIndex = sidebarModel->index(0, 0, sidebarModel->index(0, 0));
    const QModelIndex passwordIndex = sidebarModel->index(0, 0, sidebarModel->index(1, 0));
    SageFlowRunner sidebarRunner;
    sidebarRunner.start(
        {{QStringLiteral("sidebar change dialog"), [] { return activeModal<SagePasswordChangeDlg>() != nullptr; },
          [] { button(*activeDialog(), SAGE_UI_CHANGE_PW_CANCEL)->click(); }}});
    sidebarTree->setCurrentIndex(passwordIndex);
    QCOMPARE(sidebarRunner.pending(), QString());
    QCOMPARE(sidebarTree->currentIndex(), sampleIndex);

    button(window, SAGE_UI_LOGOUT_BTN)->click();
    QVERIFY(!session.isLoggedIn());
    QVERIFY(button(window, SAGE_UI_LOGIN_BTN) != nullptr);
    save(window, QStringLiteral("10-main-logged-out-again"));

    SageFlowRunner loggedOutRunner;
    loggedOutRunner.start(
        {{QStringLiteral("login required warning"),
          [] { return isMessage(SAGE_UI_MSGBOX_TITLE_WARNING, SAGE_UI_LOGIN_REQUIRED); }, [] { acceptActive(); }}});
    sidebarTree->setCurrentIndex(passwordIndex);
    QCOMPARE(loggedOutRunner.pending(), QString());
    QCOMPARE(sidebarTree->currentIndex(), sampleIndex);
}

QTEST_MAIN(SageLoginFlowTest)

#include "SageLoginFlowTest.moc"
