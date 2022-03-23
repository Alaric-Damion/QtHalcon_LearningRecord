#include "switchbuttonsdlg.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SwitchButtonsDlg w;
    w.show();

    return a.exec();
}
