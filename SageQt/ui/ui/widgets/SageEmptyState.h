#pragma once

#include <QRect>
#include <QString>
#include <QWidget>

class QPainter;
class QPaintEvent;

class SageEmptyState : public QWidget
{
    Q_OBJECT

public:
    explicit SageEmptyState(QWidget* parent = nullptr);

    void setContent(const QString& title, const QString& description);
    QString title() const;
    QString description() const;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int textWidth() const;
    int descriptionHeight(int width) const;
    static void drawIconBox(QPainter& painter, const QRect& box);

private:
    QString m_title;
    QString m_description;
};
