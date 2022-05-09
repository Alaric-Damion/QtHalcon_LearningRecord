#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
void AboutDialog::switchLanguage()
{
    ui->retranslateUi(this);
}
void AboutDialog::on_closeBtn_clicked()
{
    this->close();
}
