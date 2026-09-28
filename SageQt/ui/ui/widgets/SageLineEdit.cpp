#include "ui/widgets/SageLineEdit.h"

SageLineEdit::SageLineEdit(QWidget* parent)
    : QLineEdit(parent)
{
}

SageLineEdit::SageLineEditVariant SageLineEdit::variant() const
{
    return m_variant;
}

void SageLineEdit::setVariant(SageLineEditVariant variant)
{
    if (m_variant == variant) {
        return;
    }
    m_variant = variant;
    update();
    emit variantChanged();
}
