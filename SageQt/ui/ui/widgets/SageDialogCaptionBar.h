#pragma once

#include <QSize>
#include <QString>
#include <QWidget>

class QMouseEvent;
class QPaintEvent;
class QToolButton;

class SageDialogCaptionBar : public QWidget
{
    Q_OBJECT

public:
    explicit SageDialogCaptionBar(const QString& title, QWidget* parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void closeRequested();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    void createWidgets();
    void createLayout();
    void connectSignals();

private:
    QString m_title;
    QToolButton* m_closeButton = nullptr;
};
