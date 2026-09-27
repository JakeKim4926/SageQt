#pragma once

#include "core/auth/ISageUserRepository.h"
#include "infra/db/SageDbConfig.h"

class SageUserRepository final : public ISageUserRepository
{
public:
    explicit SageUserRepository(const SageDbConfig& config);

    bool selectByLoginId(const QString& loginId, std::optional<SageUserDto>& outUser, QString& outError) const override;
    bool existsByLoginId(const QString& loginId, bool& outExists, QString& outError) const override;
    bool insert(const SageUserDto& user, int& outUserId, QString& outError) const override;
    bool updatePassword(int userId, const QString& passwordHash, QString& outError) const override;

private:
    SageDbConfig m_config;
};
