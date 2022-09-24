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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *lbl_GrayImg;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLabel *lbl_RGBImg;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_LoadPic;
    QLabel *label_3;
    QLineEdit *edt_ImgFolderPath;
    QPushButton *btn_OpenFile;
    QPushButton *btn_NextPic;
    QLabel *label;
    QLineEdit *edt_ImgSavePath;
    QPushButton *btn_SaveImgPath;
    QPushButton *btn_trans;
    QPushButton *btn_BatchTrans;
    QPushButton *btn_close;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(821, 384);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Widget->sizePolicy().hasHeightForWidth());
        Widget->setSizePolicy(sizePolicy);
        gridLayout_3 = new QGridLayout(Widget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        groupBox = new QGroupBox(Widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        lbl_GrayImg = new QLabel(groupBox);
        lbl_GrayImg->setObjectName(QStringLiteral("lbl_GrayImg"));

        gridLayout->addWidget(lbl_GrayImg, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(Widget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Ignored);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy2);
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        lbl_RGBImg = new QLabel(groupBox_2);
        lbl_RGBImg->setObjectName(QStringLiteral("lbl_RGBImg"));

        gridLayout_2->addWidget(lbl_RGBImg, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 0, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));

        gridLayout_3->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btn_LoadPic = new QPushButton(Widget);
        btn_LoadPic->setObjectName(QStringLiteral("btn_LoadPic"));

        horizontalLayout->addWidget(btn_LoadPic);

        label_3 = new QLabel(Widget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout->addWidget(label_3);

        edt_ImgFolderPath = new QLineEdit(Widget);
        edt_ImgFolderPath->setObjectName(QStringLiteral("edt_ImgFolderPath"));

        horizontalLayout->addWidget(edt_ImgFolderPath);

        btn_OpenFile = new QPushButton(Widget);
        btn_OpenFile->setObjectName(QStringLiteral("btn_OpenFile"));

        horizontalLayout->addWidget(btn_OpenFile);

        btn_NextPic = new QPushButton(Widget);
        btn_NextPic->setObjectName(QStringLiteral("btn_NextPic"));

        horizontalLayout->addWidget(btn_NextPic);

        label = new QLabel(Widget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        edt_ImgSavePath = new QLineEdit(Widget);
        edt_ImgSavePath->setObjectName(QStringLiteral("edt_ImgSavePath"));

        horizontalLayout->addWidget(edt_ImgSavePath);

        btn_SaveImgPath = new QPushButton(Widget);
        btn_SaveImgPath->setObjectName(QStringLiteral("btn_SaveImgPath"));

        horizontalLayout->addWidget(btn_SaveImgPath);

        btn_trans = new QPushButton(Widget);
        btn_trans->setObjectName(QStringLiteral("btn_trans"));

        horizontalLayout->addWidget(btn_trans);

        btn_BatchTrans = new QPushButton(Widget);
        btn_BatchTrans->setObjectName(QStringLiteral("btn_BatchTrans"));

        horizontalLayout->addWidget(btn_BatchTrans);

        btn_close = new QPushButton(Widget);
        btn_close->setObjectName(QStringLiteral("btn_close"));

        horizontalLayout->addWidget(btn_close);


        gridLayout_3->addLayout(horizontalLayout, 2, 0, 1, 2);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("Widget", "\345\215\225\351\200\232\351\201\223", Q_NULLPTR));
        lbl_GrayImg->setText(QString());
        groupBox_2->setTitle(QApplication::translate("Widget", "\344\270\211\351\200\232\351\201\223", Q_NULLPTR));
        lbl_RGBImg->setText(QString());
        btn_LoadPic->setText(QApplication::translate("Widget", "\345\212\240\350\275\275\345\233\276\345\203\217", Q_NULLPTR));
        label_3->setText(QApplication::translate("Widget", "\346\226\207\344\273\266\345\244\271\350\267\257\345\276\204\357\274\232", Q_NULLPTR));
        btn_OpenFile->setText(QApplication::translate("Widget", "\346\211\271\351\207\217\345\212\240\350\275\275", Q_NULLPTR));
        btn_NextPic->setText(QApplication::translate("Widget", "\344\270\213\344\270\200\345\274\240", Q_NULLPTR));
        label->setText(QApplication::translate("Widget", "\344\277\235\345\255\230\350\267\257\345\276\204\357\274\232", Q_NULLPTR));
        btn_SaveImgPath->setText(QApplication::translate("Widget", "\344\277\256\346\224\271\350\267\257\345\276\204", Q_NULLPTR));
        btn_trans->setText(QApplication::translate("Widget", "\350\275\254\346\215\242", Q_NULLPTR));
        btn_BatchTrans->setText(QApplication::translate("Widget", "\346\211\271\351\207\217\350\275\254\346\215\242", Q_NULLPTR));
        btn_close->setText(QApplication::translate("Widget", "\351\200\200\345\207\272", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
