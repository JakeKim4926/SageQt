#include "ui/widgets/SageStatusCard.h"

#include "SageDefine.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/style/SageFontCatalog.h"
#include "ui/widgets/SageButton.h"

#include <QBrush>
#include <QDir>
#include <QFileInfo>
#include <QFont>
#include <QHBoxLayout>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QPointF>
#include <QProgressBar>
#include <QRectF>
#include <QSize>
#include <QVBoxLayout>
#include <qdrawutil.h>

#include <iterator>

namespace {
constexpr int SAGE_STATUS_CARD_RUNNING_BLOCK_HEIGHT =
    SAGE_STATUS_CARD_TITLE_LINE_HEIGHT + SAGE_CARD_ROW_GAP + SAGE_STATUS_CARD_PROGRESS_HEIGHT;
constexpr int SAGE_STATUS_CARD_PROGRESS_TOP = (SAGE_STATUS_CARD_HEIGHT - SAGE_STATUS_CARD_RUNNING_BLOCK_HEIGHT) / 2 +
                                              SAGE_STATUS_CARD_TITLE_LINE_HEIGHT + SAGE_CARD_ROW_GAP;
constexpr int SAGE_STATUS_CARD_PROGRESS_BOTTOM =
    SAGE_STATUS_CARD_HEIGHT - SAGE_STATUS_CARD_PROGRESS_TOP - SAGE_STATUS_CARD_PROGRESS_HEIGHT;
}

SageStatusCard::SageStatusCard(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(SAGE_STATUS_CARD_HEIGHT);
    createWidgets();
    createLayout();
    connectSignals();
    applyState(SageStatusCardVariant::Idle, QString(), QString());
}

SageStatusCard::SageStatusCardVariant SageStatusCard::variant() const
{
    return m_variant;
}

QString SageStatusCard::message() const
{
    return m_message;
}

QString SageStatusCard::detail() const
{
    return m_detail;
}

int SageStatusCard::progressPercent() const
{
    return m_progressBar->value();
}

void SageStatusCard::setIdle(const QString& message)
{
    applyState(SageStatusCardVariant::Idle, message, QString());
}

void SageStatusCard::setRunning(const QString& message)
{
    applyState(SageStatusCardVariant::Running, message, QString());
}

void SageStatusCard::setProgressPercent(int percent)
{
    if (m_variant != SageStatusCardVariant::Running || m_progressBar->value() == percent) {
        return;
    }
    m_progressBar->setValue(percent);
    update();
}

void SageStatusCard::setResult(bool success, const QString& message, const QString& detail)
{
    applyState(success ? SageStatusCardVariant::Completed : SageStatusCardVariant::Failed, message, detail);
}

void SageStatusCard::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    const QBrush surface(surfaceColor());
    qDrawPlainRect(&painter, rect(), borderColor(), SAGE_BORDER_THICKNESS, &surface);
    painter.setRenderHint(QPainter::Antialiasing);
    if (m_variant == SageStatusCardVariant::Completed || m_variant == SageStatusCardVariant::Failed) {
        drawResultContent(painter);
        return;
    }
    drawPendingContent(painter);
}

void SageStatusCard::createWidgets()
{
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, SAGE_PROGRESS_COMPLETE);
    m_progressBar->setTextVisible(false);
    m_progressBar->setFixedHeight(SAGE_STATUS_CARD_PROGRESS_HEIGHT);
    m_openFolderButton = new SageButton(SAGE_UI_STATUS_CARD_OPEN_FOLDER, this);
    m_openFolderButton->setMinimumWidth(SAGE_STATUS_CARD_ACTION_WIDTH);
}

void SageStatusCard::createLayout()
{
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_progressBar);
    QHBoxLayout* actionLayout = new QHBoxLayout();
    m_layout->addLayout(actionLayout);
    actionLayout->addStretch();
    actionLayout->addWidget(m_openFolderButton, 0, Qt::AlignVCenter);
}

void SageStatusCard::connectSignals()
{
    connect(m_openFolderButton, &SageButton::clicked, this, &SageStatusCard::openFolderRequested);
}

