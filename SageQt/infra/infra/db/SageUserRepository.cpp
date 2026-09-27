#include "infra/db/SageUserRepository.h"

#include "infra/db/SageDbConnection.h"
#include "infra/db/SageDbDefine.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

namespace {

const QString SAGE_USER_SQL_INSERT = QStringLiteral("INSERT INTO SageUser "
                                                    "(login_id, pw_hash, role, must_change_pw) "
                                                    "VALUES (?, ?, ?, ?);");
const QString SAGE_USER_SQL_SELECT_BY_LOGIN_ID =
    QStringLiteral("SELECT user_id, login_id, pw_hash, role, must_change_pw "
                   "FROM SageUser "
                   "WHERE login_id = ?;");
const QString SAGE_USER_SQL_UPDATE_PASSWORD = QStringLiteral("UPDATE SageUser SET pw_hash = ?, must_change_pw = 0 "
                                                             "WHERE user_id = ?;");
const QString SAGE_USER_SQL_COUNT_BY_LOGIN_ID = QStringLiteral("SELECT COUNT(*) FROM SageUser WHERE login_id = ?;");

constexpr int SAGE_USER_COLUMN_USER_ID = 0;
constexpr int SAGE_USER_COLUMN_LOGIN_ID = 1;
constexpr int SAGE_USER_COLUMN_PW_HASH = 2;
constexpr int SAGE_USER_COLUMN_ROLE = 3;
constexpr int SAGE_USER_COLUMN_MUST_CHANGE_PW = 4;
constexpr int SAGE_USER_COLUMN_COUNT = 0;
constexpr int SAGE_USER_MUST_CHANGE_PW_REQUIRED = 1;
constexpr int SAGE_USER_MUST_CHANGE_PW_NONE = 0;

bool executePrepared(QSqlQuery& query, QString& outError)
{
    if (!query.exec()) {
        outError = SAGE_DB_ERROR_EXECUTE.arg(query.lastError().text());
        return false;
    }
    return true;
}

bool prepare(QSqlQuery& query, const QString& sql, QString& outError)
{
    if (!query.prepare(sql)) {
        outError = SAGE_DB_ERROR_EXECUTE.arg(query.lastError().text());
        return false;
    }
    return true;
}

}

SageUserRepository::SageUserRepository(const SageDbConfig& config)
    : m_config(config)
{
}

bool SageUserRepository::selectByLoginId(const QString& loginId, std::optional<SageUserDto>& outUser,
                                         QString& outError) const
{
    outUser.reset();

    SageDbConnection connection(m_config);
    if (!connection.open(outError)) {
        return false;
    }
    QSqlQuery query(connection.database());
    if (!prepare(query, SAGE_USER_SQL_SELECT_BY_LOGIN_ID, outError)) {
        return false;
    }
    query.addBindValue(loginId);
    if (!executePrepared(query, outError)) {
        return false;
    }
    if (!query.next()) {
        return true;
    }

    SageUserDto user;
    user.m_userId = query.value(SAGE_USER_COLUMN_USER_ID).toInt();
    user.m_loginId = query.value(SAGE_USER_COLUMN_LOGIN_ID).toString();
    user.m_passwordHash = query.value(SAGE_USER_COLUMN_PW_HASH).toString();
    user.m_role = static_cast<SageUserRole>(query.value(SAGE_USER_COLUMN_ROLE).toInt());
    user.m_isPasswordChangeRequired =
        query.value(SAGE_USER_COLUMN_MUST_CHANGE_PW).toInt() == SAGE_USER_MUST_CHANGE_PW_REQUIRED;
    outUser = user;
    return true;
}

bool SageUserRepository::existsByLoginId(const QString& loginId, bool& outExists, QString& outError) const
{
    outExists = false;

    SageDbConnection connection(m_config);
    if (!connection.open(outError)) {
        return false;
    }
    QSqlQuery query(connection.database());
    if (!prepare(query, SAGE_USER_SQL_COUNT_BY_LOGIN_ID, outError)) {
        return false;
    }
    query.addBindValue(loginId);
    if (!executePrepared(query, outError)) {
        return false;
    }

    outExists = query.next() && query.value(SAGE_USER_COLUMN_COUNT).toInt() > 0;
    return true;
}

bool SageUserRepository::insert(const SageUserDto& user, int& outUserId, QString& outError) const
{
    outUserId = 0;

    SageDbConnection connection(m_config);
    if (!connection.open(outError)) {
        return false;
    }
    QSqlQuery query(connection.database());
    if (!prepare(query, SAGE_USER_SQL_INSERT, outError)) {
        return false;
    }
    query.addBindValue(user.m_loginId);
    query.addBindValue(user.m_passwordHash);
    query.addBindValue(static_cast<int>(user.m_role));
    query.addBindValue(user.m_isPasswordChangeRequired ? SAGE_USER_MUST_CHANGE_PW_REQUIRED
                                                       : SAGE_USER_MUST_CHANGE_PW_NONE);
    if (!executePrepared(query, outError)) {
        return false;
    }

    outUserId = query.lastInsertId().toInt();
    return true;
}

bool SageUserRepository::updatePassword(int userId, const QString& passwordHash, QString& outError) const
{
    SageDbConnection connection(m_config);
    if (!connection.open(outError)) {
        return false;
    }
    QSqlQuery query(connection.database());
    if (!prepare(query, SAGE_USER_SQL_UPDATE_PASSWORD, outError)) {
        return false;
    }
    query.addBindValue(passwordHash);
    query.addBindValue(userId);
    return executePrepared(query, outError);
}
