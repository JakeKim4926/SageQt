#pragma once

#include "infra/db/SageDbConfig.h"

#include <QSqlDatabase>
#include <QString>

class SageDbConnection
{
public:
    explicit SageDbConnection(const SageDbConfig& config);
    ~SageDbConnection();

    SageDbConnection(const SageDbConnection&) = delete;
    SageDbConnection& operator=(const SageDbConnection&) = delete;

    bool open(QString& outError);
    bool execute(const QString& sql, QString& outError) const;
    QSqlDatabase database() const;

private:
    static bool ensureDirectory(const QString& databasePath, QString& outError);
    static void closeDatabase(const QString& connectionName);

private:
    SageDbConfig m_config;
    QString m_connectionName;
};
