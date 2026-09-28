#include "ui/widgets/SageDialogCaptionBar.h"

#include "SageDefine.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageFontCatalog.h"

#include <QFontMetrics>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLoggingCategory>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QStyle>
#include <QToolButton>
#include <QWindow>

Q_STATIC_LOGGING_CATEGORY(sageUiLog, SAGE_LOG_CATEGORY_UI)

SageDialogCaptionBar::SageDialogCaptionBar(const QString& title, QWidget* parent)
    : QWidget(parent)
    , m_title(title)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    createWidgets();
    createLayout();
    connectSignals();
}

QSize SageDialogCaptionBar::sizeHint() const
{
    return {QWidget::sizeHint().width(), SAGE_DLG_CAPTION_HEIGHT};
}

QSize SageDialogCaptionBar::minimumSizeHint() const
{
    return {QWidget::minimumSizeHint().width(), SAGE_DLG_CAPTION_HEIGHT};
}

void SageDialogCaptionBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(), SAGE_COLOR_LIST_HEADER);
    painter.fillRect(QRect(0, height() - SAGE_BORDER_THICKNESS, width(), SAGE_BORDER_THICKNESS), SAGE_COLOR_BORDER);

    const QRect titleRect =
        rect().adjusted(SAGE_DLG_CAPTION_PAD, 0, -(SAGE_DLG_CAPTION_BTN_PAD + SAGE_DLG_CAPTION_BTN_SIZE), 0);
    const QFont titleFont = SageFontCatalog::font(SageFontRole::BodyStrong);
    painter.setFont(titleFont);
    painter.setPen(SAGE_COLOR_TEXT);
    painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter,
                     QFontMetrics(titleFont).elidedText(m_title, Qt::ElideRight, titleRect.width()));
}

void SageDialogCaptionBar::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }
    QWindow* windowHandle = window()->windowHandle();
    if (windowHandle == nullptr || !windowHandle->startSystemMove()) {
        qCWarning(sageUiLog).noquote() << SAGE_LOG_WINDOW_MOVE_UNSUPPORTED.arg(QGuiApplication::platformName());
    }
}

void SageDialogCaptionBar::createWidgets()
{
    m_closeButton = new QToolButton(this);
    m_closeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    m_closeButton->setIconSize(QSize(SAGE_ICON_SIZE, SAGE_ICON_SIZE));
    m_closeButton->setFixedSize(SAGE_DLG_CAPTION_BTN_SIZE, SAGE_DLG_CAPTION_BTN_SIZE);
    m_closeButton->setToolTip(SAGE_UI_TIP_CLOSE);
    m_closeButton->setFocusPolicy(Qt::NoFocus);
}

void SageDialogCaptionBar::createLayout()
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(SAGE_DLG_CAPTION_PAD, 0, SAGE_DLG_CAPTION_BTN_PAD, 0);
    layout->addStretch();
    layout->addWidget(m_closeButton);
}

void SageDialogCaptionBar::connectSignals()
{
    connect(m_closeButton, &QToolButton::clicked, this, &SageDialogCaptionBar::closeRequested);
}
