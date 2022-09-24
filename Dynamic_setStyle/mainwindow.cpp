#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->button1->setProperty("pagematches", true);
    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    //清楚所有按钮的选中样式
    ui->button1->setProperty("pagematches", false);
    ui->button2->setProperty("pagematches", false);
    ui->button3->setProperty("pagematches", false);

    // 设置tab对应的按钮选中样式
    if (index == 0)
        ui->button1->setProperty("pagematches", true);
    else if (index == 1)
        ui->button2->setProperty("pagematches", true);
    else
        ui->button3->setProperty("pagematches", true);

    // 每个按钮应用最新的样式设置
    ui->button1->style()->polish(ui->button1);
    ui->button2->style()->polish(ui->button2);
    ui->button3->style()->polish(ui->button3);
}

void MainWindow::on_button1_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->tab1);

}

void MainWindow::on_button2_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->tab2);
}

void MainWindow::on_button3_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->tab3);
}
