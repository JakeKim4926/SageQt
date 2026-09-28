#include "ui/dialogs/SageMessageBoxDlg.h"

#include "SageDefine.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/widgets/SageButton.h"

#include <QGridLayout>
#include <QLabel>
#include <QSize>
#include <QSizePolicy>

SageMessageBoxDlg::SageMessageBoxDlg(SageMessageIcon icon, const QString& message, QWidget* parent)
    : SageFramelessDlg(captionTitle(icon), parent)
{
    setMinimumWidth(SAGE_MSGBOX_WIDTH);
    createWidgets(icon, message);
    createLayout();
    connectSignals();
}

void SageMessageBoxDlg::createWidgets(SageMessageIcon icon, const QString& message)
{
    const QSize iconSize(SAGE_MSGBOX_ICON_SIZE, SAGE_MSGBOX_ICON_SIZE);
    m_iconLabel = new QLabel(contentWidget());
    m_iconLabel->setPixmap(style()->standardIcon(standardPixmap(icon)).pixmap(iconSize, devicePixelRatioF()));
    m_iconLabel->setFixedSize(iconSize);

    m_messageLabel = new QLabel(message, contentWidget());
    m_messageLabel->setTextFormat(Qt::PlainText);
    m_messageLabel->setWordWrap(true);
    QSizePolicy messagePolicy = m_messageLabel->sizePolicy();
    messagePolicy.setHorizontalPolicy(QSizePolicy::Ignored);
    m_messageLabel->setSizePolicy(messagePolicy);
    m_messageLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_messageLabel->setMaximumHeight(SAGE_MSGBOX_MAX_TEXT_HEIGHT);

    m_okButton = new SageButton(SAGE_UI_MSGBOX_OK, contentWidget());
    m_okButton->setVariant(SageButton::SageButtonVariant::Primary);
    m_okButton->setMinimumWidth(SAGE_LOGIN_DLG_BTN_WIDTH);
    m_okButton->setDefault(true);
}

void SageMessageBoxDlg::createLayout()
{
    QGridLayout* layout = new QGridLayout(contentWidget());
    layout->setContentsMargins(SAGE_MARGIN, SAGE_MARGIN, SAGE_MARGIN, SAGE_MARGIN);
    layout->setHorizontalSpacing(SAGE_MSGBOX_ICON_TEXT_GAP);
    layout->setVerticalSpacing(SAGE_MARGIN);
    layout->addWidget(m_iconLabel, 0, 0, Qt::AlignTop);
    layout->addWidget(m_messageLabel, 0, 1);
    layout->addWidget(m_okButton, 1, 0, 1, 2, Qt::AlignRight);
    layout->setColumnStretch(1, 1);
}

void SageMessageBoxDlg::connectSignals()
{
    connect(m_okButton, &SageButton::clicked, this, &SageMessageBoxDlg::accept);
}

QString SageMessageBoxDlg::captionTitle(SageMessageIcon icon)
{
    switch (icon) {
    case SageMessageIcon::Info:
        return SAGE_UI_MSGBOX_TITLE_INFO;
    case SageMessageIcon::Warning:
        return SAGE_UI_MSGBOX_TITLE_WARNING;
    case SageMessageIcon::Error:
        return SAGE_UI_MSGBOX_TITLE_ERROR;
    }
    return SAGE_UI_MSGBOX_TITLE_INFO;
}

QStyle::StandardPixmap SageMessageBoxDlg::standardPixmap(SageMessageIcon icon)
{
    switch (icon) {
    case SageMessageIcon::Info:
        return QStyle::SP_MessageBoxInformation;
    case SageMessageIcon::Warning:
        return QStyle::SP_MessageBoxWarning;
    case SageMessageIcon::Error:
        return QStyle::SP_MessageBoxCritical;
    }
    return QStyle::SP_MessageBoxInformation;
}
