#include "ui/widgets/SageSelectionBar.h"

#include "SageDefine.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/widgets/SageButton.h"
#include "ui/widgets/SageSelectionCountLabel.h"

#include <QCheckBox>
#include <QFontMetrics>
#include <QHBoxLayout>

SageSelectionBar::SageSelectionBar(QWidget* parent)
    : QWidget(parent)
{
    m_selectAllCheck = new QCheckBox(SAGE_UI_SELECT_ALL_BUTTON, this);
    m_countLabel = new SageSelectionCountLabel(this);
    m_clearButton = new SageButton(SAGE_UI_SELECTION_CLEAR_BUTTON, this);
    m_clearButton->setVariant(SageButton::SageButtonVariant::Ghost);
    m_clearButton->setFixedWidth(QFontMetrics(m_clearButton->font()).horizontalAdvance(SAGE_UI_SELECTION_CLEAR_BUTTON) +
                                 SAGE_SELECTION_CLEAR_PAD * 2);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(SAGE_SELECTION_BAR_GAP);
    layout->addWidget(m_selectAllCheck);
    layout->addWidget(m_countLabel);
    layout->addWidget(m_clearButton);
    setFixedHeight(SAGE_BUTTON_HEIGHT);

    connect(m_selectAllCheck, &QCheckBox::clicked, this, &SageSelectionBar::selectAllClicked);
    connect(m_clearButton, &SageButton::clicked, this, &SageSelectionBar::clearClicked);
}

void SageSelectionBar::setCounts(int totalCount, int selectedCount)
{
    m_countLabel->setCounts(totalCount, selectedCount);
}

void SageSelectionBar::setAllChecked(bool checked)
{
    m_selectAllCheck->setChecked(checked);
}

void SageSelectionBar::setControlsEnabled(bool enabled)
{
    m_selectAllCheck->setEnabled(enabled);
    m_clearButton->setEnabled(enabled);
}
