#include "SageQt.h"

SageQt::SageQt(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SageQtClass())
{
    ui->setupUi(this);
}

SageQt::~SageQt()
{
    delete ui;
}

