#include "ui/window/SageMainWindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    SageMainWindow mainWindow;
    mainWindow.show();
    return application.exec();
}
