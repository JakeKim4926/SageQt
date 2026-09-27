#include "infra/db/SageDbConnection.h"

#include "infra/db/SageDbConfig.h"
#include "infra/db/SageSchemaInitializer.h"

#include <QFuture>
#include <QList>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QTemporaryDir>
#include <QTest>
#include <QtConcurrentRun>

#include <utility>

namespace {

int foreignKeysValue(const SageDbConnection& connection)
{
    QSqlQuery query(connection.database());
    if (!query.exec(QStringLiteral("PRAGMA foreign_keys;")) || !query.next()) {
        return -1;
    }
    return query.value(0).toInt();
}

bool countUsersInOwnConnection(const SageDbConfig& config)
{
    SageDbConnection connection(config);
    QString error;
    if (!connection.open(error)) {
        return false;
    }
    QSqlQuery query(connection.database());
    return query.exec(QStringLiteral("SELECT COUNT(*) FROM SageUser;")) && query.next();
}

}

class SageDbConnectionTest : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void openEnablesForeignKeysOnEveryConnection();
    void openAppliesBusyTimeout();
    void openConnectionsHaveUniqueNames();
    void destructorRemovesConnection();
    void concurrentTasksOpenTheirOwnConnections();

private:
    QTemporaryDir m_temporaryDirectory;
    SageDbConfig m_config;
};

void SageDbConnectionTest::init()
{
    QVERIFY(m_temporaryDirectory.isValid());
    m_config.m_databasePath = m_temporaryDirectory.filePath(QStringLiteral("test.db"));
}

void SageDbConnectionTest::openEnablesForeignKeysOnEveryConnection()
{
    QString error;
    {
        SageDbConnection first(m_config);
        QVERIFY2(first.open(error), qPrintable(error));
        QCOMPARE(foreignKeysValue(first), 1);
    }

    SageDbConnection second(m_config);
    SageDbConnection third(m_config);
    QVERIFY2(second.open(error), qPrintable(error));
    QVERIFY2(third.open(error), qPrintable(error));
    QCOMPARE(foreignKeysValue(second), 1);
    QCOMPARE(foreignKeysValue(third), 1);
}

void SageDbConnectionTest::openAppliesBusyTimeout()
{
    SageDbConnection connection(m_config);
    QString error;

    QVERIFY2(connection.open(error), qPrintable(error));

    QCOMPARE(connection.database().connectOptions(), QStringLiteral("QSQLITE_BUSY_TIMEOUT=5000"));
}

void SageDbConnectionTest::openConnectionsHaveUniqueNames()
{
    SageDbConnection first(m_config);
    SageDbConnection second(m_config);
    QString error;

    QVERIFY2(first.open(error), qPrintable(error));
    QVERIFY2(second.open(error), qPrintable(error));

    QVERIFY(first.database().connectionName() != second.database().connectionName());
}

void SageDbConnectionTest::destructorRemovesConnection()
{
    QString connectionName;
    {
        SageDbConnection connection(m_config);
        QString error;
        QVERIFY2(connection.open(error), qPrintable(error));
        connectionName = connection.database().connectionName();
        QVERIFY(QSqlDatabase::contains(connectionName));
    }

    QVERIFY(!QSqlDatabase::contains(connectionName));
}

void SageDbConnectionTest::concurrentTasksOpenTheirOwnConnections()
{
    QString error;
    QVERIFY2(SageSchemaInitializer(m_config).prepare(error), qPrintable(error));

    QList<QFuture<bool>> tasks;
    for (int index = 0; index < 4; ++index) {
        tasks.append(QtConcurrent::run(countUsersInOwnConnection, m_config));
    }

    for (const QFuture<bool>& task : std::as_const(tasks)) {
        QVERIFY(task.result());
    }
}

QTEST_GUILESS_MAIN(SageDbConnectionTest)

#include "SageDbConnectionTest.moc"
