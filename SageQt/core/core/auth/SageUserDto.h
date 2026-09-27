#pragma once

#include "SageDefine.h"

#include <QString>

struct SageUserDto
{
    int m_userId = 0;
    QString m_loginId;
    QString m_passwordHash;
    SageUserRole m_role = SageUserRole::User;
    bool m_isPasswordChangeRequired = false;
};
