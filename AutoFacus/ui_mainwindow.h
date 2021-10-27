/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *lblImage;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnAutoFocus;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnMaxDev;
    QPushButton *btnTimer;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *editInter;
    QSpacerItem *verticalSpacer_2;
    QLabel *labDev;
    QSpacerItem *verticalSpacer;
    QLabel *labmaxDev;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(521, 421);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        lblImage = new QLabel(centralwidget);
        lblImage->setObjectName(QStringLiteral("lblImage"));
        lblImage->setGeometry(QRect(20, 20, 301, 281));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 330, 282, 27));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btnAutoFocus = new QPushButton(layoutWidget);
        btnAutoFocus->setObjectName(QStringLiteral("btnAutoFocus"));

        horizontalLayout->addWidget(btnAutoFocus);

        horizontalSpacer = new QSpacerItem(108, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnMaxDev = new QPushButton(layoutWidget);
        btnMaxDev->setObjectName(QStringLiteral("btnMaxDev"));

        horizontalLayout->addWidget(btnMaxDev);

        btnTimer = new QPushButton(centralwidget);
        btnTimer->setObjectName(QStringLiteral("btnTimer"));
        btnTimer->setGeometry(QRect(330, 330, 89, 25));
        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(331, 131, 185, 143));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(layoutWidget1);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        editInter = new QLineEdit(layoutWidget1);
        editInter->setObjectName(QStringLiteral("editInter"));

        horizontalLayout_2->addWidget(editInter);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        labDev = new QLabel(layoutWidget1);
        labDev->setObjectName(QStringLiteral("labDev"));

        verticalLayout->addWidget(labDev);

        verticalSpacer = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        labmaxDev = new QLabel(layoutWidget1);
        labmaxDev->setObjectName(QStringLiteral("labmaxDev"));

        verticalLayout->addWidget(labmaxDev);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 521, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        lblImage->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        btnAutoFocus->setText(QApplication::translate("MainWindow", "AutoFocus", Q_NULLPTR));
        btnMaxDev->setText(QApplication::translate("MainWindow", "MaxDev", Q_NULLPTR));
        btnTimer->setText(QApplication::translate("MainWindow", "TImer", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "time interval(ms)", Q_NULLPTR));
        labDev->setText(QApplication::translate("MainWindow", "labDev", Q_NULLPTR));
        labmaxDev->setText(QApplication::translate("MainWindow", "labmaxDev", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
