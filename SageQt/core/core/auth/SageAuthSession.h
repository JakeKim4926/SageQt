#pragma once

#include "core/auth/SageUserDto.h"

#include <QObject>

class SageAuthSession : public QObject
{
    Q_OBJECT

public:
    explicit SageAuthSession(QObject* parent = nullptr);

    bool isLoggedIn() const;
    bool isAdmin() const;
    const SageUserDto& currentUser() const;

    void setLogin(const SageUserDto& user);
    void logout();

signals:
    void authStateChanged();

private:
    bool m_isLoggedIn = false;
    SageUserDto m_currentUser;
};
