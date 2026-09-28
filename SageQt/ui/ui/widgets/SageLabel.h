#pragma once

#include <QLabel>
#include <QString>

class SageLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(SageLabelVariant variant READ variant CONSTANT)

public:
    enum class SageLabelVariant
    {
        Title,
        SecondaryCaption,
        MutedCaption,
        FormLabel,
        SidebarLogo
    };
    Q_ENUM(SageLabelVariant)

    SageLabel(SageLabelVariant variant, const QString& text, QWidget* parent = nullptr);

    SageLabelVariant variant() const;

private:
    SageLabelVariant m_variant;
};
