#include "ui/widgets/SageSurface.h"

SageSurface::SageSurface(SageSurfaceVariant variant, QWidget* parent)
    : QWidget(parent)
    , m_variant(variant)
{
}

SageSurface::SageSurfaceVariant SageSurface::variant() const
{
    return m_variant;
}
