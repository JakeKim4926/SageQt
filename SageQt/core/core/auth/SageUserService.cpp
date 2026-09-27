#include "core/auth/SageUserService.h"

#include "SageDefine.h"
#include "core/auth/ISagePasswordHasher.h"
#include "core/auth/ISageUserRepository.h"

#include <QChar>
#include <QRandomGenerator>

SageUserService::SageUserService(const ISageUserRepository& repository, const ISagePasswordHasher& hasher)
    : m_repository(repository)
    , m_hasher(hasher)
{
}

bool SageUserService::login(const QString& loginId, const QString& password, std::optional<SageUserDto>& outUser,
                            QString& outError) const
{
    outUser.reset();

    std::optional<SageUserDto> user;
    if (!m_repository.selectByLoginId(loginId, user, outError)) {
        return false;
    }
    if (!user.has_value()) {
        return true;
    }
    if (!m_hasher.verifyPassword(password, user->m_passwordHash)) {
        return true;
    }

    outUser = user;
    return true;
}

bool SageUserService::changePassword(int userId, const QString& newPassword, QString& outError) const
{
    if (!validatePassword(newPassword, outError)) {
        return false;
    }
    return m_repository.updatePassword(userId, m_hasher.hashPassword(newPassword), outError);
}

bool SageUserService::ensureDefaultAdmin(std::optional<QString>& outInitialPassword, QString& outError) const
{
    outInitialPassword.reset();

    bool exists = false;
    if (!m_repository.existsByLoginId(SAGE_DEFAULT_ADMIN_ID, exists, outError)) {
        return false;
    }
    if (exists) {
        return true;
    }

    const QString initialPassword = generateInitialPassword();
    SageUserDto admin;
    admin.m_loginId = SAGE_DEFAULT_ADMIN_ID;
    admin.m_passwordHash = m_hasher.hashPassword(initialPassword);
    admin.m_role = SageUserRole::Admin;
    admin.m_isPasswordChangeRequired = true;

    int userId = 0;
    if (!m_repository.insert(admin, userId, outError)) {
        return false;
    }

    outInitialPassword = initialPassword;
    return true;
}

bool SageUserService::validatePassword(const QString& password, QString& outError)
{
    if (password.isEmpty()) {
        outError = SAGE_UI_PW_EMPTY;
        return false;
    }
    if (password.size() < SAGE_USER_PW_MIN_LEN) {
        outError = SAGE_UI_CHANGE_PW_TOO_SHORT;
        return false;
    }
    if (password.size() > SAGE_USER_PW_MAX_LEN) {
        outError = SAGE_UI_CHANGE_PW_TOO_LONG;
        return false;
    }
    for (const QChar character : password) {
        if (!SAGE_USER_PW_ALLOWED_CHARACTERS.contains(character)) {
            outError = SAGE_UI_CHANGE_PW_INVALID_CHAR;
            return false;
        }
    }
    return true;
}

QString SageUserService::generateInitialPassword()
{
    QString password;
    password.reserve(SAGE_INITIAL_PW_LENGTH);
    for (int index = 0; index < SAGE_INITIAL_PW_LENGTH; ++index) {
        const qsizetype position = QRandomGenerator::system()->bounded(SAGE_INITIAL_PW_ALPHABET.size());
        password.append(SAGE_INITIAL_PW_ALPHABET.at(position));
    }
    return password;
}
