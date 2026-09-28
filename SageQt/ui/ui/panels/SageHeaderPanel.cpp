#include "ui/panels/SageHeaderPanel.h"

#include "core/auth/SageAuthSession.h"
#include "core/workflow/ISageWorkflowHandler.h"
#include "core/workflow/SageWorkflowRegistry.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/widgets/SageBadge.h"
#include "ui/widgets/SageButton.h"
#include "ui/widgets/SageLabel.h"
#include "ui/widgets/SageSurface.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QString>
#include <QVBoxLayout>

SageHeaderPanel::SageHeaderPanel(const SageWorkflowRegistry& registry, SageAuthSession& authSession, QWidget* parent)
    : QWidget(parent)
    , m_registry(registry)
    , m_authSession(authSession)
{
    createWidgets();
    createLayout();
    connectSignals();
    updateAuthState();
}

void SageHeaderPanel::showWorkflow(SageWorkflowType workflowType)
{
    const ISageWorkflowHandler* handler = m_registry.findHandler(workflowType);
    if (handler == nullptr) {
        m_titleLabel->clear();
        m_categoryLabel->clear();
        return;
    }
    m_titleLabel->setText(handler->headerTitle());
    m_categoryLabel->setText(handler->category());
}

void SageHeaderPanel::onLogoutButtonClicked()
{
    m_authSession.logout();
}

void SageHeaderPanel::updateAuthState()
{
    const bool isLoggedIn = m_authSession.isLoggedIn();
    m_loginButton->setVisible(!isLoggedIn);
    m_logoutButton->setVisible(isLoggedIn);
    m_userLabel->setVisible(isLoggedIn);
    m_roleBadge->setVisible(isLoggedIn);
    if (!isLoggedIn) {
        m_userLabel->clear();
        m_roleBadge->setText(QString());
        return;
    }
    m_userLabel->setText(m_authSession.currentUser().m_loginId);
    m_roleBadge->setText(m_authSession.isAdmin() ? SAGE_UI_ROLE_ADMIN : SAGE_UI_ROLE_USER);
}

void SageHeaderPanel::createWidgets()
{
    m_surface = new SageSurface(SageSurface::SageSurfaceVariant::Panel, this);
    m_row = new QWidget(m_surface);
    m_row->setFixedHeight(SAGE_HEADER_HEIGHT);

    m_titleLabel = new SageLabel(SageLabel::SageLabelVariant::Title, QString(), m_row);
    m_categoryLabel = new SageLabel(SageLabel::SageLabelVariant::SecondaryCaption, QString(), m_row);
    m_categoryLabel->setMinimumWidth(SAGE_HEADER_CATEGORY_WIDTH);

    m_userLabel = new SageLabel(SageLabel::SageLabelVariant::MutedCaption, QString(), m_row);
    m_userLabel->setMinimumWidth(SAGE_USER_LABEL_WIDTH);
    m_userLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_userLabel->setTextFormat(Qt::PlainText);

    m_roleBadge = new SageBadge(SageBadge::SageBadgeVariant::Neutral, m_row);

    m_loginButton = new SageButton(SAGE_UI_LOGIN_BTN, m_row);
    m_loginButton->setMinimumWidth(SAGE_LOGIN_BTN_WIDTH);
    m_logoutButton = new SageButton(SAGE_UI_LOGOUT_BTN, m_row);
    m_logoutButton->setMinimumWidth(SAGE_LOGIN_BTN_WIDTH);

    m_bottomLine = new QFrame(m_surface);
    m_bottomLine->setFrameShape(QFrame::HLine);
    m_bottomLine->setFixedHeight(SAGE_BORDER_THICKNESS);
}

void SageHeaderPanel::createLayout()
{
    QVBoxLayout* panelLayout = new QVBoxLayout(this);
    panelLayout->setContentsMargins(0, 0, 0, 0);
    panelLayout->addWidget(m_surface);

    QVBoxLayout* surfaceLayout = new QVBoxLayout(m_surface);
    surfaceLayout->setContentsMargins(0, 0, 0, 0);
    surfaceLayout->setSpacing(0);
    surfaceLayout->addWidget(m_row);
    surfaceLayout->addWidget(m_bottomLine);

    QHBoxLayout* rowLayout = new QHBoxLayout(m_row);
    rowLayout->setContentsMargins(SAGE_CONTENT_PAD_X, 0, SAGE_CONTENT_PAD_X, 0);
    rowLayout->setSpacing(0);
    rowLayout->addWidget(m_titleLabel);
    rowLayout->addSpacing(SAGE_HEADER_TITLE_GAP);
    rowLayout->addWidget(m_categoryLabel);
    rowLayout->addStretch();
    rowLayout->addWidget(m_userLabel);
    rowLayout->addSpacing(SAGE_HEADER_GAP);
    rowLayout->addWidget(m_roleBadge);
    rowLayout->addSpacing(SAGE_HEADER_GAP);
    rowLayout->addWidget(m_loginButton);
    rowLayout->addWidget(m_logoutButton);
}

void SageHeaderPanel::connectSignals()
{
    connect(m_loginButton, &SageButton::clicked, this, &SageHeaderPanel::loginRequested);
    connect(m_logoutButton, &SageButton::clicked, this, &SageHeaderPanel::onLogoutButtonClicked);
    connect(&m_authSession, &SageAuthSession::authStateChanged, this, &SageHeaderPanel::updateAuthState);
}
