/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout;
    QPushButton *btnOpenVedio;
    QSpacerItem *horizontalSpacer;
    QCheckBox *ckb_Track;
    QPushButton *btnQuit;
    QLabel *picshow;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(942, 652);
        gridLayout = new QGridLayout(Widget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        btnOpenVedio = new QPushButton(Widget);
        btnOpenVedio->setObjectName(QStringLiteral("btnOpenVedio"));

        gridLayout->addWidget(btnOpenVedio, 1, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(763, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        ckb_Track = new QCheckBox(Widget);
        ckb_Track->setObjectName(QStringLiteral("ckb_Track"));

        gridLayout->addWidget(ckb_Track, 1, 1, 1, 1);

        btnQuit = new QPushButton(Widget);
        btnQuit->setObjectName(QStringLiteral("btnQuit"));

        gridLayout->addWidget(btnQuit, 1, 3, 1, 1);

        picshow = new QLabel(Widget);
        picshow->setObjectName(QStringLiteral("picshow"));

        gridLayout->addWidget(picshow, 0, 0, 1, 4);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        btnOpenVedio->setText(QApplication::translate("Widget", "\346\211\223\345\274\200\346\221\204\345\203\217\345\244\264", Q_NULLPTR));
        ckb_Track->setText(QApplication::translate("Widget", "\346\211\223\345\274\200\350\277\275\350\270\252", Q_NULLPTR));
        btnQuit->setText(QApplication::translate("Widget", "\351\200\200\345\207\272", Q_NULLPTR));
        picshow->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
