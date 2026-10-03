#include "ui/dialogs/SageLoginDlg.h"

#include "SageDefine.h"
#include "core/auth/SageAuthSession.h"
#include "core/auth/SageUserService.h"
#include "ui/dialogs/SageMessageBoxDlg.h"
#include "ui/dialogs/SagePasswordChangeDlg.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/widgets/SageButton.h"
#include "ui/widgets/SageInlineMessage.h"
#include "ui/widgets/SageLabel.h"
#include "ui/widgets/SageLineEdit.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QWidget>
#include <QtConcurrentRun>

SageLoginDlg::SageLoginDlg(const SageUserService& userService, SageAuthSession& authSession, QWidget* parent)
    : SageFramelessDlg(SAGE_UI_LOGIN_DLG_TITLE, parent)
    , m_userService(userService)
    , m_authSession(authSession)
{
    setMinimumWidth(SAGE_LOGIN_DLG_WIDTH);
    createWidgets();
    createLayout();
    connectSignals();
}

void SageLoginDlg::onLoginButtonClicked()
{
    m_errorMessage->clearMessage();
    m_idEdit->setVariant(SageLineEdit::SageLineEditVariant::Normal);
    m_passwordEdit->setVariant(SageLineEdit::SageLineEditVariant::Normal);

    const QString loginId = m_idEdit->text().trimmed();
    const QString password = m_passwordEdit->text();
    if (loginId.isEmpty()) {
        showInputError(m_idEdit, SAGE_UI_LOGIN_EMPTY_ID);
        return;
    }
    if (password.isEmpty()) {
        showInputError(m_passwordEdit, SAGE_UI_LOGIN_EMPTY_PW);
        return;
    }

    m_loginButton->setEnabled(false);
    const SageUserService& userService = m_userService;
    m_loginWatcher->setFuture(QtConcurrent::run([&userService, loginId, password]() {
        SageLoginAttempt attempt;
        attempt.m_isQueried = userService.login(loginId, password, attempt.m_user, attempt.m_error);
        return attempt;
    }));
}

void SageLoginDlg::onLoginWatcherFinished()
{
    m_loginButton->setEnabled(true);
    const SageLoginAttempt attempt = m_loginWatcher->result();
    if (!attempt.m_isQueried) {
        SageMessageBoxDlg errorDialog(SageMessageIcon::Error, attempt.m_error, this);
        errorDialog.exec();
        return;
    }
    if (!attempt.m_user.has_value()) {
        showInputError(m_passwordEdit, SAGE_UI_LOGIN_FAILED);
        m_passwordEdit->selectAll();
        return;
    }

    m_authSession.setLogin(*attempt.m_user);
    if (attempt.m_user->m_isPasswordChangeRequired && !completePasswordChange()) {
        return;
    }
    accept();
}

void SageLoginDlg::createWidgets()
{
    m_idLabel = new SageLabel(SageLabel::SageLabelVariant::FormLabel, SAGE_UI_LOGIN_ID_LABEL, contentWidget());
    m_idLabel->setMinimumWidth(SAGE_LOGIN_DLG_LABEL_WIDTH);
    m_passwordLabel = new SageLabel(SageLabel::SageLabelVariant::FormLabel, SAGE_UI_LOGIN_PW_LABEL, contentWidget());
    m_passwordLabel->setMinimumWidth(SAGE_LOGIN_DLG_LABEL_WIDTH);

    m_idEdit = new SageLineEdit(contentWidget());
    m_passwordEdit = new SageLineEdit(contentWidget());
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_errorMessage = new SageInlineMessage(SageInlineMessage::SageInlineMessageVariant::Error, contentWidget());

    m_buttonRow = new QWidget(contentWidget());
    m_loginButton = new SageButton(SAGE_UI_LOGIN_OK, m_buttonRow);
    m_loginButton->setVariant(SageButton::SageButtonVariant::Primary);
    m_loginButton->setMinimumWidth(SAGE_LOGIN_DLG_BTN_WIDTH);
    m_loginButton->setDefault(true);
    m_cancelButton = new SageButton(SAGE_UI_LOGIN_CANCEL, m_buttonRow);
    m_cancelButton->setMinimumWidth(SAGE_LOGIN_DLG_BTN_WIDTH);

    m_loginWatcher = new QFutureWatcher<SageLoginAttempt>(this);
}

void SageLoginDlg::createLayout()
{
    QGridLayout* layout = new QGridLayout(contentWidget());
    layout->setContentsMargins(SAGE_MARGIN, SAGE_MARGIN, SAGE_MARGIN, SAGE_MARGIN);
    layout->setHorizontalSpacing(SAGE_ROW_GAP);
    layout->setVerticalSpacing(0);
    layout->addWidget(m_idLabel, 0, 0);
    layout->addWidget(m_idEdit, 0, 1);
    layout->setRowMinimumHeight(1, SAGE_ROW_GAP);
    layout->addWidget(m_passwordLabel, 2, 0);
    layout->addWidget(m_passwordEdit, 2, 1);
    layout->addWidget(m_errorMessage, 3, 1);
    layout->setRowMinimumHeight(4, SAGE_ROW_GAP);
    layout->addWidget(m_buttonRow, 5, 0, 1, 2);
    layout->setColumnStretch(1, 1);

    QHBoxLayout* buttonLayout = new QHBoxLayout(m_buttonRow);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(SAGE_ROW_GAP);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_loginButton);
    buttonLayout->addWidget(m_cancelButton);
}

void SageLoginDlg::connectSignals()
{
    connect(m_loginButton, &SageButton::clicked, this, &SageLoginDlg::onLoginButtonClicked);
    connect(m_cancelButton, &SageButton::clicked, this, &SageLoginDlg::reject);
    connect(m_loginWatcher, &QFutureWatcher<SageLoginAttempt>::finished, this, &SageLoginDlg::onLoginWatcherFinished);
}

void SageLoginDlg::showInputError(SageLineEdit* edit, const QString& message)
{
    m_errorMessage->setMessage(message);
    edit->setVariant(SageLineEdit::SageLineEditVariant::Error);
    edit->setFocus();
}

bool SageLoginDlg::completePasswordChange()
{
    SageMessageBoxDlg requiredDialog(SageMessageIcon::Warning, SAGE_UI_MUST_CHANGE_PW_REQUIRED, this);
    requiredDialog.exec();

    SagePasswordChangeDlg passwordDialog(m_userService, m_authSession, this);
    if (passwordDialog.exec() == QDialog::Accepted) {
        return true;
    }

    m_authSession.logout();
    SageMessageBoxDlg canceledDialog(SageMessageIcon::Warning, SAGE_UI_MUST_CHANGE_PW_CANCELED, this);
    canceledDialog.exec();
    m_passwordEdit->clear();
    m_passwordEdit->setFocus();
    return false;
}
