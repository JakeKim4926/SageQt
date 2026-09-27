#pragma once

#include "core/auth/SageUserDto.h"

#include <QString>

#include <optional>

class ISagePasswordHasher;
class ISageUserRepository;

class SageUserService
{
public:
    SageUserService(const ISageUserRepository& repository, const ISagePasswordHasher& hasher);

    bool login(const QString& loginId, const QString& password, std::optional<SageUserDto>& outUser,
               QString& outError) const;
    bool changePassword(int userId, const QString& newPassword, QString& outError) const;
    bool ensureDefaultAdmin(std::optional<QString>& outInitialPassword, QString& outError) const;

private:
    static bool validatePassword(const QString& password, QString& outError);
    static QString generateInitialPassword();

private:
    const ISageUserRepository& m_repository;
    const ISagePasswordHasher& m_hasher;
};
