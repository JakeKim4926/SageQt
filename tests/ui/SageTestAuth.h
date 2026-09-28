#pragma once

#include "core/auth/ISagePasswordHasher.h"
#include "core/auth/ISageUserRepository.h"
#include "core/auth/SageUserDto.h"

#include <QHash>
#include <QString>

#include <optional>

class SageTestPasswordHasher final : public ISagePasswordHasher
{
public:
    QString hashPassword(const QString& password) const override
    {
        return QStringLiteral("hashed:") + password;
    }

    bool verifyPassword(const QString& password, const QString& storedHash) const override
    {
        return storedHash == hashPassword(password);
    }
};

class SageTestUserRepository final : public ISageUserRepository
{
public:
    bool selectByLoginId(const QString& loginId, std::optional<SageUserDto>& outUser, QString& outError) const override
    {
        if (m_isFailing) {
            outError = QStringLiteral("db down");
            return false;
        }
        outUser.reset();
        if (m_users.contains(loginId)) {
            outUser = m_users.value(loginId);
        }
        return true;
    }

    bool existsByLoginId(const QString& loginId, bool& outExists, QString&) const override
    {
        outExists = m_users.contains(loginId);
        return true;
    }

    bool insert(const SageUserDto& user, int& outUserId, QString&) const override
    {
        m_users.insert(user.m_loginId, user);
        outUserId = user.m_userId;
        return true;
    }

    bool updatePassword(int userId, const QString& passwordHash, QString&) const override
    {
        for (SageUserDto& user : m_users) {
            if (user.m_userId == userId) {
                user.m_passwordHash = passwordHash;
                user.m_isPasswordChangeRequired = false;
            }
        }
        return true;
    }

    void addUser(int userId, const QString& loginId, const QString& password, SageUserRole role,
                 bool isPasswordChangeRequired)
    {
        SageUserDto user;
        user.m_userId = userId;
        user.m_loginId = loginId;
        user.m_passwordHash = SageTestPasswordHasher().hashPassword(password);
        user.m_role = role;
        user.m_isPasswordChangeRequired = isPasswordChangeRequired;
        m_users.insert(loginId, user);
    }

    SageUserDto user(const QString& loginId) const
    {
        return m_users.value(loginId);
    }

    void setFailing(bool isFailing)
    {
        m_isFailing = isFailing;
    }

private:
    mutable QHash<QString, SageUserDto> m_users;
    bool m_isFailing = false;
};
