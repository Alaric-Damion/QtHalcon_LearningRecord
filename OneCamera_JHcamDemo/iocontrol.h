#ifndef IOCONTROL_H
#define IOCONTROL_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class IOControl;
}

class IOControl : public QDialog
{
    Q_OBJECT
    
public:
    explicit IOControl(QWidget *parent = 0);
    ~IOControl();

    void InitControlParameter();
    
private slots:
    void on_btnRefresh_clicked();
    void on_btnTrigger_clicked();
    void on_cbOut_clicked(bool checked);
    void on_cbTriggerMode_clicked(bool checked);
    void on_cbStrobePolarity_clicked(bool checked);
    void on_cbTriggerPolarity_clicked(bool checked);
    void on_cbEnableStrobe_clicked(bool checked);

private:
    Ui::IOControl *ui;

    QSettings *settings;
};

#endif // IOCONTROL_H
