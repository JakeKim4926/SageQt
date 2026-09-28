#pragma once

#include <QDialog>
#include <QSize>
#include <QString>

class QFrame;
class QWidget;
class SageDialogCaptionBar;

class SageFramelessDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SageFramelessDlg(const QString& title, QWidget* parent = nullptr);

    QSize sizeHint() const override;

protected:
    QWidget* contentWidget() const;

private:
    void createWidgets(const QString& title);
    void createLayout();
    void connectSignals();

private:
    QFrame* m_frame = nullptr;
    SageDialogCaptionBar* m_captionBar = nullptr;
    QWidget* m_contentWidget = nullptr;
};
