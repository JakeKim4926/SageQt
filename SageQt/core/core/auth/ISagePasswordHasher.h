#pragma once

#include <QString>

class ISagePasswordHasher
{
public:
    virtual ~ISagePasswordHasher() = default;

    virtual QString hashPassword(const QString& password) const = 0;
    virtual bool verifyPassword(const QString& password, const QString& storedHash) const = 0;
};
