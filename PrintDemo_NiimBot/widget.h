#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void refreshPrinters();

private slots:
    void on_btn_findPrinter_clicked();

    void on_btn_Print_clicked();

    void getConnectStatus();

private:
    Ui::Widget *ui;
    bool isSDKInit = false;

    QMap<QString, int> printers;

};
#endif // WIDGET_H
