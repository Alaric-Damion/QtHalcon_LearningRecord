#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTranslator>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void setTranslator(QTranslator *translator);
    void setTranSet();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

protected:
    void changeEvent(QEvent *event);
private:
    Ui::Widget *ui;
    QTranslator *m_translator;
    int tranIndex = 0;
};
#endif // WIDGET_H
