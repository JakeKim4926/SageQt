#pragma once

#include "ui/dialogs/SageFramelessDlg.h"

#include <QString>
#include <QStyle>

class QLabel;
class QWidget;
class SageButton;

enum class SageMessageIcon
{
    Info,
    Warning,
    Error
};

class SageMessageBoxDlg : public SageFramelessDlg
{
    Q_OBJECT

public:
    SageMessageBoxDlg(SageMessageIcon icon, const QString& message, QWidget* parent = nullptr);

private:
    void createWidgets(SageMessageIcon icon, const QString& message);
    void createLayout();
    void connectSignals();

    static QString captionTitle(SageMessageIcon icon);
    static QStyle::StandardPixmap standardPixmap(SageMessageIcon icon);

private:
    QLabel* m_iconLabel = nullptr;
    QLabel* m_messageLabel = nullptr;
    SageButton* m_okButton = nullptr;
};
