#include "mainwindow.h"
#include <QApplication>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    // TODO uncomment this line before release
    // right now we always need it
    QLoggingCategory::setFilterRules(QStringLiteral("qt.modbus* = true"));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
