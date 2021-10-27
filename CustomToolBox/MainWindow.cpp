#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ToolBox.h"
#include "Form.h"

#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QStringLiteral("自定义ToolBox演示  Qt小罗"));

    ToolBox *toolBox = new ToolBox(this);
    toolBox->addWidget(QStringLiteral("Qt小罗"), new Form());
    toolBox->addWidget(QStringLiteral("Qt小罗"), new Form());
    toolBox->addWidget(QStringLiteral("Qt小罗"), new Form());
    toolBox->addWidget(QStringLiteral("Qt小罗"), new Form());

    setCentralWidget(toolBox);
}

MainWindow::~MainWindow()
{
    delete ui;
}

