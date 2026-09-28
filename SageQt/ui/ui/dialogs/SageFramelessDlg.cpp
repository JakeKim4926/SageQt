#include "ui/dialogs/SageFramelessDlg.h"

#include "ui/style/SageDesignDefine.h"
#include "ui/widgets/SageDialogCaptionBar.h"

#include <QFrame>
#include <QPalette>
#include <QVBoxLayout>

SageFramelessDlg::SageFramelessDlg(const QString& title, QWidget* parent)
    : QDialog(parent, Qt::Dialog | Qt::FramelessWindowHint)
{
    setWindowTitle(title);
    createWidgets(title);
    createLayout();
    connectSignals();
}

QSize SageFramelessDlg::sizeHint() const
{
    return QDialog::sizeHint().expandedTo(minimumSize());
}

QWidget* SageFramelessDlg::contentWidget() const
{
    return m_contentWidget;
}

void SageFramelessDlg::createWidgets(const QString& title)
{
    m_frame = new QFrame(this);
    m_frame->setFrameShape(QFrame::Box);
    m_frame->setLineWidth(SAGE_BORDER_THICKNESS);
    m_frame->setBackgroundRole(QPalette::Base);
    m_frame->setAutoFillBackground(true);
    m_captionBar = new SageDialogCaptionBar(title, m_frame);
    m_contentWidget = new QWidget(m_frame);
}

void SageFramelessDlg::createLayout()
{
    QVBoxLayout* dialogLayout = new QVBoxLayout(this);
    dialogLayout->setContentsMargins(0, 0, 0, 0);
    dialogLayout->addWidget(m_frame);

    QVBoxLayout* frameLayout = new QVBoxLayout(m_frame);
    frameLayout->setContentsMargins(0, 0, 0, 0);
    frameLayout->setSpacing(0);
    frameLayout->addWidget(m_captionBar);
    frameLayout->addWidget(m_contentWidget);
}

void SageFramelessDlg::connectSignals()
{
    connect(m_captionBar, &SageDialogCaptionBar::closeRequested, this, &SageFramelessDlg::reject);
}
