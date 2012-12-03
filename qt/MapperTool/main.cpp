#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("mappertool");
    a.setOrganizationName("fieldofview");
    a.setOrganizationDomain("fieldofview.com");

    MainWindow w;
    w.show();
    
    return a.exec();
}
