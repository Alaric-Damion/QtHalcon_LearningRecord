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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QHBoxLayout *horizontalLayout_6;
    QLabel *lbl_PicShow;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *btnOpenCamera;
    QPushButton *btnGrabImage;
    QPushButton *btnStopGrab;
    QPushButton *btnSnapImage;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *ckb_ModifyFps;
    QLabel *label;
    QLineEdit *txt_FrameRate;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QSlider *hSliderExTime;
    QSpinBox *spinBox_ExTime;
    QPushButton *btnAutoExTime;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout;
    QSlider *hSlider_Gain;
    QSpinBox *spinBox_Gain;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(793, 507);
        horizontalLayout_6 = new QHBoxLayout(Widget);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        lbl_PicShow = new QLabel(Widget);
        lbl_PicShow->setObjectName(QStringLiteral("lbl_PicShow"));

        horizontalLayout_6->addWidget(lbl_PicShow);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        btnOpenCamera = new QPushButton(Widget);
        btnOpenCamera->setObjectName(QStringLiteral("btnOpenCamera"));

        horizontalLayout_5->addWidget(btnOpenCamera);

        btnGrabImage = new QPushButton(Widget);
        btnGrabImage->setObjectName(QStringLiteral("btnGrabImage"));

        horizontalLayout_5->addWidget(btnGrabImage);

        btnStopGrab = new QPushButton(Widget);
        btnStopGrab->setObjectName(QStringLiteral("btnStopGrab"));

        horizontalLayout_5->addWidget(btnStopGrab);

        btnSnapImage = new QPushButton(Widget);
        btnSnapImage->setObjectName(QStringLiteral("btnSnapImage"));

        horizontalLayout_5->addWidget(btnSnapImage);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        label_2 = new QLabel(Widget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_4->addWidget(label_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_4);

        groupBox = new QGroupBox(Widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout_3 = new QHBoxLayout(groupBox);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        ckb_ModifyFps = new QCheckBox(groupBox);
        ckb_ModifyFps->setObjectName(QStringLiteral("ckb_ModifyFps"));

        horizontalLayout_3->addWidget(ckb_ModifyFps);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        txt_FrameRate = new QLineEdit(groupBox);
        txt_FrameRate->setObjectName(QStringLiteral("txt_FrameRate"));

        horizontalLayout_3->addWidget(txt_FrameRate);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(Widget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        hSliderExTime = new QSlider(groupBox_2);
        hSliderExTime->setObjectName(QStringLiteral("hSliderExTime"));
        hSliderExTime->setMinimum(10);
        hSliderExTime->setMaximum(840000);
        hSliderExTime->setSingleStep(10);
        hSliderExTime->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(hSliderExTime);

        spinBox_ExTime = new QSpinBox(groupBox_2);
        spinBox_ExTime->setObjectName(QStringLiteral("spinBox_ExTime"));
        spinBox_ExTime->setMinimum(10);
        spinBox_ExTime->setMaximum(840000);
        spinBox_ExTime->setSingleStep(10);

        horizontalLayout_2->addWidget(spinBox_ExTime);

        btnAutoExTime = new QPushButton(groupBox_2);
        btnAutoExTime->setObjectName(QStringLiteral("btnAutoExTime"));

        horizontalLayout_2->addWidget(btnAutoExTime);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(Widget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        horizontalLayout = new QHBoxLayout(groupBox_3);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        hSlider_Gain = new QSlider(groupBox_3);
        hSlider_Gain->setObjectName(QStringLiteral("hSlider_Gain"));
        hSlider_Gain->setMinimum(1);
        hSlider_Gain->setMaximum(3);
        hSlider_Gain->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(hSlider_Gain);

        spinBox_Gain = new QSpinBox(groupBox_3);
        spinBox_Gain->setObjectName(QStringLiteral("spinBox_Gain"));
        spinBox_Gain->setMinimum(1);
        spinBox_Gain->setMaximum(3);

        horizontalLayout->addWidget(spinBox_Gain);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(groupBox_3);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 2);
        verticalLayout->setStretch(2, 2);
        verticalLayout->setStretch(3, 2);
        verticalLayout->setStretch(4, 2);

        horizontalLayout_6->addLayout(verticalLayout);

        horizontalLayout_6->setStretch(0, 5);
        horizontalLayout_6->setStretch(1, 1);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        lbl_PicShow->setText(QApplication::translate("Widget", "TextLabel", Q_NULLPTR));
        btnOpenCamera->setText(QApplication::translate("Widget", "\346\211\223\345\274\200\347\233\270\346\234\272", Q_NULLPTR));
        btnGrabImage->setText(QApplication::translate("Widget", "\350\277\236\346\213\215", Q_NULLPTR));
        btnStopGrab->setText(QApplication::translate("Widget", "\345\201\234\346\255\242", Q_NULLPTR));
        btnSnapImage->setText(QApplication::translate("Widget", "\346\212\223\345\233\276", Q_NULLPTR));
        label_2->setText(QApplication::translate("Widget", "\347\233\270\346\234\272\350\256\276\345\256\232", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("Widget", "\345\270\247\347\216\207\350\256\276\347\275\256", Q_NULLPTR));
        ckb_ModifyFps->setText(QApplication::translate("Widget", "\346\233\264\346\224\271\345\270\247\347\216\207", Q_NULLPTR));
        label->setText(QApplication::translate("Widget", "\351\207\207\351\233\206\345\270\247\347\216\207\357\274\232", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("Widget", "\346\233\235\345\205\211\350\256\276\347\275\256", Q_NULLPTR));
        btnAutoExTime->setText(QApplication::translate("Widget", "AE", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("Widget", "\345\242\236\347\233\212\350\256\276\347\275\256", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
