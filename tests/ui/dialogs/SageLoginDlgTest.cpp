#include "ui/dialogs/SageLoginDlg.h"

#include "SageTestAuth.h"
#include "SageTestModalDriver.h"
#include "core/auth/SageAuthSession.h"
#include "core/auth/SageUserService.h"
#include "ui/dialogs/SagePasswordChangeDlg.h"
#include "ui/style/SageFontRegistry.h"
#include "ui/style/SageStyle.h"
#include "ui/widgets/SageButton.h"
#include "ui/widgets/SageInlineMessage.h"
#include "ui/widgets/SageLineEdit.h"

#include <QApplication>
#include <QDialog>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTest>

#include <memory>

class SageLoginDlgTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void emptyIdShowsInlineError();
    void emptyPasswordShowsInlineError();
    void wrongPasswordShowsFailedAndSelectsPassword();
    void queryErrorShowsErrorBox();
    void validLoginSetsSessionAndAccepts();
    void canceledForcedChangeLogsOutBeforeWarning();
    void completedForcedChangeKeepsLogin();

private:
    static QList<SageLineEdit*> edits(QWidget& dialog);
    static SageButton* button(QWidget& dialog, const QString& text);
    static QString inlineMessage(QWidget& dialog);
    void submit(SageLoginDlg& dialog, const QString& loginId, const QString& password);

private:
    std::unique_ptr<SageTestUserRepository> m_repository;
    SageTestPasswordHasher m_hasher;
    std::unique_ptr<SageUserService> m_userService;
    std::unique_ptr<SageAuthSession> m_authSession;
};

void SageLoginDlgTest::initTestCase()
{
    QString error;
    QVERIFY2(SageFontRegistry::registerApplicationFonts(error), qPrintable(error));
    QApplication::setStyle(new SageStyle);
    QApplication::setPalette(QApplication::style()->standardPalette());
}

void SageLoginDlgTest::init()
{
    m_repository = std::make_unique<SageTestUserRepository>();
    m_repository->addUser(1, QStringLiteral("admin"), QStringLiteral("Admin1234"), SageUserRole::Admin, true);
    m_repository->addUser(2, QStringLiteral("kim"), QStringLiteral("Kim1234"), SageUserRole::User, false);
    m_userService = std::make_unique<SageUserService>(*m_repository, m_hasher);
    m_authSession = std::make_unique<SageAuthSession>();
}

QList<SageLineEdit*> SageLoginDlgTest::edits(QWidget& dialog)
{
    return dialog.findChildren<SageLineEdit*>();
}

SageButton* SageLoginDlgTest::button(QWidget& dialog, const QString& text)
{
    const QList<SageButton*> buttons = dialog.findChildren<SageButton*>();
    for (SageButton* candidate : buttons) {
        if (candidate->text() == text) {
            return candidate;
        }
    }
    return nullptr;
}

QString SageLoginDlgTest::inlineMessage(QWidget& dialog)
{
    return dialog.findChild<SageInlineMessage*>()->message();
}

void SageLoginDlgTest::submit(SageLoginDlg& dialog, const QString& loginId, const QString& password)
{
    edits(dialog).at(0)->setText(loginId);
    edits(dialog).at(1)->setText(password);
    button(dialog, QStringLiteral("로그인"))->click();
}

void SageLoginDlgTest::emptyIdShowsInlineError()
{
    SageLoginDlg dialog(*m_userService, *m_authSession);

    submit(dialog, QStringLiteral("   "), QStringLiteral("x"));

    QCOMPARE(inlineMessage(dialog), QStringLiteral("아이디를 입력하세요."));
    QCOMPARE(edits(dialog).at(0)->variant(), SageLineEdit::SageLineEditVariant::Error);
    QCOMPARE(edits(dialog).at(1)->variant(), SageLineEdit::SageLineEditVariant::Normal);
}

void SageLoginDlgTest::emptyPasswordShowsInlineError()
{
    SageLoginDlg dialog(*m_userService, *m_authSession);

    submit(dialog, QStringLiteral("kim"), QString());

    QCOMPARE(inlineMessage(dialog), QStringLiteral("비밀번호를 입력하세요."));
    QCOMPARE(edits(dialog).at(1)->variant(), SageLineEdit::SageLineEditVariant::Error);
}

