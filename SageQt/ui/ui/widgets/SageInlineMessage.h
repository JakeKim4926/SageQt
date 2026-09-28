#pragma once

#include <QSize>
#include <QString>
#include <QWidget>

class QPaintEvent;

class SageInlineMessage : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(SageInlineMessageVariant variant READ variant CONSTANT)

public:
    enum class SageInlineMessageVariant
    {
        Error
    };
    Q_ENUM(SageInlineMessageVariant)

    explicit SageInlineMessage(SageInlineMessageVariant variant, QWidget* parent = nullptr);

    SageInlineMessageVariant variant() const;
    QString message() const;
    void setMessage(const QString& message);
    void clearMessage();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    SageInlineMessageVariant m_variant;
    QString m_message;
};
