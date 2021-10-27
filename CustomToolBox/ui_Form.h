/********************************************************************************
** Form generated from reading UI file 'Form.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QFormLayout *formLayout;
    QLabel *label;
    QRadioButton *radioButton;
    QLabel *label_2;
    QCheckBox *checkBox;
    QLabel *label_3;
    QSlider *horizontalSlider;
    QLabel *label_4;
    QTimeEdit *timeEdit;
    QLabel *label_5;
    QLineEdit *lineEdit;
    QLabel *label_6;
    QLineEdit *lineEdit_2;
    QLabel *label_7;
    QComboBox *comboBox;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(243, 221);
        verticalLayout = new QVBoxLayout(Form);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton = new QPushButton(Form);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(Form);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label = new QLabel(Form);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        radioButton = new QRadioButton(Form);
        radioButton->setObjectName(QStringLiteral("radioButton"));

        formLayout->setWidget(0, QFormLayout::FieldRole, radioButton);

        label_2 = new QLabel(Form);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        checkBox = new QCheckBox(Form);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, checkBox);

        label_3 = new QLabel(Form);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        horizontalSlider = new QSlider(Form);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        formLayout->setWidget(2, QFormLayout::FieldRole, horizontalSlider);

        label_4 = new QLabel(Form);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        timeEdit = new QTimeEdit(Form);
        timeEdit->setObjectName(QStringLiteral("timeEdit"));

        formLayout->setWidget(3, QFormLayout::FieldRole, timeEdit);

        label_5 = new QLabel(Form);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_5);

        lineEdit = new QLineEdit(Form);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        formLayout->setWidget(4, QFormLayout::FieldRole, lineEdit);

        label_6 = new QLabel(Form);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_6);

        lineEdit_2 = new QLineEdit(Form);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        formLayout->setWidget(5, QFormLayout::FieldRole, lineEdit_2);

        label_7 = new QLabel(Form);
        label_7->setObjectName(QStringLiteral("label_7"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_7);

        comboBox = new QComboBox(Form);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        formLayout->setWidget(6, QFormLayout::FieldRole, comboBox);


        verticalLayout->addLayout(formLayout);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Form", "\344\277\235\345\255\230", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("Form", "\345\210\240\351\231\244", Q_NULLPTR));
        label->setText(QApplication::translate("Form", "TextLabel", Q_NULLPTR));
        radioButton->setText(QApplication::translate("Form", "RadioButton", Q_NULLPTR));
        label_2->setText(QApplication::translate("Form", "TextLabel", Q_NULLPTR));
        checkBox->setText(QApplication::translate("Form", "CheckBox", Q_NULLPTR));
        label_3->setText(QApplication::translate("Form", "TextLabel", Q_NULLPTR));
        label_4->setText(QApplication::translate("Form", "TextLabel", Q_NULLPTR));
        label_5->setText(QApplication::translate("Form", "TextLabel", Q_NULLPTR));
        lineEdit->setText(QApplication::translate("Form", "qt\345\260\217\347\275\227", Q_NULLPTR));
        label_6->setText(QApplication::translate("Form", "TextLabel", Q_NULLPTR));
        lineEdit_2->setText(QApplication::translate("Form", "qt\345\260\217\347\275\227", Q_NULLPTR));
        label_7->setText(QApplication::translate("Form", "TextLabel", Q_NULLPTR));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("Form", "\345\214\227\344\272\254", Q_NULLPTR)
         << QApplication::translate("Form", "\344\270\212\346\265\267", Q_NULLPTR)
         << QApplication::translate("Form", "\345\271\277\345\267\236", Q_NULLPTR)
         << QApplication::translate("Form", "\346\210\220\351\203\275", Q_NULLPTR)
        );
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