void SageLoginDlgTest::wrongPasswordShowsFailedAndSelectsPassword()
{
    SageLoginDlg dialog(*m_userService, *m_authSession);

    submit(dialog, QStringLiteral("kim"), QStringLiteral("wrong"));

    QTRY_COMPARE(inlineMessage(dialog), QStringLiteral("아이디 또는 비밀번호가 올바르지 않습니다."));
    QCOMPARE(edits(dialog).at(1)->selectedText(), QStringLiteral("wrong"));
    QVERIFY(!m_authSession->isLoggedIn());
}

void SageLoginDlgTest::queryErrorShowsErrorBox()
{
    m_repository->setFailing(true);
    SageLoginDlg dialog(*m_userService, *m_authSession);
    const SageTestModalDriver driver([](QDialog& modal) { modal.reject(); });

    submit(dialog, QStringLiteral("kim"), QStringLiteral("Kim1234"));

    QTRY_COMPARE(driver.titles(), QStringList{QStringLiteral("오류")});
    QVERIFY(!m_authSession->isLoggedIn());
}

void SageLoginDlgTest::validLoginSetsSessionAndAccepts()
{
    SageLoginDlg dialog(*m_userService, *m_authSession);
    dialog.show();

    submit(dialog, QStringLiteral(" kim "), QStringLiteral("Kim1234"));

    QTRY_COMPARE(dialog.result(), static_cast<int>(QDialog::Accepted));
    QVERIFY(m_authSession->isLoggedIn());
    QCOMPARE(m_authSession->currentUser().m_loginId, QStringLiteral("kim"));
}

void SageLoginDlgTest::canceledForcedChangeLogsOutBeforeWarning()
{
    SageLoginDlg dialog(*m_userService, *m_authSession);
    dialog.show();
    QList<bool> loggedInAtModal;
    SageAuthSession& session = *m_authSession;
    const SageTestModalDriver driver([&loggedInAtModal, &session](QDialog& modal) {
        loggedInAtModal.append(session.isLoggedIn());
        modal.reject();
    });

    submit(dialog, QStringLiteral("admin"), QStringLiteral("Admin1234"));

    QTRY_COMPARE(driver.titles().size(), 3);
    QCOMPARE(driver.titles(),
             (QStringList{QStringLiteral("경고"), QStringLiteral("비밀번호 변경"), QStringLiteral("경고")}));
    QCOMPARE(loggedInAtModal, (QList<bool>{true, true, false}));
    QVERIFY(!m_authSession->isLoggedIn());
    QVERIFY(edits(dialog).at(1)->text().isEmpty());
    QVERIFY(dialog.result() != QDialog::Accepted);
}

void SageLoginDlgTest::completedForcedChangeKeepsLogin()
{
    SageLoginDlg dialog(*m_userService, *m_authSession);
    dialog.show();
    const SageTestModalDriver driver([](QDialog& modal) {
        if (qobject_cast<SagePasswordChangeDlg*>(&modal) == nullptr) {
            modal.accept();
            return;
        }
        const QList<SageLineEdit*> passwordEdits = modal.findChildren<SageLineEdit*>();
        passwordEdits.at(0)->setText(QStringLiteral("Admin1234"));
        passwordEdits.at(1)->setText(QStringLiteral("New1234"));
        passwordEdits.at(2)->setText(QStringLiteral("New1234"));
        SageLoginDlgTest::button(modal, QStringLiteral("변경"))->click();
    });

    submit(dialog, QStringLiteral("admin"), QStringLiteral("Admin1234"));

    QTRY_COMPARE(dialog.result(), static_cast<int>(QDialog::Accepted));
    QCOMPARE(driver.titles(),
             (QStringList{QStringLiteral("경고"), QStringLiteral("비밀번호 변경"), QStringLiteral("알림")}));
    QVERIFY(m_authSession->isLoggedIn());
    QVERIFY(!m_authSession->currentUser().m_isPasswordChangeRequired);
    QCOMPARE(m_repository->user(QStringLiteral("admin")).m_passwordHash, QStringLiteral("hashed:New1234"));
}

QTEST_MAIN(SageLoginDlgTest)

#include "SageLoginDlgTest.moc"