void SageStatusCard::applyState(SageStatusCardVariant variant, const QString& message, const QString& detail)
{
    const bool variantChangedNow = m_variant != variant;
    m_variant = variant;
    m_message = message;
    m_detail = detail;
    if (variant == SageStatusCardVariant::Idle || variant == SageStatusCardVariant::Running) {
        m_progressBar->setValue(0);
    }
    const bool running = variant == SageStatusCardVariant::Running;
    m_progressBar->setVisible(running);
    m_openFolderButton->setVisible(variant == SageStatusCardVariant::Completed && !detail.isEmpty());
    if (running) {
        m_layout->setContentsMargins(SAGE_CARD_PADDING, SAGE_STATUS_CARD_PROGRESS_TOP, SAGE_CARD_PADDING,
                                     SAGE_STATUS_CARD_PROGRESS_BOTTOM);
    } else {
        m_layout->setContentsMargins(SAGE_CARD_PADDING, 0, SAGE_CARD_PADDING, 0);
    }
    update();
    if (variantChangedNow) {
        emit variantChanged();
    }
}

void SageStatusCard::drawPendingContent(QPainter& painter) const
{
    const QRect content = rect().adjusted(SAGE_CARD_PADDING, 0, -SAGE_CARD_PADDING, 0);
    const bool running = m_variant == SageStatusCardVariant::Running;
    const int blockHeight = running ? SAGE_STATUS_CARD_RUNNING_BLOCK_HEIGHT : SAGE_STATUS_CARD_TITLE_LINE_HEIGHT;
    const QRect row(content.left(), (height() - blockHeight) / 2, content.width(), SAGE_STATUS_CARD_TITLE_LINE_HEIGHT);

    const QRect dot(row.left(), row.top() + (row.height() - SAGE_STATUS_CARD_DOT_SIZE) / 2, SAGE_STATUS_CARD_DOT_SIZE,
                    SAGE_STATUS_CARD_DOT_SIZE);
    painter.setPen(Qt::NoPen);
    painter.setBrush(accentColor());
    painter.drawEllipse(QRectF(dot));

    QRect textRect = row.adjusted(dot.right() + 1 - row.left() + SAGE_STATUS_CARD_DOT_GAP, 0, 0, 0);
    if (running) {
        textRect.setRight(textRect.right() - SAGE_PROGRESS_TEXT_WIDTH);
    }
    const QFont titleFont = SageFontCatalog::font(SageFontRole::BodyStrong);
    painter.setFont(titleFont);
    painter.setPen(messageColor());
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter,
                     QFontMetrics(titleFont).elidedText(m_message, Qt::ElideRight, textRect.width()));
    if (!running) {
        return;
    }
    const QRect percentRect(row.right() + 1 - SAGE_PROGRESS_TEXT_WIDTH, row.top(), SAGE_PROGRESS_TEXT_WIDTH,
                            row.height());
    painter.setPen(SAGE_COLOR_PRIMARY);
    painter.drawText(percentRect, Qt::AlignRight | Qt::AlignVCenter,
                     SAGE_UI_PROGRESS_FORMAT.arg(m_progressBar->value()));
}

void SageStatusCard::drawResultContent(QPainter& painter) const
{
    QRect content = rect().adjusted(SAGE_CARD_PADDING, 0, -SAGE_CARD_PADDING, 0);
    if (m_openFolderButton->isVisible()) {
        content.setRight(content.right() - SAGE_STATUS_CARD_ACTION_AREA_WIDTH);
    }
    const QRect iconRect(content.left(), (height() - SAGE_STATUS_CARD_ICON_SIZE) / 2, SAGE_STATUS_CARD_ICON_SIZE,
                         SAGE_STATUS_CARD_ICON_SIZE);
    drawResultIcon(painter, iconRect);

    const int blockHeight =
        SAGE_STATUS_CARD_TITLE_LINE_HEIGHT + (m_detail.isEmpty() ? 0 : SAGE_STATUS_CARD_DETAIL_LINE_HEIGHT);
    QRect titleRect = content;
    titleRect.setLeft(iconRect.right() + 1 + SAGE_STATUS_CARD_ICON_GAP);
    titleRect.setTop((height() - blockHeight) / 2);
    titleRect.setHeight(SAGE_STATUS_CARD_TITLE_LINE_HEIGHT);

    const QFont titleFont = SageFontCatalog::font(SageFontRole::BodyStrong);
    painter.setFont(titleFont);
    painter.setPen(messageColor());
    painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter,
                     QFontMetrics(titleFont).elidedText(m_message, Qt::ElideRight, titleRect.width()));
    if (m_detail.isEmpty()) {
        return;
    }
    const QRect detailRect(titleRect.left(), titleRect.bottom() + 1, titleRect.width(),
                           SAGE_STATUS_CARD_DETAIL_LINE_HEIGHT);
    const QFont detailFont = SageFontCatalog::font(SageFontRole::Caption);
    const QFontMetrics detailMetrics(detailFont);
    const QString detailText = m_variant == SageStatusCardVariant::Completed
                                   ? elidedPath(m_detail, detailMetrics, detailRect.width())
                                   : detailMetrics.elidedText(m_detail, Qt::ElideRight, detailRect.width());
    painter.setFont(detailFont);
    painter.setPen(SAGE_COLOR_TEXT_MUTED);
    painter.drawText(detailRect, Qt::AlignLeft | Qt::AlignVCenter, detailText);
}

