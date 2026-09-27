#pragma once

#include "core/auth/SageUserDto.h"

#include <QString>

#include <optional>

class ISageUserRepository
{
public:
    virtual ~ISageUserRepository() = default;

    virtual bool selectByLoginId(const QString& loginId, std::optional<SageUserDto>& outUser,
                                 QString& outError) const = 0;
    virtual bool existsByLoginId(const QString& loginId, bool& outExists, QString& outError) const = 0;
    virtual bool insert(const SageUserDto& user, int& outUserId, QString& outError) const = 0;
    virtual bool updatePassword(int userId, const QString& passwordHash, QString& outError) const = 0;
};
