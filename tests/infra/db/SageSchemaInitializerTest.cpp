#include "infra/db/SageSchemaInitializer.h"

#include "infra/db/SageDbConfig.h"
#include "infra/db/SageDbConnection.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QTemporaryDir>
#include <QTest>

class SageSchemaInitializerTest : public QObject
{
    Q_OBJECT

private slots:
    void prepareCreatesMissingFoldersAndTable();
    void prepareCanRunTwice();
    void prepareCreatesSageUserColumnsInOrder();
    void prepareFailsWhenFolderPathIsAFile();
};

void SageSchemaInitializerTest::prepareCreatesMissingFoldersAndTable()
{
    const QTemporaryDir temporaryDirectory;
    QVERIFY(temporaryDirectory.isValid());
    SageDbConfig config;
    config.m_databasePath = temporaryDirectory.filePath(QStringLiteral("nested/deeper/test.db"));
    const SageSchemaInitializer initializer(config);
    QString error;

    QVERIFY2(initializer.prepare(error), qPrintable(error));

    QVERIFY(QFileInfo::exists(config.m_databasePath));
    SageDbConnection connection(config);
    QVERIFY2(connection.open(error), qPrintable(error));
    QSqlQuery query(connection.database());
    QVERIFY(query.exec(QStringLiteral("SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'SageUser';")));
    QVERIFY(query.next());
}

void SageSchemaInitializerTest::prepareCanRunTwice()
{
    const QTemporaryDir temporaryDirectory;
    SageDbConfig config;
    config.m_databasePath = temporaryDirectory.filePath(QStringLiteral("test.db"));
    const SageSchemaInitializer initializer(config);
    QString error;

    QVERIFY2(initializer.prepare(error), qPrintable(error));
    QVERIFY2(initializer.prepare(error), qPrintable(error));
}

void SageSchemaInitializerTest::prepareCreatesSageUserColumnsInOrder()
{
    const QTemporaryDir temporaryDirectory;
    SageDbConfig config;
    config.m_databasePath = temporaryDirectory.filePath(QStringLiteral("test.db"));
    QString error;
    QVERIFY2(SageSchemaInitializer(config).prepare(error), qPrintable(error));

    SageDbConnection connection(config);
    QVERIFY2(connection.open(error), qPrintable(error));
    QSqlQuery query(connection.database());
    QVERIFY(query.exec(QStringLiteral("PRAGMA table_info(SageUser);")));
    QStringList columns;
    while (query.next()) {
        columns.append(query.value(1).toString());
    }

    const QStringList expected = {QStringLiteral("user_id"), QStringLiteral("login_id"), QStringLiteral("pw_hash"),
                                  QStringLiteral("role"), QStringLiteral("must_change_pw")};
    QCOMPARE(columns, expected);
}

void SageSchemaInitializerTest::prepareFailsWhenFolderPathIsAFile()
{
    const QTemporaryDir temporaryDirectory;
    const QString blockerPath = temporaryDirectory.filePath(QStringLiteral("blocker"));
    QFile blocker(blockerPath);
    QVERIFY(blocker.open(QIODevice::WriteOnly));
    blocker.close();
    SageDbConfig config;
    config.m_databasePath = QDir(blockerPath).filePath(QStringLiteral("test.db"));
    QString error;

    QCOMPARE(SageSchemaInitializer(config).prepare(error), false);

    QVERIFY(error.contains(QStringLiteral("동일한 이름의 파일이 존재하여 폴더를 만들 수 없습니다.")));
}

QTEST_GUILESS_MAIN(SageSchemaInitializerTest)

#include "SageSchemaInitializerTest.moc"
