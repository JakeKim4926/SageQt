#include "ui/panels/SageWorkflowInputPanel.h"

#include "SageDefine.h"
#include "core/workflow/ISageWorkflowHandler.h"
#include "ui/panels/SageResultTablePanel.h"
#include "ui/style/SageDesignDefine.h"
#include "ui/widgets/SageButton.h"
#include "ui/widgets/SageLabel.h"
#include "ui/widgets/SageLineEdit.h"
#include "ui/widgets/SageStatusCard.h"

#include <QDir>
#include <QFileDialog>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPalette>
#include <QTimer>
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
    m_running = running;
    m_selectInputButton->setEnabled(!running);
    m_selectOutputButton->setEnabled(!running);
    m_runButton->setEnabled(!running);
    m_inputResetButton->setEnabled(!running);
    m_inputTable->setSelectionControlsEnabled(!running);
    m_emptyHintArea->setVisible(!m_inputTableVisible && !running);
    if (running) {
        m_statusCard->setRunning(SAGE_UI_STATUS_CARD_RUNNING);
        m_progressPercent = 0;
        m_statusCard->setProgressPercent(m_progressPercent);
        m_progressTimer->start();
        return;
    }
    m_progressTimer->stop();
}

void SageWorkflowInputPanel::setStatusResult(bool success, const QString& message, const QString& detail)
{
    m_statusCard->setResult(success, message, detail);
}

void SageWorkflowInputPanel::resetStatusCard()
{
    m_statusCard->setIdle(SAGE_UI_STATUS_CARD_IDLE);
}

void SageWorkflowInputPanel::setGenerateEnabled(bool enabled)
{
    m_runButton->setEnabled(enabled);
}

void SageWorkflowInputPanel::setInputResetVisible(bool visible)
{
    m_inputResetButton->setVisible(visible);
}

void SageWorkflowInputPanel::setInputTableVisible(bool tableVisible, bool filterVisible)
{
    m_inputTableVisible = tableVisible;
    m_inputTable->showSelectAll(tableVisible);
    m_inputTable->showFilter(tableVisible && filterVisible);
    m_inputTable->setVisible(tableVisible);
    m_emptyHintArea->setVisible(!tableVisible && !m_running);
}

SageResultTablePanel& SageWorkflowInputPanel::inputTable()
{
    return *m_inputTable;
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

void SageWorkflowInputPanel::onProgressTimer()
{
    if (!m_running || m_progressPercent >= SAGE_PROGRESS_RUNNING_MAX) {
        return;
    }
    m_progressPercent = qMin(m_progressPercent + SAGE_PROGRESS_STEP, SAGE_PROGRESS_RUNNING_MAX);
    m_statusCard->setProgressPercent(m_progressPercent);
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

    m_inputResetButton = new SageButton(SAGE_UI_INPUT_RESET_BTN, m_formArea);
    m_inputResetButton->setVariant(SageButton::SageButtonVariant::Ghost);
    m_inputResetButton->setMinimumWidth(SAGE_INPUT_RESET_WIDTH);
    m_inputResetButton->setFixedHeight(SAGE_CARD_ACTION_BUTTON_HEIGHT);
    m_inputResetButton->hide();

    m_inputTable = new SageResultTablePanel(this);
    m_inputTable->hide();
    m_emptyHintArea = new QWidget(this);
    m_emptyHintLabel = new SageLabel(SageLabel::SageLabelVariant::Hint, SAGE_UI_EMPTY_STATE_HINT, m_emptyHintArea);
    m_emptyHintLabel->setAlignment(Qt::AlignCenter);
    m_emptyHintLabel->setMinimumHeight(SAGE_RESULT_MIN_HEIGHT);

    m_statusCard = new SageStatusCard(this);
    m_statusCard->setIdle(SAGE_UI_STATUS_CARD_IDLE);
    m_progressTimer = new QTimer(this);
    m_progressTimer->setInterval(SAGE_PROGRESS_TIMER_MS);
}

void SageWorkflowInputPanel::createLayout()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_inputCard);
    layout->addSpacing(SAGE_CARD_GAP);
    layout->addWidget(m_statusCard);
    layout->addSpacing(SAGE_CARD_GAP - SAGE_RESULT_FILTER_TOP_LIFT - SAGE_RESULT_FILTER_BOX_PAD);
    layout->addWidget(m_inputTable, 1);
    layout->addWidget(m_emptyHintArea, 1);

    QVBoxLayout* hintLayout = new QVBoxLayout(m_emptyHintArea);
    hintLayout->setContentsMargins(0, SAGE_RESULT_FILTER_TOP_LIFT + SAGE_RESULT_FILTER_BOX_PAD, 0,
                                   SAGE_RESULT_HEADER_HEIGHT);
    hintLayout->addWidget(m_emptyHintLabel);

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
    QHBoxLayout* actionLayout = new QHBoxLayout();
    formLayout->addLayout(actionLayout, 2, 1);
    actionLayout->setSpacing(SAGE_ACTION_GAP);
    actionLayout->addWidget(m_runButton);
    actionLayout->addWidget(m_inputResetButton);
    actionLayout->addStretch();
    formLayout->setColumnStretch(1, 1);
}

void SageWorkflowInputPanel::connectSignals()
{
    connect(m_selectInputButton, &SageButton::clicked, this, &SageWorkflowInputPanel::onSelectInputClicked);
    connect(m_selectOutputButton, &SageButton::clicked, this, &SageWorkflowInputPanel::onSelectOutputClicked);
    connect(m_runButton, &SageButton::clicked, this, &SageWorkflowInputPanel::onRunClicked);
    connect(m_progressTimer, &QTimer::timeout, this, &SageWorkflowInputPanel::onProgressTimer);
    connect(m_inputResetButton, &SageButton::clicked, this, &SageWorkflowInputPanel::inputResetRequested);
    connect(m_statusCard, &SageStatusCard::openFolderRequested, this,
            &SageWorkflowInputPanel::openOutputFolderRequested);
}

SageLineEdit* SageWorkflowInputPanel::createPathEdit()
{
    SageLineEdit* edit = new SageLineEdit(m_formArea);
    edit->setReadOnly(true);
    edit->setAcceptDrops(false);
    edit->setMinimumWidth(SAGE_CO_COMPANY_EDIT_MIN_WIDTH);
    return edit;
}
