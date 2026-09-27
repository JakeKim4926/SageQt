#pragma once

#include <QChar>
#include <QString>

inline constexpr int SAGE_PASSWORD_HASH_ITERATIONS = 600000;
inline constexpr int SAGE_PASSWORD_HASH_SALT_BYTES = 16;
inline constexpr int SAGE_PASSWORD_HASH_KEY_BYTES = 32;
inline constexpr int SAGE_PASSWORD_HASH_BYTE_VALUES = 256;
inline constexpr int SAGE_PASSWORD_HASH_PART_COUNT = 4;
inline constexpr int SAGE_PASSWORD_HASH_PART_SCHEME = 0;
inline constexpr int SAGE_PASSWORD_HASH_PART_ITERATIONS = 1;
inline constexpr int SAGE_PASSWORD_HASH_PART_SALT = 2;
inline constexpr int SAGE_PASSWORD_HASH_PART_KEY = 3;

inline const QString SAGE_PASSWORD_HASH_SCHEME = QStringLiteral("pbkdf2-sha256");
inline const QChar SAGE_PASSWORD_HASH_SEPARATOR = QLatin1Char('$');
