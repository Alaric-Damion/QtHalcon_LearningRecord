/********************************************************************************
** Form generated from reading UI file 'ToolPage.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOLPAGE_H
#define UI_TOOLPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ToolPage
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *pushButtonFold;
    QWidget *widgetContent;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayoutContent;

    void setupUi(QWidget *ToolPage)
    {
        if (ToolPage->objectName().isEmpty())
            ToolPage->setObjectName(QStringLiteral("ToolPage"));
        ToolPage->resize(237, 278);
        verticalLayout = new QVBoxLayout(ToolPage);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButtonFold = new QPushButton(ToolPage);
        pushButtonFold->setObjectName(QStringLiteral("pushButtonFold"));
        pushButtonFold->setMinimumSize(QSize(0, 24));
        pushButtonFold->setCheckable(false);
        pushButtonFold->setFlat(true);

        verticalLayout->addWidget(pushButtonFold);

        widgetContent = new QWidget(ToolPage);
        widgetContent->setObjectName(QStringLiteral("widgetContent"));
        verticalLayout_3 = new QVBoxLayout(widgetContent);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayoutContent = new QVBoxLayout();
        verticalLayoutContent->setObjectName(QStringLiteral("verticalLayoutContent"));

        verticalLayout_3->addLayout(verticalLayoutContent);


        verticalLayout->addWidget(widgetContent);


        retranslateUi(ToolPage);

        QMetaObject::connectSlotsByName(ToolPage);
    } // setupUi

    void retranslateUi(QWidget *ToolPage)
    {
        ToolPage->setWindowTitle(QApplication::translate("ToolPage", "Form", Q_NULLPTR));
        pushButtonFold->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ToolPage: public Ui_ToolPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOLPAGE_H
