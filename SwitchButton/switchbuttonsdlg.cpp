#include "switchbuttonsdlg.h"
#include "ui_switchbuttonsdlg.h"

SwitchButtonsDlg::SwitchButtonsDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwitchButtonsDlg)
{
    ui->setupUi(this);
}

SwitchButtonsDlg::~SwitchButtonsDlg()
{
    delete ui;
}
