#include "ui/dialogs/SagePasswordChangeDlg.h"

#include "SageTestAuth.h"
#include "SageTestModalDriver.h"
#include "core/auth/SageAuthSession.h"
#include "core/auth/SageUserService.h"
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

class SagePasswordChangeDlgTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void emptyCurrentIsCheckedFirst();
    void emptyNewIsCheckedSecond();
    void emptyConfirmIsCheckedThird();
    void mismatchSelectsConfirm();
    void wrongCurrentPasswordShowsInlineError();
    void invalidNewPasswordShowsServiceError();
    void successUpdatesSessionAndAccepts();

private:
    void submit(SagePasswordChangeDlg& dialog, const QString& current, const QString& next, const QString& confirm);
    static QList<SageLineEdit*> edits(QWidget& dialog);
    static QString inlineMessage(QWidget& dialog);

private:
    std::unique_ptr<SageTestUserRepository> m_repository;
    SageTestPasswordHasher m_hasher;
    std::unique_ptr<SageUserService> m_userService;
    SageAuthSession m_authSession{};
};

void SagePasswordChangeDlgTest::initTestCase()
{
    QString error;
    QVERIFY2(SageFontRegistry::registerApplicationFonts(error), qPrintable(error));
    QApplication::setStyle(new SageStyle);
    QApplication::setPalette(QApplication::style()->standardPalette());
}

void SagePasswordChangeDlgTest::init()
{
    m_repository = std::make_unique<SageTestUserRepository>();
    m_repository->addUser(1, QStringLiteral("admin"), QStringLiteral("Admin1234"), SageUserRole::Admin, true);
    m_userService = std::make_unique<SageUserService>(*m_repository, m_hasher);
    m_authSession.logout();
    m_authSession.setLogin(m_repository->user(QStringLiteral("admin")));
}

QList<SageLineEdit*> SagePasswordChangeDlgTest::edits(QWidget& dialog)
{
    return dialog.findChildren<SageLineEdit*>();
}

QString SagePasswordChangeDlgTest::inlineMessage(QWidget& dialog)
{
    return dialog.findChild<SageInlineMessage*>()->message();
}

void SagePasswordChangeDlgTest::submit(SagePasswordChangeDlg& dialog, const QString& current, const QString& next,
                                       const QString& confirm)
{
    edits(dialog).at(0)->setText(current);
    edits(dialog).at(1)->setText(next);
    edits(dialog).at(2)->setText(confirm);
    const QList<SageButton*> buttons = dialog.findChildren<SageButton*>();
    for (SageButton* candidate : buttons) {
        if (candidate->text() == QStringLiteral("변경")) {
            candidate->click();
        }
    }
}

void SagePasswordChangeDlgTest::emptyCurrentIsCheckedFirst()
{
    SagePasswordChangeDlg dialog(*m_userService, m_authSession);

    submit(dialog, QString(), QString(), QString());

    QCOMPARE(inlineMessage(dialog), QStringLiteral("현재 비밀번호를 입력하세요."));
    QCOMPARE(edits(dialog).at(0)->variant(), SageLineEdit::SageLineEditVariant::Error);
}

void SagePasswordChangeDlgTest::emptyNewIsCheckedSecond()
{
    SagePasswordChangeDlg dialog(*m_userService, m_authSession);

    submit(dialog, QStringLiteral("a"), QString(), QString());

    QCOMPARE(inlineMessage(dialog), QStringLiteral("변경할 비밀번호를 입력하세요."));
    QCOMPARE(edits(dialog).at(1)->variant(), SageLineEdit::SageLineEditVariant::Error);
}

void SagePasswordChangeDlgTest::emptyConfirmIsCheckedThird()
{
    SagePasswordChangeDlg dialog(*m_userService, m_authSession);

    submit(dialog, QStringLiteral("a"), QStringLiteral("b"), QString());

    QCOMPARE(inlineMessage(dialog), QStringLiteral("변경할 비밀번호 확인을 입력하세요."));
    QCOMPARE(edits(dialog).at(2)->variant(), SageLineEdit::SageLineEditVariant::Error);
}

void SagePasswordChangeDlgTest::mismatchSelectsConfirm()
{
    SagePasswordChangeDlg dialog(*m_userService, m_authSession);

    submit(dialog, QStringLiteral("Admin1234"), QStringLiteral("New1234"), QStringLiteral("New9999"));

    QCOMPARE(inlineMessage(dialog), QStringLiteral("변경할 비밀번호가 서로 다릅니다."));
    QCOMPARE(edits(dialog).at(2)->selectedText(), QStringLiteral("New9999"));
}

void SagePasswordChangeDlgTest::wrongCurrentPasswordShowsInlineError()
{
    SagePasswordChangeDlg dialog(*m_userService, m_authSession);

    submit(dialog, QStringLiteral("wrong"), QStringLiteral("New1234"), QStringLiteral("New1234"));

    QTRY_COMPARE(inlineMessage(dialog), QStringLiteral("현재 비밀번호가 올바르지 않습니다."));
    QCOMPARE(edits(dialog).at(0)->variant(), SageLineEdit::SageLineEditVariant::Error);
}

void SagePasswordChangeDlgTest::invalidNewPasswordShowsServiceError()
{
    SagePasswordChangeDlg dialog(*m_userService, m_authSession);

    submit(dialog, QStringLiteral("Admin1234"), QStringLiteral("abc"), QStringLiteral("abc"));

    QTRY_COMPARE(inlineMessage(dialog), QStringLiteral("비밀번호는 4자 이상이어야 합니다."));
    QCOMPARE(edits(dialog).at(1)->variant(), SageLineEdit::SageLineEditVariant::Error);
    QCOMPARE(m_repository->user(QStringLiteral("admin")).m_passwordHash, QStringLiteral("hashed:Admin1234"));
}

void SagePasswordChangeDlgTest::successUpdatesSessionAndAccepts()
{
    SagePasswordChangeDlg dialog(*m_userService, m_authSession);
    dialog.show();
    const SageTestModalDriver driver([](QDialog& modal) { modal.accept(); });

    submit(dialog, QStringLiteral("Admin1234"), QStringLiteral("New1234"), QStringLiteral("New1234"));

    QTRY_COMPARE(dialog.result(), static_cast<int>(QDialog::Accepted));
    QCOMPARE(driver.titles(), QStringList{QStringLiteral("알림")});
    QVERIFY(!m_authSession.currentUser().m_isPasswordChangeRequired);
    QCOMPARE(m_repository->user(QStringLiteral("admin")).m_passwordHash, QStringLiteral("hashed:New1234"));
}

QTEST_MAIN(SagePasswordChangeDlgTest)

#include "SagePasswordChangeDlgTest.moc"
