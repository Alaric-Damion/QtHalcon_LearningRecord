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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout_5;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *lbl_DispHalcon;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLabel *lbl_DispCV;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QLabel *lbl_DispQImage;
    QWidget *widget;
    QGridLayout *gridLayout_4;
    QLabel *label_4;
    QComboBox *cmb_InputFormat;
    QLabel *label_5;
    QComboBox *cmb_OutputFormat;
    QLabel *label;
    QComboBox *cmb_ImgChannels;
    QPushButton *btn_LoadImg;
    QPushButton *btn_TransImg;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(837, 473);
        gridLayout_5 = new QGridLayout(Widget);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        groupBox = new QGroupBox(Widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        lbl_DispHalcon = new QLabel(groupBox);
        lbl_DispHalcon->setObjectName(QStringLiteral("lbl_DispHalcon"));

        gridLayout->addWidget(lbl_DispHalcon, 0, 0, 1, 1);


        gridLayout_5->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(Widget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        lbl_DispCV = new QLabel(groupBox_2);
        lbl_DispCV->setObjectName(QStringLiteral("lbl_DispCV"));

        gridLayout_2->addWidget(lbl_DispCV, 0, 0, 1, 1);


        gridLayout_5->addWidget(groupBox_2, 0, 1, 1, 1);

        groupBox_3 = new QGroupBox(Widget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        lbl_DispQImage = new QLabel(groupBox_3);
        lbl_DispQImage->setObjectName(QStringLiteral("lbl_DispQImage"));

        gridLayout_3->addWidget(lbl_DispQImage, 0, 0, 1, 1);


        gridLayout_5->addWidget(groupBox_3, 1, 0, 1, 1);

        widget = new QWidget(Widget);
        widget->setObjectName(QStringLiteral("widget"));
        gridLayout_4 = new QGridLayout(widget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label_4 = new QLabel(widget);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_4->addWidget(label_4, 0, 0, 1, 1);

        cmb_InputFormat = new QComboBox(widget);
        cmb_InputFormat->setObjectName(QStringLiteral("cmb_InputFormat"));

        gridLayout_4->addWidget(cmb_InputFormat, 0, 1, 1, 1);

        label_5 = new QLabel(widget);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_4->addWidget(label_5, 0, 2, 1, 1);

        cmb_OutputFormat = new QComboBox(widget);
        cmb_OutputFormat->setObjectName(QStringLiteral("cmb_OutputFormat"));

        gridLayout_4->addWidget(cmb_OutputFormat, 0, 3, 1, 1);

        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_4->addWidget(label, 1, 0, 1, 1);

        cmb_ImgChannels = new QComboBox(widget);
        cmb_ImgChannels->setObjectName(QStringLiteral("cmb_ImgChannels"));

        gridLayout_4->addWidget(cmb_ImgChannels, 1, 1, 1, 1);

        btn_LoadImg = new QPushButton(widget);
        btn_LoadImg->setObjectName(QStringLiteral("btn_LoadImg"));

        gridLayout_4->addWidget(btn_LoadImg, 1, 2, 1, 1);

        btn_TransImg = new QPushButton(widget);
        btn_TransImg->setObjectName(QStringLiteral("btn_TransImg"));

        gridLayout_4->addWidget(btn_TransImg, 1, 3, 1, 1);

        verticalSpacer = new QSpacerItem(20, 148, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer, 2, 1, 1, 1);


        gridLayout_5->addWidget(widget, 1, 1, 1, 1);

        gridLayout_5->setRowStretch(0, 1);
        gridLayout_5->setRowStretch(1, 1);
        gridLayout_5->setColumnStretch(0, 1);
        gridLayout_5->setColumnStretch(1, 1);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("Widget", "Halcon\347\252\227\345\217\243", Q_NULLPTR));
        lbl_DispHalcon->setText(QString());
        groupBox_2->setTitle(QApplication::translate("Widget", "OpenCV\347\252\227\345\217\243", Q_NULLPTR));
        lbl_DispCV->setText(QString());
        groupBox_3->setTitle(QApplication::translate("Widget", "QImage\347\252\227\345\217\243", Q_NULLPTR));
        lbl_DispQImage->setText(QApplication::translate("Widget", "TextLabel", Q_NULLPTR));
        label_4->setText(QApplication::translate("Widget", "\350\276\223\345\205\245\346\240\274\345\274\217\357\274\232", Q_NULLPTR));
        cmb_InputFormat->clear();
        cmb_InputFormat->insertItems(0, QStringList()
         << QApplication::translate("Widget", "HObject", Q_NULLPTR)
         << QApplication::translate("Widget", "Mat", Q_NULLPTR)
         << QApplication::translate("Widget", "QImage", Q_NULLPTR)
        );
        label_5->setText(QApplication::translate("Widget", "\350\276\223\345\207\272\346\240\274\345\274\217\357\274\232", Q_NULLPTR));
        cmb_OutputFormat->clear();
        cmb_OutputFormat->insertItems(0, QStringList()
         << QApplication::translate("Widget", "QImage", Q_NULLPTR)
         << QApplication::translate("Widget", "Mat", Q_NULLPTR)
         << QApplication::translate("Widget", "HObject", Q_NULLPTR)
        );
        label->setText(QApplication::translate("Widget", "\345\233\276\345\203\217\351\200\232\351\201\223\357\274\232", Q_NULLPTR));
        cmb_ImgChannels->clear();
        cmb_ImgChannels->insertItems(0, QStringList()
         << QApplication::translate("Widget", "Gray", Q_NULLPTR)
         << QApplication::translate("Widget", "RGB", Q_NULLPTR)
        );
        btn_LoadImg->setText(QApplication::translate("Widget", "\345\212\240\350\275\275\345\233\276\345\203\217", Q_NULLPTR));
        btn_TransImg->setText(QApplication::translate("Widget", "\350\275\254\346\215\242", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
