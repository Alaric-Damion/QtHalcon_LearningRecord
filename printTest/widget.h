#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <Windows.h>
#include <QMutex>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

     void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    HINSTANCE m_hInstancePrint;

private slots:
    void on_btn_initPrint_clicked();

    void on_btn_print_clicked();

    void on_btn_devStatus_clicked();

    void on_btn_PrintStatus_clicked();

    void on_btn_clearLog_clicked();

    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    void init_printDLL();

};

#endif // WIDGET_H
