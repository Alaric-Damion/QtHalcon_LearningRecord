#include "widget.h"
#include <QTranslator>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load(":/translation/Chinese.qm");
    a.installTranslator(&translator);

    Widget w;
    w.setTranslator(&translator);
    w.setTranSet();
    w.show();
    return a.exec();
}
