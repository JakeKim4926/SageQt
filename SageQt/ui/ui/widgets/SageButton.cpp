#include "ui/widgets/SageButton.h"

SageButton::SageButton(const QString& text, QWidget* parent)
    : QPushButton(text, parent)
{
}

SageButton::SageButtonVariant SageButton::variant() const
{
    return m_variant;
}

void SageButton::setVariant(SageButtonVariant variant)
{
    if (m_variant == variant) {
        return;
    }
    m_variant = variant;
    update();
    emit variantChanged();
}
