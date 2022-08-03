#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QString str = getCpuId();
    qDebug()<<"cpu id is"<<str;
//    QString times_str = "RemainTime:"+time;
//    WriteInfo2Registry(times_str);
    Judge_Authorize_times(0);
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::getWMIC(const QString &cmd)
{
    //获取cpu名称：wmic cpu get Name
    //获取cpu核心数：wmic cpu get NumberOfCores
    //获取cpu线程数：wmic cpu get NumberOfLogicalProcessors
    //查询cpu序列号：wmic cpu get processorid
    //查询主板序列号：wmic baseboard get serialnumber
    //查询BIOS序列号：wmic bios get serialnumber
    //查看硬盘：wmic diskdrive get serialnumber
    QProcess p;
    p.start(cmd);
    p.waitForFinished();
    QString result = QString::fromLocal8Bit(p.readAllStandardOutput());
    QStringList list = cmd.split(" ");
    result = result.remove(list.last(), Qt::CaseInsensitive);
    result = result.replace("\r", "");
    result = result.replace("\n", "");
    result = result.simplified();
    return result;
}

QString Widget::getCpuId() //获取CPU序列号
{
    return getWMIC("wmic cpu get processorid");
}

QString Widget::getDiskNum() //获取硬盘序列号
{
    return getWMIC("wmic diskdrive where index=0 get serialnumber");
}

QString Widget::Encode(QString row)
{
    QByteArray byteArray = row.toUtf8();
    byteArray = byteArray.toBase64();
    return  byteArray;
}
QString Widget::Decode(QString passwd)
{
    QByteArray byteArray = passwd.toUtf8();
    byteArray = QByteArray::fromBase64(byteArray);
    return byteArray;
}

QString Widget::getInfoFromIni(QString str)
{
    QString info;
    QFile file(str);
    if(!file.exists()) { //如果文件不存在
        QString times_str = "RemainTime:"+time;
        WriteInfo2Ini(str, times_str);
    }
    file.open(QFile::ReadWrite | QFile::Text);
    info = file.readAll(); //读取信息
    file.close();
    return info;
}

void Widget::WriteInfo2Ini(QString str, QString info_text)
{
    QFile file(str);
    file.open(QFile::ReadWrite | QFile::Text | QFile::Truncate);
    file.write(Encode(info_text).toUtf8()); //写入信息
    file.close();
}

void Widget::WriteInfo2Registry(QString str)
{
    //写入注册表
    QSettings settings("HKEY_CURRENT_USER\\Software\\Code_Encryption\\Settings",QSettings::NativeFormat);
    settings.setValue("remain_times",Encode(str).toUtf8());
}

QString Widget::getInfoFromRegistry()
{
    QString info;
    //通过写入注册表来判断
    QSettings settings("HKEY_CURRENT_USER\\Software\\Code_Encryption\\Settings",QSettings::NativeFormat);
    info = settings.value("remain_times").toString();
    return info;
}

void Widget::Judge_Authorize_times(int type)
{
    QString times_info;
    if(type == 0)
    {
        //通过ini配置文件进行判断
        times_info = getInfoFromIni("System.ini");
    }
    else
    {
        //通过写入注册表来判断
        times_info = getInfoFromRegistry();
    }

    if(times_info.isEmpty())
    {
        QMessageBox::about(this,"提示","授权信息为空，请检查！");
        exit(0);
    }
    else
    {
        QString info = Decode(times_info); //解码
        if(info.contains(':')) {	//信息正确
            QStringList list = info.split(':');
            if(list.length()>=1) {
                if(list[1].toInt() <= 0) {	//程序剩余使用次数不足
                    QMessageBox::about(this,"提示","请注册后再使用！");
                    exit(0); //程序退出
                }else {	//程序还有剩余使用次数
                    QString time_remain = QString::number(list[1].toInt()-1); //程序剩余使用次数减1
                    QString str = "RemainTime:"+time_remain;
                    if(type == 0) WriteInfo2Ini("System.ini", str);
                    else {
                        //写入注册表
                        WriteInfo2Registry(str);
                    }

                    QMessageBox::about(this,"提示","程序剩余使用次数："+time_remain+"次");
                }
            }
        }

    }


}

