/********************************************************************************
** Form generated from reading UI file 'browsepic.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BROWSEPIC_H
#define UI_BROWSEPIC_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <cmylabel.h>

QT_BEGIN_NAMESPACE

class Ui_BrowsePic
{
public:
    QGridLayout *gridLayout;
    CMyLabel *label_show;
    QPushButton *btn_prePic;
    QPushButton *btn_openPic;
    QPushButton *btn_nextPic;
    QCheckBox *checkBox_equalScale;
    QCheckBox *checkBox_showCenter;
    QLabel *label_status;
    QPushButton *btn_resetPic;

    void setupUi(QWidget *BrowsePic)
    {
        if (BrowsePic->objectName().isEmpty())
            BrowsePic->setObjectName(QStringLiteral("BrowsePic"));
        BrowsePic->resize(647, 530);
        gridLayout = new QGridLayout(BrowsePic);
        gridLayout->setSpacing(2);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(2, 2, 2, 2);
        label_show = new CMyLabel(BrowsePic);
        label_show->setObjectName(QStringLiteral("label_show"));

        gridLayout->addWidget(label_show, 0, 0, 1, 6);

        btn_prePic = new QPushButton(BrowsePic);
        btn_prePic->setObjectName(QStringLiteral("btn_prePic"));

        gridLayout->addWidget(btn_prePic, 1, 0, 1, 1);

        btn_openPic = new QPushButton(BrowsePic);
        btn_openPic->setObjectName(QStringLiteral("btn_openPic"));

        gridLayout->addWidget(btn_openPic, 1, 1, 1, 1);

        btn_nextPic = new QPushButton(BrowsePic);
        btn_nextPic->setObjectName(QStringLiteral("btn_nextPic"));

        gridLayout->addWidget(btn_nextPic, 1, 2, 1, 1);

        checkBox_equalScale = new QCheckBox(BrowsePic);
        checkBox_equalScale->setObjectName(QStringLiteral("checkBox_equalScale"));

        gridLayout->addWidget(checkBox_equalScale, 1, 4, 1, 1);

        checkBox_showCenter = new QCheckBox(BrowsePic);
        checkBox_showCenter->setObjectName(QStringLiteral("checkBox_showCenter"));

        gridLayout->addWidget(checkBox_showCenter, 1, 5, 1, 1);

        label_status = new QLabel(BrowsePic);
        label_status->setObjectName(QStringLiteral("label_status"));

        gridLayout->addWidget(label_status, 2, 0, 1, 1);

        btn_resetPic = new QPushButton(BrowsePic);
        btn_resetPic->setObjectName(QStringLiteral("btn_resetPic"));

        gridLayout->addWidget(btn_resetPic, 1, 3, 1, 1);

        gridLayout->setRowStretch(0, 9);
        gridLayout->setRowStretch(1, 1);
        gridLayout->setRowStretch(2, 1);
        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 1);
        gridLayout->setColumnStretch(2, 1);
        gridLayout->setColumnStretch(3, 1);
        gridLayout->setColumnStretch(4, 1);
        gridLayout->setColumnStretch(5, 1);

        retranslateUi(BrowsePic);

        QMetaObject::connectSlotsByName(BrowsePic);
    } // setupUi

    void retranslateUi(QWidget *BrowsePic)
    {
        BrowsePic->setWindowTitle(QApplication::translate("BrowsePic", "BrowsePic", Q_NULLPTR));
        label_show->setText(QApplication::translate("BrowsePic", "TextLabel", Q_NULLPTR));
        btn_prePic->setText(QApplication::translate("BrowsePic", "<", Q_NULLPTR));
        btn_openPic->setText(QApplication::translate("BrowsePic", "\346\211\223\345\274\200\345\233\276\345\203\217", Q_NULLPTR));
        btn_nextPic->setText(QApplication::translate("BrowsePic", ">", Q_NULLPTR));
        checkBox_equalScale->setText(QApplication::translate("BrowsePic", "\347\255\211\346\257\224\347\274\251\346\224\276", Q_NULLPTR));
        checkBox_showCenter->setText(QApplication::translate("BrowsePic", "\345\261\205\344\270\255\346\230\276\347\244\272", Q_NULLPTR));
        label_status->setText(QApplication::translate("BrowsePic", "TextLabel", Q_NULLPTR));
        btn_resetPic->setText(QApplication::translate("BrowsePic", "\346\201\242\345\244\215", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class BrowsePic: public Ui_BrowsePic {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BROWSEPIC_H