void SageStatusCard::drawResultIcon(QPainter& painter, const QRect& iconRect) const
{
    const QColor iconColor = accentColor();
    const QPointF center = QRectF(iconRect).center();
    painter.setPen(QPen(iconColor, SAGE_STATUS_CARD_ICON_THICKNESS));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(center, SAGE_STATUS_CARD_ICON_RADIUS, SAGE_STATUS_CARD_ICON_RADIUS);
    if (m_variant == SageStatusCardVariant::Completed) {
        const QPointF points[] = {
            center + QPointF(SAGE_STATUS_CARD_CHECK_START_X, SAGE_STATUS_CARD_CHECK_START_Y),
            center + QPointF(SAGE_STATUS_CARD_CHECK_MID_X, SAGE_STATUS_CARD_CHECK_MID_Y),
            center + QPointF(SAGE_STATUS_CARD_CHECK_END_X, SAGE_STATUS_CARD_CHECK_END_Y),
        };
        painter.drawPolyline(points, std::size(points));
        return;
    }
    painter.drawLine(center + QPointF(0, SAGE_STATUS_CARD_ALERT_STEM_TOP),
                     center + QPointF(0, SAGE_STATUS_CARD_ALERT_STEM_BOTTOM));
    painter.fillRect(QRectF(center + QPointF(0, SAGE_STATUS_CARD_ALERT_DOT_TOP),
                            QSize(SAGE_STATUS_CARD_ALERT_DOT_SIZE, SAGE_STATUS_CARD_ALERT_DOT_SIZE)),
                     iconColor);
}

QColor SageStatusCard::surfaceColor() const
{
    if (m_variant == SageStatusCardVariant::Completed) {
        return SAGE_COLOR_STATUS_CARD_BG_SUCCESS;
    }
    if (m_variant == SageStatusCardVariant::Failed) {
        return SAGE_COLOR_STATUS_CARD_BG_ERROR;
    }
    return SAGE_COLOR_PANEL;
}

QColor SageStatusCard::borderColor() const
{
    if (m_variant == SageStatusCardVariant::Completed) {
        return SAGE_COLOR_STATUS_CARD_BORDER_SUCCESS;
    }
    if (m_variant == SageStatusCardVariant::Failed) {
        return SAGE_COLOR_DANGER_BORDER;
    }
    return SAGE_COLOR_BORDER;
}

QColor SageStatusCard::accentColor() const
{
    if (m_variant == SageStatusCardVariant::Running) {
        return SAGE_COLOR_WARNING;
    }
    if (m_variant == SageStatusCardVariant::Completed) {
        return SAGE_COLOR_SUCCESS;
    }
    if (m_variant == SageStatusCardVariant::Failed) {
        return SAGE_COLOR_ERROR;
    }
    return SAGE_COLOR_TEXT_PLACEHOLDER;
}

QColor SageStatusCard::messageColor() const
{
    if (m_variant == SageStatusCardVariant::Idle) {
        return SAGE_COLOR_SECONDARY_TEXT;
    }
    if (m_variant == SageStatusCardVariant::Completed) {
        return SAGE_COLOR_STATUS_CARD_TEXT_SUCCESS;
    }
    if (m_variant == SageStatusCardVariant::Failed) {
        return SAGE_COLOR_INLINE_ERROR_TEXT;
    }
    return SAGE_COLOR_TEXT;
}

QString SageStatusCard::elidedPath(const QString& path, const QFontMetrics& metrics, int width)
{
    const QString displayPath = QDir::toNativeSeparators(path);
    if (metrics.horizontalAdvance(displayPath) <= width) {
        return displayPath;
    }
    const QFileInfo pathInfo(path);
    if (pathInfo.fileName() == path) {
        return metrics.elidedText(displayPath, Qt::ElideMiddle, width);
    }
    const QString displayFolder = QDir::toNativeSeparators(pathInfo.path());
    const QString fileName = displayPath.mid(displayFolder.size());
    const int folderWidth = width - metrics.horizontalAdvance(fileName);
    const QString folder = metrics.elidedText(displayFolder, Qt::ElideMiddle, folderWidth);
    if (folder.isEmpty()) {
        return metrics.elidedText(displayPath, Qt::ElideMiddle, width);
    }
    return folder + fileName;
}
