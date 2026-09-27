#pragma once

#include "infra/db/SageDbDefine.h"

#include <QString>

struct SageDbConfig
{
    QString m_databasePath;
    int m_busyTimeoutMs = SAGE_DB_BUSY_TIMEOUT_MS;

    static bool buildDefaultConfig(SageDbConfig& outConfig, QString& outError);
};
