#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QWidget>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
private slots:
        void keyPressEvent(QKeyEvent* event);
};

#endif // DIALOG_H
