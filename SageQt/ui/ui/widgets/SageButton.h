#pragma once

#include <QPushButton>
#include <QString>

class SageButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(SageButtonVariant variant READ variant WRITE setVariant NOTIFY variantChanged)

public:
    enum class SageButtonVariant
    {
        Secondary,
        Primary,
        Ghost
    };
    Q_ENUM(SageButtonVariant)

    explicit SageButton(const QString& text, QWidget* parent = nullptr);

    SageButtonVariant variant() const;
    void setVariant(SageButtonVariant variant);

signals:
    void variantChanged();

private:
    SageButtonVariant m_variant = SageButtonVariant::Secondary;
};
