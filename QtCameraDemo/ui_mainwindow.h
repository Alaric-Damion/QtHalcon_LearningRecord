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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include "qvideowidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QFrame *centralWidget;
    QGridLayout *gridLayout;
    QVideoWidget *widgetCamera;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QComboBox *cmb_CameraName;
    QLabel *label_4;
    QComboBox *cmb_Resolution;
    QLabel *label_5;
    QComboBox *cmb_MaxFrameRate;
    QPushButton *btnSnap;
    QLabel *label_2;
    QSpinBox *spinBox;
    QPushButton *btnVedio;
    QSpacerItem *horizontalSpacer;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(579, 354);
        centralWidget = new QFrame(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widgetCamera = new QVideoWidget(centralWidget);
        widgetCamera->setObjectName(QStringLiteral("widgetCamera"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widgetCamera->sizePolicy().hasHeightForWidth());
        widgetCamera->setSizePolicy(sizePolicy);

        gridLayout->addWidget(widgetCamera, 0, 0, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout->addWidget(label_3);

        cmb_CameraName = new QComboBox(centralWidget);
        cmb_CameraName->setObjectName(QStringLiteral("cmb_CameraName"));

        horizontalLayout->addWidget(cmb_CameraName);

        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout->addWidget(label_4);

        cmb_Resolution = new QComboBox(centralWidget);
        cmb_Resolution->setObjectName(QStringLiteral("cmb_Resolution"));

        horizontalLayout->addWidget(cmb_Resolution);

        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout->addWidget(label_5);

        cmb_MaxFrameRate = new QComboBox(centralWidget);
        cmb_MaxFrameRate->setObjectName(QStringLiteral("cmb_MaxFrameRate"));

        horizontalLayout->addWidget(cmb_MaxFrameRate);

        btnSnap = new QPushButton(centralWidget);
        btnSnap->setObjectName(QStringLiteral("btnSnap"));

        horizontalLayout->addWidget(btnSnap);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        spinBox = new QSpinBox(centralWidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setMinimum(20);
        spinBox->setMaximum(300);
        spinBox->setSingleStep(10);

        horizontalLayout->addWidget(spinBox);

        btnVedio = new QPushButton(centralWidget);
        btnVedio->setObjectName(QStringLiteral("btnVedio"));

        horizontalLayout->addWidget(btnVedio);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 2);

        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 1);
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\346\210\221\347\232\204\346\221\204\345\203\217\345\244\264", Q_NULLPTR));
        label->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "\346\221\204\345\203\217\345\244\264\357\274\232", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "\345\210\206\350\276\250\347\216\207\357\274\232", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "\346\234\200\345\244\247\345\270\247\347\216\207\357\274\232", Q_NULLPTR));
        btnSnap->setText(QApplication::translate("MainWindow", "\345\215\225\345\274\240\346\212\223\346\213\215", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\351\227\264\351\232\224\346\227\266\351\227\264\357\274\232", Q_NULLPTR));
        btnVedio->setText(QApplication::translate("MainWindow", "\350\277\236\347\273\255\346\212\223\346\213\215", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
