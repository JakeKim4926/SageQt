#pragma once

#include "infra/db/SageDbConfig.h"

#include <QString>

class SageSchemaInitializer
{
public:
    explicit SageSchemaInitializer(const SageDbConfig& config);

    bool prepare(QString& outError) const;

private:
    SageDbConfig m_config;
};
