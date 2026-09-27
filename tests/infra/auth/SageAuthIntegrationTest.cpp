#include "SageDefine.h"
#include "core/auth/SageUserService.h"
#include "infra/auth/SagePbkdf2PasswordHasher.h"
#include "infra/db/SageDbConfig.h"
#include "infra/db/SageDbConnection.h"
#include "infra/db/SageSchemaInitializer.h"
#include "infra/db/SageUserRepository.h"

#include <QObject>
#include <QSqlQuery>
#include <QString>
#include <QTemporaryDir>
#include <QTest>

namespace {

constexpr int SAGE_TEST_ITERATIONS = 1000;

}

class SageAuthIntegrationTest : public QObject
{
    Q_OBJECT

private slots:
    void initialAdminLogsInAndMustChangePassword();
    void defaultAdminIsCreatedOnlyOnce();
};

void SageAuthIntegrationTest::initialAdminLogsInAndMustChangePassword()
{
    const QTemporaryDir temporaryDirectory;
    SageDbConfig config;
    config.m_databasePath = temporaryDirectory.filePath(QStringLiteral("test.db"));
    QString error;
    QVERIFY2(SageSchemaInitializer(config).prepare(error), qPrintable(error));
    const SageUserRepository repository(config);
    const SagePbkdf2PasswordHasher hasher(SAGE_TEST_ITERATIONS);
    const SageUserService service(repository, hasher);

    std::optional<QString> initialPassword;
    QVERIFY2(service.ensureDefaultAdmin(initialPassword, error), qPrintable(error));
    QVERIFY(initialPassword.has_value());

    std::optional<SageUserDto> user;
    QVERIFY2(service.login(QStringLiteral("admin"), initialPassword.value(), user, error), qPrintable(error));
    QVERIFY(user.has_value());
    QCOMPARE(user->m_role, SageUserRole::Admin);
    QCOMPARE(user->m_isPasswordChangeRequired, true);

    QVERIFY2(service.changePassword(user->m_userId, QStringLiteral("newPass1"), error), qPrintable(error));

    QVERIFY2(service.login(QStringLiteral("admin"), initialPassword.value(), user, error), qPrintable(error));
    QVERIFY(!user.has_value());
    QVERIFY2(service.login(QStringLiteral("admin"), QStringLiteral("newPass1"), user, error), qPrintable(error));
    QVERIFY(user.has_value());
    QCOMPARE(user->m_isPasswordChangeRequired, false);
}

void SageAuthIntegrationTest::defaultAdminIsCreatedOnlyOnce()
{
    const QTemporaryDir temporaryDirectory;
    SageDbConfig config;
    config.m_databasePath = temporaryDirectory.filePath(QStringLiteral("test.db"));
    QString error;
    QVERIFY2(SageSchemaInitializer(config).prepare(error), qPrintable(error));
    const SageUserRepository repository(config);
    const SagePbkdf2PasswordHasher hasher(SAGE_TEST_ITERATIONS);
    const SageUserService service(repository, hasher);
    std::optional<QString> initialPassword;

    QVERIFY2(service.ensureDefaultAdmin(initialPassword, error), qPrintable(error));
    QVERIFY2(service.ensureDefaultAdmin(initialPassword, error), qPrintable(error));

    QVERIFY(!initialPassword.has_value());
    SageDbConnection connection(config);
    QVERIFY2(connection.open(error), qPrintable(error));
    QSqlQuery query(connection.database());
    QVERIFY(query.exec(QStringLiteral("SELECT COUNT(*) FROM SageUser;")));
    QVERIFY(query.next());
    QCOMPARE(query.value(0).toInt(), 1);
}

QTEST_GUILESS_MAIN(SageAuthIntegrationTest)

#include "SageAuthIntegrationTest.moc"
