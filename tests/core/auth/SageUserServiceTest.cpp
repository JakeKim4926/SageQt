#include "core/auth/SageUserService.h"

#include "SageDefine.h"
#include "core/auth/ISagePasswordHasher.h"
#include "core/auth/ISageUserRepository.h"

#include <QHash>
#include <QObject>
#include <QString>
#include <QTest>

#include <utility>

namespace {

class SageFakePasswordHasher final : public ISagePasswordHasher
{
public:
    QString hashPassword(const QString& password) const override
    {
        return QStringLiteral("hashed:") + password;
    }

    bool verifyPassword(const QString& password, const QString& storedHash) const override
    {
        return storedHash == hashPassword(password);
    }
};

class SageFakeUserRepository final : public ISageUserRepository
{
public:
    bool selectByLoginId(const QString& loginId, std::optional<SageUserDto>& outUser, QString& outError) const override
    {
        if (m_isFailing) {
            outError = QStringLiteral("db down");
            return false;
        }
        outUser.reset();
        if (m_users.contains(loginId)) {
            outUser = m_users.value(loginId);
        }
        return true;
    }

    bool existsByLoginId(const QString& loginId, bool& outExists, QString& outError) const override
    {
        if (m_isFailing) {
            outError = QStringLiteral("db down");
            return false;
        }
        outExists = m_users.contains(loginId);
        return true;
    }

    bool insert(const SageUserDto& user, int& outUserId, QString&) const override
    {
        ++m_insertCount;
        SageUserDto stored = user;
        stored.m_userId = m_insertCount;
        m_users.insert(user.m_loginId, stored);
        outUserId = stored.m_userId;
        return true;
    }

    bool updatePassword(int userId, const QString& passwordHash, QString&) const override
    {
        for (SageUserDto& user : m_users) {
            if (user.m_userId == userId) {
                user.m_passwordHash = passwordHash;
                user.m_isPasswordChangeRequired = false;
            }
        }
        m_lastUpdatedHash = passwordHash;
        return true;
    }

    void addUser(const SageUserDto& user)
    {
        m_users.insert(user.m_loginId, user);
    }

    void setFailing(bool isFailing)
    {
        m_isFailing = isFailing;
    }

    int insertCount() const
    {
        return m_insertCount;
    }

    QString lastUpdatedHash() const
    {
        return m_lastUpdatedHash;
    }

    SageUserDto user(const QString& loginId) const
    {
        return m_users.value(loginId);
    }

private:
    mutable QHash<QString, SageUserDto> m_users;
    mutable int m_insertCount = 0;
    mutable QString m_lastUpdatedHash;
    bool m_isFailing = false;
};

SageUserDto storedUser(int userId, const QString& loginId, const QString& password)
{
    SageUserDto user;
    user.m_userId = userId;
    user.m_loginId = loginId;
    user.m_passwordHash = QStringLiteral("hashed:") + password;
    return user;
}

}

class SageUserServiceTest : public QObject
{
    Q_OBJECT

private slots:
    void loginSucceedsWithCorrectPassword();
    void loginFailsWithWrongPassword();
    void loginFailsWithUnknownId();
    void loginReportsRepositoryError();
    void changePasswordRejectsEmpty();
    void changePasswordRejectsThreeCharacters();
    void changePasswordAcceptsFourCharacters();
    void changePasswordAcceptsFifteenCharacters();
    void changePasswordRejectsSixteenCharacters();
    void changePasswordRejectsNonAlphanumeric();
    void changePasswordRejectsKorean();
    void changePasswordStoresHashAndClearsChangeRequired();
    void ensureDefaultAdminCreatesAdminOnce();
    void ensureDefaultAdminUsesLengthAndAlphabet();
    void ensureDefaultAdminReportsRepositoryError();
};

void SageUserServiceTest::loginSucceedsWithCorrectPassword()
{
    SageFakeUserRepository repository;
    repository.addUser(storedUser(3, QStringLiteral("kim"), QStringLiteral("pass1234")));
    const SageFakePasswordHasher hasher;
    const SageUserService service(repository, hasher);
    std::optional<SageUserDto> user;
    QString error;

    QCOMPARE(service.login(QStringLiteral("kim"), QStringLiteral("pass1234"), user, error), true);

    QVERIFY(user.has_value());
    QCOMPARE(user->m_userId, 3);
}

void SageUserServiceTest::loginFailsWithWrongPassword()
{
    SageFakeUserRepository repository;
    repository.addUser(storedUser(3, QStringLiteral("kim"), QStringLiteral("pass1234")));
    const SageFakePasswordHasher hasher;
    const SageUserService service(repository, hasher);
    std::optional<SageUserDto> user;
    QString error;

    QCOMPARE(service.login(QStringLiteral("kim"), QStringLiteral("wrong"), user, error), true);

    QVERIFY(!user.has_value());
    QVERIFY(error.isEmpty());
}

void SageUserServiceTest::loginFailsWithUnknownId()
{
    const SageFakeUserRepository repository;
    const SageFakePasswordHasher hasher;
    const SageUserService service(repository, hasher);
    std::optional<SageUserDto> user;
    QString error;

    QCOMPARE(service.login(QStringLiteral("nobody"), QStringLiteral("pass1234"), user, error), true);

    QVERIFY(!user.has_value());
}

void SageUserServiceTest::loginReportsRepositoryError()
{
    SageFakeUserRepository repository;
    repository.setFailing(true);
    const SageFakePasswordHasher hasher;
    const SageUserService service(repository, hasher);
    std::optional<SageUserDto> user;
    QString error;

    QCOMPARE(service.login(QStringLiteral("kim"), QStringLiteral("pass1234"), user, error), false);

    QCOMPARE(error, QStringLiteral("db down"));
}

