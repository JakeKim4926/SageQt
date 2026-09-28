#pragma once

#include <QWidget>

class SageSurface : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(SageSurfaceVariant variant READ variant CONSTANT)

public:
    enum class SageSurfaceVariant
    {
        Sidebar,
        Panel
    };
    Q_ENUM(SageSurfaceVariant)

    explicit SageSurface(SageSurfaceVariant variant, QWidget* parent = nullptr);

    SageSurfaceVariant variant() const;

private:
    SageSurfaceVariant m_variant;
};
