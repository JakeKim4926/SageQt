#include "infra/db/SageDbConfig.h"

#include <QDir>
#include <QStandardPaths>

bool SageDbConfig::buildDefaultConfig(SageDbConfig& outConfig, QString& outError)
{
    const QString dataDirectory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dataDirectory.isEmpty()) {
        outError = SAGE_DB_ERROR_DIRECTORY_NOT_FOUND.arg(SAGE_DB_FILE_NAME);
        return false;
    }

    outConfig.m_databasePath = QDir(dataDirectory).filePath(SAGE_DB_FILE_NAME);
    return true;
}
