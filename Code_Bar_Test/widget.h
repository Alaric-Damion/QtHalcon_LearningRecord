#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "dialog.h"
#include "form.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    Dialog *p_dialog;
    Form *p_form;

private slots:

    void on_pushButton_Load_clicked();
    void on_pushButton_test_clicked();
    void keyPressEvent(QKeyEvent* event);
};

#endif // WIDGET_H
