#ifndef SWITCHBUTTONSDLG_H
#define SWITCHBUTTONSDLG_H

#include <QWidget>

namespace Ui {
class SwitchButtonsDlg;
}

class SwitchButtonsDlg : public QWidget
{
    Q_OBJECT

public:
    explicit SwitchButtonsDlg(QWidget *parent = 0);
    ~SwitchButtonsDlg();

private:
    Ui::SwitchButtonsDlg *ui;
};

#endif // SWITCHBUTTONSDLG_H
