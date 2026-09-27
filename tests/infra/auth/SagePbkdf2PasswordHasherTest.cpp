#include "infra/auth/SagePbkdf2PasswordHasher.h"

#include <QByteArray>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTest>

namespace {

constexpr int SAGE_TEST_ITERATIONS = 1000;

}

class SagePbkdf2PasswordHasherTest : public QObject
{
    Q_OBJECT

private slots:
    void defaultIterationsFollowOwaspRecommendation();
    void hashUsesStoredFormat();
    void verifyAcceptsCorrectPassword();
    void verifyRejectsWrongPassword();
    void samePasswordGetsDifferentHashes();
    void verifyUsesStoredIterations();
    void verifyRejectsMalformedHash();
};

void SagePbkdf2PasswordHasherTest::defaultIterationsFollowOwaspRecommendation()
{
    QCOMPARE(SAGE_PASSWORD_HASH_ITERATIONS, 600000);
}

void SagePbkdf2PasswordHasherTest::hashUsesStoredFormat()
{
    const SagePbkdf2PasswordHasher hasher(SAGE_TEST_ITERATIONS);

    const QStringList parts = hasher.hashPassword(QStringLiteral("pass1234")).split(QLatin1Char('$'));

    QCOMPARE(parts.size(), 4);
    QCOMPARE(parts.at(0), QStringLiteral("pbkdf2-sha256"));
    QCOMPARE(parts.at(1), QStringLiteral("1000"));
    QCOMPARE(QByteArray::fromBase64(parts.at(2).toLatin1()).size(), 16);
    QCOMPARE(QByteArray::fromBase64(parts.at(3).toLatin1()).size(), 32);
}

void SagePbkdf2PasswordHasherTest::verifyAcceptsCorrectPassword()
{
    const SagePbkdf2PasswordHasher hasher(SAGE_TEST_ITERATIONS);
    const QString stored = hasher.hashPassword(QStringLiteral("pass1234"));

    QCOMPARE(hasher.verifyPassword(QStringLiteral("pass1234"), stored), true);
}

void SagePbkdf2PasswordHasherTest::verifyRejectsWrongPassword()
{
    const SagePbkdf2PasswordHasher hasher(SAGE_TEST_ITERATIONS);
    const QString stored = hasher.hashPassword(QStringLiteral("pass1234"));

    QCOMPARE(hasher.verifyPassword(QStringLiteral("Pass1234"), stored), false);
}

void SagePbkdf2PasswordHasherTest::samePasswordGetsDifferentHashes()
{
    const SagePbkdf2PasswordHasher hasher(SAGE_TEST_ITERATIONS);

    QVERIFY(hasher.hashPassword(QStringLiteral("pass1234")) != hasher.hashPassword(QStringLiteral("pass1234")));
}

void SagePbkdf2PasswordHasherTest::verifyUsesStoredIterations()
{
    const SagePbkdf2PasswordHasher oldHasher(SAGE_TEST_ITERATIONS);
    const SagePbkdf2PasswordHasher newHasher(SAGE_TEST_ITERATIONS * 2);
    const QString stored = oldHasher.hashPassword(QStringLiteral("pass1234"));

    QCOMPARE(newHasher.verifyPassword(QStringLiteral("pass1234"), stored), true);
}

void SagePbkdf2PasswordHasherTest::verifyRejectsMalformedHash()
{
    const SagePbkdf2PasswordHasher hasher(SAGE_TEST_ITERATIONS);

    QCOMPARE(hasher.verifyPassword(QStringLiteral("pass1234"), QString()), false);
    QCOMPARE(hasher.verifyPassword(QStringLiteral("pass1234"), QStringLiteral("sha256$1000$abc$def")), false);
    QCOMPARE(hasher.verifyPassword(QStringLiteral("pass1234"), QStringLiteral("pbkdf2-sha256$zero$abc$def")), false);
    QCOMPARE(hasher.verifyPassword(QStringLiteral("pass1234"), QStringLiteral("pbkdf2-sha256$1000$$")), false);
}

QTEST_GUILESS_MAIN(SagePbkdf2PasswordHasherTest)

#include "SagePbkdf2PasswordHasherTest.moc"
