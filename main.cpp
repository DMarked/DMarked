#include "document.h"
#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("DMarkEd");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    MainWindow window;
    window.show();

    return a.exec();
}
