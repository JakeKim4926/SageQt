#include "infra/db/SageDbConnection.h"

#include "infra/db/SageDbDefine.h"

#include <QDir>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>

namespace {

const QString SAGE_DB_SQL_ENABLE_FOREIGN_KEYS = QStringLiteral("PRAGMA foreign_keys = ON;");

}

SageDbConnection::SageDbConnection(const SageDbConfig& config)
    : m_config(config)
    , m_connectionName(SAGE_DB_CONNECTION_NAME_PREFIX + QUuid::createUuid().toString(QUuid::WithoutBraces))
{
}

SageDbConnection::~SageDbConnection()
{
    closeDatabase(m_connectionName);
    if (QSqlDatabase::contains(m_connectionName)) {
        QSqlDatabase::removeDatabase(m_connectionName);
    }
}

bool SageDbConnection::open(QString& outError)
{
    if (!ensureDirectory(m_config.m_databasePath, outError)) {
        return false;
    }

    QSqlDatabase database = QSqlDatabase::addDatabase(SAGE_DB_DRIVER, m_connectionName);
    database.setDatabaseName(m_config.m_databasePath);
    database.setConnectOptions(SAGE_DB_BUSY_TIMEOUT_OPTION.arg(m_config.m_busyTimeoutMs));
    if (!database.open()) {
        outError = SAGE_DB_ERROR_OPEN.arg(m_config.m_databasePath, database.lastError().text());
        return false;
    }

    return execute(SAGE_DB_SQL_ENABLE_FOREIGN_KEYS, outError);
}

bool SageDbConnection::execute(const QString& sql, QString& outError) const
{
    QSqlQuery query(database());
    if (!query.exec(sql)) {
        outError = SAGE_DB_ERROR_EXECUTE.arg(query.lastError().text());
        return false;
    }
    return true;
}

QSqlDatabase SageDbConnection::database() const
{
    return QSqlDatabase::database(m_connectionName, false);
}

bool SageDbConnection::ensureDirectory(const QString& databasePath, QString& outError)
{
    const QString directoryPath = QFileInfo(databasePath).absolutePath();
    const QFileInfo directory(directoryPath);
    if (directory.exists() && !directory.isDir()) {
        outError = SAGE_DB_ERROR_DIRECTORY_IS_FILE.arg(directoryPath);
        return false;
    }
    if (!QDir().mkpath(directoryPath)) {
        outError = SAGE_DB_ERROR_CREATE_DIRECTORY.arg(directoryPath);
        return false;
    }
    return true;
}

void SageDbConnection::closeDatabase(const QString& connectionName)
{
    QSqlDatabase database = QSqlDatabase::database(connectionName, false);
    if (database.isValid()) {
        database.close();
    }
}
