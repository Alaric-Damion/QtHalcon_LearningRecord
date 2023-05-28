#include "widget.h"
#include <QApplication>
#include "VLCQtCore/Common.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VlcCommon::setPluginPath(a.applicationDirPath() + "/plugins");
    Widget w;
    w.show();
    return a.exec();
}
