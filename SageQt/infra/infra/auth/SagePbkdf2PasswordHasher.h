#pragma once

#include "core/auth/ISagePasswordHasher.h"
#include "infra/auth/SagePasswordHashDefine.h"

#include <QByteArray>
#include <QString>

class SagePbkdf2PasswordHasher final : public ISagePasswordHasher
{
public:
    explicit SagePbkdf2PasswordHasher(int iterations = SAGE_PASSWORD_HASH_ITERATIONS);

    QString hashPassword(const QString& password) const override;
    bool verifyPassword(const QString& password, const QString& storedHash) const override;

private:
    static QByteArray deriveKey(const QString& password, const QByteArray& salt, int iterations, qsizetype keyBytes);
    static QByteArray generateSalt();

private:
    int m_iterations = SAGE_PASSWORD_HASH_ITERATIONS;
};
