/********************************************************************************
** Form generated from reading UI file 'ToolBox.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOLBOX_H
#define UI_TOOLBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ToolBox
{
public:
    QVBoxLayout *verticalLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_3;

    void setupUi(QWidget *ToolBox)
    {
        if (ToolBox->objectName().isEmpty())
            ToolBox->setObjectName(QStringLiteral("ToolBox"));
        ToolBox->resize(193, 375);
        verticalLayout_2 = new QVBoxLayout(ToolBox);
        verticalLayout_2->setSpacing(3);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(3, 3, 3, 3);
        scrollArea = new QScrollArea(ToolBox);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 185, 367));
        verticalLayout_3 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_2->addWidget(scrollArea);


        retranslateUi(ToolBox);

        QMetaObject::connectSlotsByName(ToolBox);
    } // setupUi

    void retranslateUi(QWidget *ToolBox)
    {
        ToolBox->setWindowTitle(QApplication::translate("ToolBox", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ToolBox: public Ui_ToolBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOLBOX_H
