#pragma once

#include "core/auth/SageUserDto.h"

class SageAuthSession
{
public:
    bool isLoggedIn() const;
    bool isAdmin() const;
    const SageUserDto& currentUser() const;

    void setLogin(const SageUserDto& user);
    void logout();

private:
    bool m_isLoggedIn = false;
    SageUserDto m_currentUser;
};
