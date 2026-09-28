#pragma once

#include <QLineEdit>

class SageLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(SageLineEditVariant variant READ variant WRITE setVariant NOTIFY variantChanged)

public:
    enum class SageLineEditVariant
    {
        Normal,
        Error
    };
    Q_ENUM(SageLineEditVariant)

    explicit SageLineEdit(QWidget* parent = nullptr);

    SageLineEditVariant variant() const;
    void setVariant(SageLineEditVariant variant);

signals:
    void variantChanged();

private:
    SageLineEditVariant m_variant = SageLineEditVariant::Normal;
};
