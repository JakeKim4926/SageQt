#include "infra/db/SageSchemaInitializer.h"

#include "infra/db/SageDbConnection.h"

namespace {

const QString SAGE_DB_SQL_CREATE_SAGE_USER = QStringLiteral("CREATE TABLE IF NOT EXISTS SageUser ("
                                                            "    user_id   INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                            "    login_id  TEXT NOT NULL UNIQUE,"
                                                            "    pw_hash   TEXT NOT NULL,"
                                                            "    role      INTEGER NOT NULL DEFAULT 0,"
                                                            "    must_change_pw INTEGER NOT NULL DEFAULT 0,"
                                                            "    CHECK (role >= 0),"
                                                            "    CHECK (must_change_pw IN (0, 1))"
                                                            ");");

}

SageSchemaInitializer::SageSchemaInitializer(const SageDbConfig& config)
    : m_config(config)
{
}

bool SageSchemaInitializer::prepare(QString& outError) const
{
    SageDbConnection connection(m_config);
    if (!connection.open(outError)) {
        return false;
    }
    return connection.execute(SAGE_DB_SQL_CREATE_SAGE_USER, outError);
}
