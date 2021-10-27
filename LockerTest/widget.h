#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_ckbPic_clicked(bool checked);

    void on_ckbVideo_clicked(bool checked);

private:
    Ui::Widget *ui;

        void initUI();
        int m_PicList;
        int m_VideoList;
};

#endif // WIDGET_H
