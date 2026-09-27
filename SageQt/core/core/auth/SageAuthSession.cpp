#include "core/auth/SageAuthSession.h"

bool SageAuthSession::isLoggedIn() const
{
    return m_isLoggedIn;
}

bool SageAuthSession::isAdmin() const
{
    return m_isLoggedIn && m_currentUser.m_role == SageUserRole::Admin;
}

const SageUserDto& SageAuthSession::currentUser() const
{
    return m_currentUser;
}

void SageAuthSession::setLogin(const SageUserDto& user)
{
    m_currentUser = user;
    m_isLoggedIn = true;
}

void SageAuthSession::logout()
{
    m_currentUser = SageUserDto();
    m_isLoggedIn = false;
}