void SageUserServiceTest::changePasswordRejectsEmpty()
{
    const SageFakeUserRepository repository;
    const SageFakePasswordHasher hasher;
    const SageUserService service(repository, hasher);
    QString error;

    QCOMPARE(service.changePassword(1, QString(), error), false);

    QCOMPARE(error, QStringLiteral("비밀번호를 입력하세요."));
}

void SageUserServiceTest::changePasswordRejectsThreeCharacters()
{
    const SageFakeUserRepository repository;
    const SageFakePasswordHasher hasher;
    const SageUserService service(repository, hasher);
    QString error;

    QCOMPARE(service.changePassword(1, QStringLiteral("abc"), error), false);

    QCOMPARE(error, QStringLiteral("비밀번호는 4자 이상이어야 합니다."));
}

void SageUserServiceTest::changePasswordAcceptsFourCharacters()
{
    const SageFakeUserRepository repository;
    const SageFakePasswordHasher hasher;
    const SageUserService service(repository, hasher);
    QString error;

    QVERIFY2(service.changePassword(1, QStringLiteral("abc1"), error), qPrintable(error));
}

void SageUserServiceTest::changePasswordAcceptsFifteenCharacters()
{
    const SageFakeUserRepository repository;
    const SageFakePasswordHasher hasher;
    const SageUserService service(repository, hasher);
    QString error;

    QVERIFY2(service.changePassword(1, QStringLiteral("abcdefghij12345"), error), qPrintable(error));
}

void SageUserServiceTest::changePasswordRejectsSixteenCharacters()
{
    const SageFakeUserRepository repository;
    const SageFakePasswordHasher hasher;
    const SageUserService service(repository, hasher);
    QString error;

    QCOMPARE(service.changePassword(1, QStringLiteral("abcdefghij123456"), error), false);

    QCOMPARE(error, QStringLiteral("비밀번호는 15자 이하이어야 합니다."));
}

void SageUserServiceTest::changePasswordRejectsNonAlphanumeric()
{
    const SageFakeUserRepository repository;
    const SageFakePasswordHasher hasher;
    const SageUserService service(repository, hasher);
    QString error;

    QCOMPARE(service.changePassword(1, QStringLiteral("abcd!"), error), false);

    QCOMPARE(error, QStringLiteral("비밀번호는 영문과 숫자만 사용할 수 있습니다."));
}

void SageUserServiceTest::changePasswordRejectsKorean()
{
    const SageFakeUserRepository repository;
    const SageFakePasswordHasher hasher;
    const SageUserService service(repository, hasher);
    QString error;

    QCOMPARE(service.changePassword(1, QStringLiteral("비밀번호12"), error), false);

    QCOMPARE(error, QStringLiteral("비밀번호는 영문과 숫자만 사용할 수 있습니다."));
}

void SageUserServiceTest::changePasswordStoresHashAndClearsChangeRequired()
{
    SageFakeUserRepository repository;
    SageUserDto user = storedUser(5, QStringLiteral("kim"), QStringLiteral("old1"));
    user.m_isPasswordChangeRequired = true;
    repository.addUser(user);
    const SageFakePasswordHasher hasher;
    const SageUserService service(repository, hasher);
    QString error;

    QVERIFY2(service.changePassword(5, QStringLiteral("new1"), error), qPrintable(error));

    QCOMPARE(repository.lastUpdatedHash(), QStringLiteral("hashed:new1"));
    QCOMPARE(repository.user(QStringLiteral("kim")).m_isPasswordChangeRequired, false);
}

void SageUserServiceTest::ensureDefaultAdminCreatesAdminOnce()
{
    const SageFakeUserRepository repository;
    const SageFakePasswordHasher hasher;
    const SageUserService service(repository, hasher);
    std::optional<QString> initialPassword;
    QString error;

    QVERIFY2(service.ensureDefaultAdmin(initialPassword, error), qPrintable(error));
    QVERIFY(initialPassword.has_value());
    const SageUserDto admin = repository.user(QStringLiteral("admin"));
    QCOMPARE(admin.m_role, SageUserRole::Admin);
    QCOMPARE(admin.m_isPasswordChangeRequired, true);
    QVERIFY(hasher.verifyPassword(initialPassword.value(), admin.m_passwordHash));

    QVERIFY2(service.ensureDefaultAdmin(initialPassword, error), qPrintable(error));
    QVERIFY(!initialPassword.has_value());
    QCOMPARE(repository.insertCount(), 1);
}

void SageUserServiceTest::ensureDefaultAdminUsesLengthAndAlphabet()
{
    const SageFakeUserRepository repository;
    const SageFakePasswordHasher hasher;
    const SageUserService service(repository, hasher);
    std::optional<QString> initialPassword;
    QString error;

    QVERIFY2(service.ensureDefaultAdmin(initialPassword, error), qPrintable(error));

    QVERIFY(initialPassword.has_value());
    QCOMPARE(initialPassword->size(), 14);
    const QString alphabet = QStringLiteral("ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz23456789");
    for (const QChar character : std::as_const(initialPassword.value())) {
        QVERIFY(alphabet.contains(character));
    }
}

void SageUserServiceTest::ensureDefaultAdminReportsRepositoryError()
{
    SageFakeUserRepository repository;
    repository.setFailing(true);
    const SageFakePasswordHasher hasher;
    const SageUserService service(repository, hasher);
    std::optional<QString> initialPassword;
    QString error;

    QCOMPARE(service.ensureDefaultAdmin(initialPassword, error), false);

    QVERIFY(!initialPassword.has_value());
    QCOMPARE(repository.insertCount(), 0);
}

QTEST_GUILESS_MAIN(SageUserServiceTest)

#include "SageUserServiceTest.moc"
