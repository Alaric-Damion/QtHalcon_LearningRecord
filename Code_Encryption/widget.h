#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFile>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>
#include <QSettings>

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

    QString getWMIC(const QString &cmd);
    QString getCpuId();
    QString getDiskNum();
    //加密
    QString Encode(QString row);
    //解密
    QString Decode(QString passwd);
    //判断授权信息  0:使用ini方法判断；1：使用注册表方法判断
    void Judge_Authorize_times(int type);
    //从ini获取授权信息
    QString getInfoFromIni(QString str);
    //写入授权信息到ini
    void WriteInfo2Ini(QString str, QString info_text);
    //从注册表获取授权信息
    QString getInfoFromRegistry();
    //写入授权信息到注册表
    void WriteInfo2Registry(QString str);
    //授权程序使用次数为2次
    QString time = "2";

};

#endif // WIDGET_H
