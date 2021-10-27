#include "ToolBox.h"
#include "ui_ToolBox.h"
#include "ToolPage.h"

#include <QVBoxLayout>

ToolBox::ToolBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolBox),
    m_pContentVBoxLayout(nullptr)
{
    ui->setupUi(this);

    QWidget *widget = new QWidget(this);
    m_pContentVBoxLayout = new QVBoxLayout;
    m_pContentVBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_pContentVBoxLayout->setSpacing(2);

    QVBoxLayout *vBoxLayout = new QVBoxLayout(widget);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->addLayout(m_pContentVBoxLayout);
    vBoxLayout->addStretch(1);

    ui->scrollArea->setWidget(widget);
}

ToolBox::~ToolBox()
{
    delete ui;
}

void ToolBox::addWidget(const QString &title, QWidget *widget)
{
    ToolPage *page = new ToolPage(this);
    page->addWidget(title, widget);

    m_pContentVBoxLayout->addWidget(page);
}
