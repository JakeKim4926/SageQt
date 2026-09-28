#include "ui/widgets/SageLabel.h"

SageLabel::SageLabel(SageLabelVariant variant, const QString& text, QWidget* parent)
    : QLabel(text, parent)
    , m_variant(variant)
{
}

SageLabel::SageLabelVariant SageLabel::variant() const
{
    return m_variant;
}
