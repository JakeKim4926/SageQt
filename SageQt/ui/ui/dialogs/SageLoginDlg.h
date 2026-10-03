#pragma once

#include "core/auth/SageUserDto.h"
#include "ui/dialogs/SageFramelessDlg.h"

#include <QFutureWatcher>
#include <QString>

#include <optional>

class QWidget;
class SageAuthSession;
class SageButton;
class SageInlineMessage;
class SageLabel;
class SageLineEdit;
class SageUserService;

struct SageLoginAttempt
{
    bool m_isQueried = false;
    std::optional<SageUserDto> m_user;
    QString m_error;
};

class SageLoginDlg : public SageFramelessDlg
{
    Q_OBJECT

public:
    SageLoginDlg(const SageUserService& userService, SageAuthSession& authSession, QWidget* parent = nullptr);

private slots:
    void onLoginButtonClicked();
    void onLoginWatcherFinished();

private:
    void createWidgets();
    void createLayout();
    void connectSignals();
    void showInputError(SageLineEdit* edit, const QString& message);
    bool completePasswordChange();

private:
    const SageUserService& m_userService;
    SageAuthSession& m_authSession;
    SageLabel* m_idLabel = nullptr;
    SageLabel* m_passwordLabel = nullptr;
    SageLineEdit* m_idEdit = nullptr;
    SageLineEdit* m_passwordEdit = nullptr;
    SageInlineMessage* m_errorMessage = nullptr;
    QWidget* m_buttonRow = nullptr;
    SageButton* m_loginButton = nullptr;
    SageButton* m_cancelButton = nullptr;
    QFutureWatcher<SageLoginAttempt>* m_loginWatcher = nullptr;
};
