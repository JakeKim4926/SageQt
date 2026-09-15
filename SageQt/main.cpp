#include "SageQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SageQt window;
    window.show();
    return app.exec();
}
