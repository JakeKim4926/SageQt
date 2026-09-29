#include "ui/panels/SageWorkflowInputPanel.h"

#include "SageDefine.h"
#include "core/workflow/ISageWorkflowHandler.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/widgets/SageButton.h"
#include "ui/widgets/SageLabel.h"
#include "ui/widgets/SageLineEdit.h"

#include <QDir>
#include <QFileDialog>
#include <QFrame>
#include <QGridLayout>
#include <QPalette>
#include <QVBoxLayout>

SageWorkflowInputPanel::SageWorkflowInputPanel(QWidget* parent)
    : QWidget(parent)
{
    createWidgets();
    createLayout();
    connectSignals();
}

QString SageWorkflowInputPanel::inputPath() const
{
    return QDir::fromNativeSeparators(m_inputPathEdit->text());
}

void SageWorkflowInputPanel::setInputPath(const QString& inputPath)
{
    m_inputPathEdit->setText(QDir::toNativeSeparators(inputPath));
}

QString SageWorkflowInputPanel::outputFolder() const
{
    return QDir::fromNativeSeparators(m_outputFolderEdit->text());
}

void SageWorkflowInputPanel::setOutputFolder(const QString& outputFolder)
{
    m_outputFolderEdit->setText(QDir::toNativeSeparators(outputFolder));
}

void SageWorkflowInputPanel::applyHandler(const ISageWorkflowHandler& handler)
{
    m_inputLabel->setText(handler.inputSectionLabel());
    m_inputDialogTitle = handler.inputDialogTitle();
    m_inputFileFilter = handler.inputFileFilter();
    m_autoLoadOnInput = handler.hasInputTable();
    m_runButton->setText(handler.actionButtonLabel());
}

void SageWorkflowInputPanel::setRunningState(bool running)
{
    m_selectInputButton->setEnabled(!running);
    m_selectOutputButton->setEnabled(!running);
    m_runButton->setEnabled(!running);
}

void SageWorkflowInputPanel::onSelectInputClicked()
{
    const QString selectedPath = QFileDialog::getOpenFileName(this, m_inputDialogTitle, QString(), m_inputFileFilter);
    if (!selectedPath.isEmpty()) {
        setInputPath(selectedPath);
        if (m_autoLoadOnInput) {
            emit runRequested(SageTaskType::Load);
        }
    }
}

void SageWorkflowInputPanel::onSelectOutputClicked()
{
    const QString selectedFolder = QFileDialog::getExistingDirectory(this, SAGE_UI_SELECT_OUTPUT_TITLE);
    if (!selectedFolder.isEmpty()) {
        setOutputFolder(selectedFolder);
    }
}

void SageWorkflowInputPanel::onRunClicked()
{
    emit runRequested(SageTaskType::Generate);
}

void SageWorkflowInputPanel::createWidgets()
{
    m_inputCard = new QFrame(this);
    m_inputCard->setFrameShape(QFrame::Box);
    m_inputCard->setLineWidth(SAGE_BORDER_THICKNESS);
    m_inputCard->setBackgroundRole(QPalette::Base);
    m_inputCard->setAutoFillBackground(true);

    m_cardTitleLabel = new SageLabel(SageLabel::SageLabelVariant::Section, SAGE_UI_INPUT_CARD_TITLE, m_inputCard);
    m_cardTitleLabel->setFixedHeight(SAGE_CARD_HEADER_HEIGHT - SAGE_BORDER_THICKNESS);
    m_cardTitleLabel->setIndent(SAGE_CARD_PADDING);
    m_cardTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_cardTitleLine = new QFrame(m_inputCard);
    m_cardTitleLine->setFrameShape(QFrame::HLine);
    m_cardTitleLine->setFixedHeight(SAGE_BORDER_THICKNESS);

    m_formArea = new QWidget(m_inputCard);
    m_inputLabel = new SageLabel(SageLabel::SageLabelVariant::FormLabel, SAGE_UI_SECTION_INPUT, m_formArea);
    m_inputLabel->setMinimumWidth(SAGE_FORM_LABEL_WIDTH);
    m_outputLabel = new SageLabel(SageLabel::SageLabelVariant::FormLabel, SAGE_UI_SECTION_OUTPUT, m_formArea);
    m_outputLabel->setMinimumWidth(SAGE_FORM_LABEL_WIDTH);
    m_inputPathEdit = createPathEdit();
    m_outputFolderEdit = createPathEdit();
    m_selectInputButton = new SageButton(SAGE_UI_INPUT_BUTTON, m_formArea);
    m_selectInputButton->setMinimumWidth(SAGE_BUTTON_WIDTH);
    m_selectOutputButton = new SageButton(SAGE_UI_OUTPUT_BUTTON, m_formArea);
    m_selectOutputButton->setMinimumWidth(SAGE_BUTTON_WIDTH);
    m_runButton = new SageButton(QString(), m_formArea);
    m_runButton->setVariant(SageButton::SageButtonVariant::Primary);
    m_runButton->setMinimumWidth(SAGE_BUTTON_WIDTH);
    m_runButton->setFixedHeight(SAGE_CARD_ACTION_BUTTON_HEIGHT);
}

void SageWorkflowInputPanel::createLayout()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_inputCard);
    layout->addStretch();

    QVBoxLayout* cardLayout = new QVBoxLayout(m_inputCard);
    cardLayout->setContentsMargins(0, 0, 0, 0);
    cardLayout->setSpacing(0);
    cardLayout->addWidget(m_cardTitleLabel);
    cardLayout->addWidget(m_cardTitleLine);
    cardLayout->addWidget(m_formArea);

    QGridLayout* formLayout = new QGridLayout(m_formArea);
    formLayout->setContentsMargins(SAGE_CARD_PADDING, SAGE_CARD_PADDING, SAGE_CARD_PADDING, SAGE_CARD_PADDING);
    formLayout->setHorizontalSpacing(SAGE_CARD_ROW_GAP);
    formLayout->setVerticalSpacing(SAGE_CARD_ROW_GAP);
    formLayout->addWidget(m_inputLabel, 0, 0);
    formLayout->addWidget(m_inputPathEdit, 0, 1);
    formLayout->addWidget(m_selectInputButton, 0, 2);
    formLayout->addWidget(m_outputLabel, 1, 0);
    formLayout->addWidget(m_outputFolderEdit, 1, 1);
    formLayout->addWidget(m_selectOutputButton, 1, 2);
    formLayout->addWidget(m_runButton, 2, 1, Qt::AlignLeft);
    formLayout->setColumnStretch(1, 1);
}

void SageWorkflowInputPanel::connectSignals()
{
    connect(m_selectInputButton, &SageButton::clicked, this, &SageWorkflowInputPanel::onSelectInputClicked);
    connect(m_selectOutputButton, &SageButton::clicked, this, &SageWorkflowInputPanel::onSelectOutputClicked);
    connect(m_runButton, &SageButton::clicked, this, &SageWorkflowInputPanel::onRunClicked);
}

SageLineEdit* SageWorkflowInputPanel::createPathEdit()
{
    SageLineEdit* edit = new SageLineEdit(m_formArea);
    edit->setReadOnly(true);
    edit->setAcceptDrops(false);
    edit->setMinimumWidth(SAGE_CO_COMPANY_EDIT_MIN_WIDTH);
    return edit;
}
