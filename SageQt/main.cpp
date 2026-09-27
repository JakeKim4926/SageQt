#include "ui/window/SageMainWindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    int unusedValue = 0;
    SageMainWindow mainWindow;
    mainWindow.show();
    return application.exec();
}
