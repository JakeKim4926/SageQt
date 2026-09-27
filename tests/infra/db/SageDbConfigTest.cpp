#include "infra/db/SageDbConfig.h"

#include <QCoreApplication>
#include <QDir>
#include <QObject>
#include <QStandardPaths>
#include <QString>
#include <QTest>

class SageDbConfigTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void buildDefaultConfigUsesAppDataFolder();
    void buildDefaultConfigIsNotInApplicationFolder();
    void defaultBusyTimeoutIsFiveSeconds();
};

void SageDbConfigTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
    QCoreApplication::setOrganizationName(QStringLiteral("Sage"));
    QCoreApplication::setApplicationName(QStringLiteral("SageQt"));
}

void SageDbConfigTest::buildDefaultConfigUsesAppDataFolder()
{
    SageDbConfig config;
    QString error;

    QCOMPARE(SageDbConfig::buildDefaultConfig(config, error), true);

    const QString dataDirectory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QCOMPARE(config.m_databasePath, QDir(dataDirectory).filePath(QStringLiteral("sageqt.db")));
    QVERIFY(error.isEmpty());
}

void SageDbConfigTest::buildDefaultConfigIsNotInApplicationFolder()
{
    SageDbConfig config;
    QString error;

    QCOMPARE(SageDbConfig::buildDefaultConfig(config, error), true);

    QVERIFY(!config.m_databasePath.startsWith(QCoreApplication::applicationDirPath()));
}

void SageDbConfigTest::defaultBusyTimeoutIsFiveSeconds()
{
    const SageDbConfig config;

    QCOMPARE(config.m_busyTimeoutMs, 5000);
}

QTEST_GUILESS_MAIN(SageDbConfigTest)

#include "SageDbConfigTest.moc"
