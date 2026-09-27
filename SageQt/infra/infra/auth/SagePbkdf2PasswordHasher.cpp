#include "infra/auth/SagePbkdf2PasswordHasher.h"

#include <QCryptographicHash>
#include <QPasswordDigestor>
#include <QRandomGenerator>
#include <QStringList>

SagePbkdf2PasswordHasher::SagePbkdf2PasswordHasher(int iterations)
    : m_iterations(iterations)
{
}

QString SagePbkdf2PasswordHasher::hashPassword(const QString& password) const
{
    const QByteArray salt = generateSalt();
    const QByteArray key = deriveKey(password, salt, m_iterations, SAGE_PASSWORD_HASH_KEY_BYTES);
    const QStringList parts = {SAGE_PASSWORD_HASH_SCHEME, QString::number(m_iterations),
                               QString::fromLatin1(salt.toBase64()), QString::fromLatin1(key.toBase64())};
    return parts.join(SAGE_PASSWORD_HASH_SEPARATOR);
}

bool SagePbkdf2PasswordHasher::verifyPassword(const QString& password, const QString& storedHash) const
{
    const QStringList parts = storedHash.split(SAGE_PASSWORD_HASH_SEPARATOR);
    if (parts.size() != SAGE_PASSWORD_HASH_PART_COUNT ||
        parts.at(SAGE_PASSWORD_HASH_PART_SCHEME) != SAGE_PASSWORD_HASH_SCHEME) {
        return false;
    }

    bool isNumber = false;
    const int iterations = parts.at(SAGE_PASSWORD_HASH_PART_ITERATIONS).toInt(&isNumber);
    if (!isNumber || iterations <= 0) {
        return false;
    }

    const QByteArray salt = QByteArray::fromBase64(parts.at(SAGE_PASSWORD_HASH_PART_SALT).toLatin1());
    const QByteArray expectedKey = QByteArray::fromBase64(parts.at(SAGE_PASSWORD_HASH_PART_KEY).toLatin1());
    if (salt.isEmpty() || expectedKey.isEmpty()) {
        return false;
    }

    return deriveKey(password, salt, iterations, expectedKey.size()) == expectedKey;
}

QByteArray SagePbkdf2PasswordHasher::deriveKey(const QString& password, const QByteArray& salt, int iterations,
                                               qsizetype keyBytes)
{
    return QPasswordDigestor::deriveKeyPbkdf2(QCryptographicHash::Sha256, password.toUtf8(), salt, iterations,
                                              static_cast<quint64>(keyBytes));
}

QByteArray SagePbkdf2PasswordHasher::generateSalt()
{
    QByteArray salt;
    salt.reserve(SAGE_PASSWORD_HASH_SALT_BYTES);
    for (int index = 0; index < SAGE_PASSWORD_HASH_SALT_BYTES; ++index) {
        salt.append(static_cast<char>(QRandomGenerator::system()->bounded(SAGE_PASSWORD_HASH_BYTE_VALUES)));
    }
    return salt;
}
