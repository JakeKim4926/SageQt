#include "infra/db/SageUserRepository.h"

#include "SageDefine.h"
#include "infra/db/SageDbConfig.h"
#include "infra/db/SageDbConnection.h"
#include "infra/db/SageSchemaInitializer.h"

#include <QObject>
#include <QSqlQuery>
#include <QString>
#include <QTemporaryDir>
#include <QTest>

class SageUserRepositoryTest : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void insertThenSelectReturnsSameUser();
    void selectUnknownReturnsNoUser();
    void existsByLoginIdReflectsInsert();
    void updatePasswordStoresHashAndClearsChangeRequired();
    void insertDuplicateLoginIdFails();

private:
    SageUserDto sampleUser() const;

private:
    QTemporaryDir m_temporaryDirectory;
    SageDbConfig m_config;
};

void SageUserRepositoryTest::init()
{
    QVERIFY(m_temporaryDirectory.isValid());
    m_config.m_databasePath = m_temporaryDirectory.filePath(QStringLiteral("%1.db").arg(QTest::currentTestFunction()));
    QString error;
    QVERIFY2(SageSchemaInitializer(m_config).prepare(error), qPrintable(error));
}

SageUserDto SageUserRepositoryTest::sampleUser() const
{
    SageUserDto user;
    user.m_loginId = QStringLiteral("admin");
    user.m_passwordHash = QStringLiteral("pbkdf2-sha256$1000$c2FsdA==$a2V5");
    user.m_role = SageUserRole::Admin;
    user.m_isPasswordChangeRequired = true;
    return user;
}

void SageUserRepositoryTest::insertThenSelectReturnsSameUser()
{
    const SageUserRepository repository(m_config);
    int userId = 0;
    QString error;
    QVERIFY2(repository.insert(sampleUser(), userId, error), qPrintable(error));
    QVERIFY(userId > 0);

    std::optional<SageUserDto> user;
    QVERIFY2(repository.selectByLoginId(QStringLiteral("admin"), user, error), qPrintable(error));

    QVERIFY(user.has_value());
    QCOMPARE(user->m_userId, userId);
    QCOMPARE(user->m_loginId, QStringLiteral("admin"));
    QCOMPARE(user->m_passwordHash, QStringLiteral("pbkdf2-sha256$1000$c2FsdA==$a2V5"));
    QCOMPARE(user->m_role, SageUserRole::Admin);
    QCOMPARE(user->m_isPasswordChangeRequired, true);
}

void SageUserRepositoryTest::selectUnknownReturnsNoUser()
{
    const SageUserRepository repository(m_config);
    std::optional<SageUserDto> user;
    QString error;

    QVERIFY2(repository.selectByLoginId(QStringLiteral("nobody"), user, error), qPrintable(error));

    QVERIFY(!user.has_value());
}

void SageUserRepositoryTest::existsByLoginIdReflectsInsert()
{
    const SageUserRepository repository(m_config);
    bool exists = true;
    QString error;
    QVERIFY2(repository.existsByLoginId(QStringLiteral("admin"), exists, error), qPrintable(error));
    QCOMPARE(exists, false);

    int userId = 0;
    QVERIFY2(repository.insert(sampleUser(), userId, error), qPrintable(error));
    QVERIFY2(repository.existsByLoginId(QStringLiteral("admin"), exists, error), qPrintable(error));

    QCOMPARE(exists, true);
}

void SageUserRepositoryTest::updatePasswordStoresHashAndClearsChangeRequired()
{
    const SageUserRepository repository(m_config);
    int userId = 0;
    QString error;
    QVERIFY2(repository.insert(sampleUser(), userId, error), qPrintable(error));

    QVERIFY2(repository.updatePassword(userId, QStringLiteral("new-hash"), error), qPrintable(error));

    SageDbConnection connection(m_config);
    QVERIFY2(connection.open(error), qPrintable(error));
    QSqlQuery query(connection.database());
    QVERIFY(query.exec(QStringLiteral("SELECT pw_hash, must_change_pw FROM SageUser WHERE login_id = 'admin';")));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toString(), QStringLiteral("new-hash"));
    QCOMPARE(query.value(1).toInt(), 0);
}

void SageUserRepositoryTest::insertDuplicateLoginIdFails()
{
    const SageUserRepository repository(m_config);
    int userId = 0;
    QString error;
    QVERIFY2(repository.insert(sampleUser(), userId, error), qPrintable(error));

    QCOMPARE(repository.insert(sampleUser(), userId, error), false);

    QVERIFY(error.startsWith(QStringLiteral("SQLite SQL 실행 실패.")));
}

QTEST_GUILESS_MAIN(SageUserRepositoryTest)

#include "SageUserRepositoryTest.moc"
