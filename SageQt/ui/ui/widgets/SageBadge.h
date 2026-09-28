#pragma once

#include <QSize>
#include <QString>
#include <QWidget>

class QPaintEvent;

class SageBadge : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(SageBadgeVariant variant READ variant CONSTANT)

public:
    enum class SageBadgeVariant
    {
        Neutral
    };
    Q_ENUM(SageBadgeVariant)

    explicit SageBadge(SageBadgeVariant variant, QWidget* parent = nullptr);

    SageBadgeVariant variant() const;
    QString text() const;
    void setText(const QString& text);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    SageBadgeVariant m_variant;
    QString m_text;
};
