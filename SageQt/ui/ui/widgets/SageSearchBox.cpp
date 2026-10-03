#include "ui/widgets/SageSearchBox.h"

#include "SageDefine.h"
#include "ui/style/SageDesignDefine.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QSignalBlocker>
#include <qdrawutil.h>

SageSearchBox::SageSearchBox(QWidget* parent)
    : QWidget(parent)
{
    m_criteriaCombo = new QComboBox(this);
    m_criteriaCombo->setMaxVisibleItems(SAGE_RESULT_CRITERIA_DROP_ROWS);
    m_criteriaCombo->setFixedWidth(SAGE_SEARCH_CRITERIA_CELL_WIDTH - SAGE_EDIT_BORDER_WIDTH);
    m_keywordEdit = new QLineEdit(this);
    m_keywordEdit->setFrame(false);
    m_keywordEdit->setMaxLength(SAGE_RESULT_FILTER_MAX_LENGTH);
    m_keywordEdit->setPlaceholderText(SAGE_UI_RESULT_FILTER_PLACEHOLDER);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(SAGE_EDIT_BORDER_WIDTH, SAGE_EDIT_BORDER_WIDTH, SAGE_EDIT_BORDER_WIDTH,
                               SAGE_EDIT_BORDER_WIDTH);
    layout->setSpacing(0);
    layout->addWidget(m_criteriaCombo);
    layout->addSpacing(SAGE_EDIT_BORDER_WIDTH + SAGE_EDIT_TEXT_LEFT_PAD);
    layout->addWidget(m_keywordEdit, 1);
    layout->addSpacing(SAGE_EDIT_TEXT_LEFT_PAD + SAGE_SEARCH_ICON_CELL_WIDTH - SAGE_EDIT_BORDER_WIDTH);

    connect(m_keywordEdit, &QLineEdit::returnPressed, this, &SageSearchBox::searchRequested);
    connect(m_criteriaCombo, &QComboBox::activated, this, &SageSearchBox::onCriteriaComboActivated);
}

void SageSearchBox::setCriteria(const QList<SageWorkflowFilterCriteria>& criteria, int selectedCriteria)
{
    const QSignalBlocker blocker(m_criteriaCombo);
    m_criteriaCombo->clear();
    for (const SageWorkflowFilterCriteria& definition : criteria) {
        m_criteriaCombo->addItem(definition.m_label, definition.m_criteria);
    }
    const int selectedIndex = m_criteriaCombo->findData(selectedCriteria);
    m_criteriaCombo->setCurrentIndex(selectedIndex < 0 ? 0 : selectedIndex);
}

std::optional<int> SageSearchBox::selectedCriteria() const
{
    if (m_criteriaCombo->currentIndex() < 0) {
        return std::nullopt;
    }
    return m_criteriaCombo->currentData().toInt();
}

QString SageSearchBox::keyword() const
{
    return m_keywordEdit->text();
}

void SageSearchBox::setKeyword(const QString& keyword)
{
    m_keywordEdit->setText(keyword);
}

QSize SageSearchBox::sizeHint() const
{
    return {SAGE_SEARCH_CRITERIA_CELL_WIDTH + SAGE_RESULT_FILTER_WIDTH + SAGE_SEARCH_ICON_CELL_WIDTH, SAGE_EDIT_HEIGHT};
}

QSize SageSearchBox::minimumSizeHint() const
{
    return sizeHint();
}

void SageSearchBox::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    const QBrush face(SAGE_COLOR_PANEL);
    qDrawPlainRect(&painter, rect(), SAGE_COLOR_BUTTON_BORDER, SAGE_EDIT_BORDER_WIDTH, &face);

    const QRect criteriaCell = criteriaCellRect();
    painter.fillRect(criteriaCell, SAGE_COLOR_APP_BACKGROUND);
    painter.fillRect(QRect(criteriaCell.right() + 1, criteriaCell.top(), SAGE_EDIT_BORDER_WIDTH, criteriaCell.height()),
                     SAGE_COLOR_LIST_HEADER_BORDER);

    const QRect iconCell = iconCellRect();
    painter.fillRect(iconCell, SAGE_COLOR_APP_BACKGROUND);
    painter.fillRect(QRect(iconCell.left(), iconCell.top(), SAGE_EDIT_BORDER_WIDTH, iconCell.height()),
                     SAGE_COLOR_LIST_HEADER_BORDER);

    const QPointF lensCenter =
        QPointF(iconCell.center()) - QPointF(SAGE_ICON_SEARCH_HANDLE / 2, SAGE_ICON_SEARCH_HANDLE / 2);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(SAGE_COLOR_TEXT_MUTED, SAGE_ICON_STROKE));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(lensCenter, SAGE_ICON_SEARCH_RADIUS, SAGE_ICON_SEARCH_RADIUS);
    const double handleStart = SAGE_ICON_SEARCH_RADIUS - SAGE_EDIT_BORDER_WIDTH;
    const double handleEnd = SAGE_ICON_SEARCH_RADIUS + SAGE_ICON_SEARCH_HANDLE;
    painter.drawLine(lensCenter + QPointF(handleStart, handleStart), lensCenter + QPointF(handleEnd, handleEnd));
}

void SageSearchBox::mousePressEvent(QMouseEvent* event)
{
    if (iconCellRect().contains(event->position().toPoint())) {
        emit searchRequested();
        return;
    }
    m_keywordEdit->setFocus();
}

void SageSearchBox::onCriteriaComboActivated(int index)
{
    emit criteriaChanged(m_criteriaCombo->itemData(index).toInt());
}

QRect SageSearchBox::criteriaCellRect() const
{
    return QRect(SAGE_EDIT_BORDER_WIDTH, SAGE_EDIT_BORDER_WIDTH,
                 SAGE_SEARCH_CRITERIA_CELL_WIDTH - SAGE_EDIT_BORDER_WIDTH, height() - SAGE_EDIT_BORDER_WIDTH * 2);
}

QRect SageSearchBox::iconCellRect() const
{
    return QRect(width() - SAGE_SEARCH_ICON_CELL_WIDTH, SAGE_EDIT_BORDER_WIDTH,
                 SAGE_SEARCH_ICON_CELL_WIDTH - SAGE_EDIT_BORDER_WIDTH, height() - SAGE_EDIT_BORDER_WIDTH * 2);
}
