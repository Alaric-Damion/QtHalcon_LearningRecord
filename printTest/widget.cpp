#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"
#include "../printTest/printDll/include/Msprintsdk.h"
//#include "../printDll/include/Msprintsdk.h"
#include <QTextCodec>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    init_printDLL();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btn_initPrint_clicked()
{
    QString txt_Com = ui->edt_Port->text();
    char* ch;
    QByteArray ba = txt_Com.toLatin1(); // must
    ch=ba.data();
//    char text_port[40] = "USB001";
    QString txt_rate = ui->edt_Rate->text();
    int rate = txt_rate.toInt();
    qDebug()<<"rate"<<ch<<rate;
    SetPrintport(ch,rate);
    //    SetPrintportFlowCtrl(1);

    int m_iInit = SetInit();

    if(m_iInit ==0)
    {
        qDebug()<<"打印机初始化成功";
    }
    else
    {
        qWarning()<<"打印机初始化失败";
    }
    int ret = GetStatus();
    qDebug()<<"ret"<<ret;
}

void Widget::on_btn_print_clicked()
{    
    qDebug()<<"打印开始";
    SetBold(1);
    QByteArray string;
    //第一行预留10dot（1dot=0.125mm）
    PrintFeedDot(10);
    PrintQrcode("1123469-033",3,6,1);
    SetLeftmargin(160);

    QString ProductIDStr = "单号：1123469-033";
    string = ProductIDStr.toLocal8Bit();
    char *c_ProductIDStr = string.data();
    qDebug()<<"c_ProductIDStr"<<c_ProductIDStr;
    PrintString(c_ProductIDStr,0);
    QString length = "长度：2555";
    string = length.toLocal8Bit();
    char *c_length = string.data();
    PrintString(c_length,0);
    QString PurposeStr = "上固定框";
    string = PurposeStr.toLocal8Bit();
    char *c_PurposeStr = string.data();
    PrintString(c_PurposeStr,0);
    QString ProfileNOStr = "型材编号：HNPC109-01";
    string = ProfileNOStr.toLocal8Bit();
    char *c_ProfileNOStr = string.data();
    PrintString(c_ProfileNOStr,0);
    QString ProfileNameStr = "型材名称：109外框";
    string = ProfileNameStr.toLocal8Bit();
    char *c_ProfileNameStr = string.data();
    PrintString(c_ProfileNameStr,0);
    int r = PrintRemainQR();
    //清空设置
    SetClean();
    //设置距离左边距20dot
    SetLeftmargin(20);
    QString ProfileColorStr = "A法式香槟色B氟碳灰色";
    string = ProfileColorStr.toLocal8Bit();
    char *c_ProfileColorStr = string.data();
    PrintString(c_ProfileColorStr,0);
    QString ProfileClientStr = "海南三亚旗舰店-陆涛";
    string = ProfileClientStr.toLocal8Bit();
    char *c_ProfileClientStr = string.data();
    PrintString(c_ProfileClientStr,0);
    //    PrintString("静雅系列断桥-平框-系列窗纱一体平开窗（外开玻璃+内开纱窗）",0);

    //进纸到切纸位置
    PrintMarkpositioncut();
    qDebug()<<"打印完成";
}


void Widget::init_printDLL()
{
    ui->textEdit->append("单号：1123469-033");
    ui->textEdit->append("长度：2555");
    ui->textEdit->append("上固定框");
    ui->textEdit->append("型材编号：HNPC109-01");
    ui->textEdit->append("型材名称：109外框");
    ui->textEdit->append("A法式香槟色B氟碳灰色");
    ui->textEdit->append("海南三亚旗舰店-陆涛");

    QString DLLpath = "./printDll/lib/Msprintsdk.x64.dll";

    m_hInstancePrint =LoadLibrary(DLLpath.toStdWString().c_str());
    if(m_hInstancePrint!=NULL)
    {
        qDebug()<<"dll load success";
        SetPrintport =(int (*)(const char* strPort,int iBaudrate))GetProcAddress(m_hInstancePrint, "SetPrintport");
        SetPrintportFlowCtrl = (int(*)(int iFlowCtrlFlag))GetProcAddress(m_hInstancePrint, "SetPrintportFlowCtrl");
        SetInit = (int(*)(void))GetProcAddress(m_hInstancePrint, "SetInit");
        SetLeftmargin = (int(*)(int iLeftspace))GetProcAddress(m_hInstancePrint, "SetLeftmargin");
        PrintQrcode =   (int (*)(const char* strData,int iLmargin,int iMside,int iRound))GetProcAddress(m_hInstancePrint, "PrintQrcode");
        PrintString = (int(*)(const char* strData, int iImme))GetProcAddress(m_hInstancePrint, "PrintString");
        PrintRemainQR = (int(*)(void))GetProcAddress(m_hInstancePrint, "PrintRemainQR");
        SetSizetext =   (int (*)(int iHeight,int iWidth))GetProcAddress(m_hInstancePrint, "SetSizetext");
        PrintMarkpositioncut = (int(*)(void))GetProcAddress(m_hInstancePrint, "PrintMarkpositioncut");
        SetClean = (int(*)(void))GetProcAddress(m_hInstancePrint, "SetClean");
        SetAlignment =   (int (*)(int iAlignment))GetProcAddress(m_hInstancePrint, "SetAlignment");
        PrintFeedDot =   (int (*)(int Lnumber))GetProcAddress(m_hInstancePrint, "PrintFeedDot");
        PrintFeedline =   (int (*)(int iLine))GetProcAddress(m_hInstancePrint, "PrintFeedline");
        PrintChangeRow = (int(*)(void))GetProcAddress(m_hInstancePrint, "PrintChangeRow");
        GetStatus = (int(*)(void))GetProcAddress(m_hInstancePrint, "GetStatus");
        SetReadZKmode =   (int (*)(int mode))GetProcAddress(m_hInstancePrint, "SetReadZKmode");
        SetSizechinese =   (int (*)(int iHeight,int iWidth,int iUnderline,int iChinesetype))GetProcAddress(m_hInstancePrint, "SetSizechinese");
        SetSizechar =   (int (*)(int iHeight,int iWidth,int iUnderline,int iAsciitype))GetProcAddress(m_hInstancePrint, "SetSizechar");
        SetBold =   (int (*)(int iBold))GetProcAddress(m_hInstancePrint, "SetBold");
        GetStatus = (int(*)(void))GetProcAddress(m_hInstancePrint, "GetStatus");
        GetStatusspecial = (int(*)(void))GetProcAddress(m_hInstancePrint, "GetStatusspecial");
    }
    else
    {
        qWarning()<<"调用库加载失败";
    }
}

void Widget::on_btn_devStatus_clicked()
{
    int ret = GetStatus();
    QString devStatus = "设备状态码：" + QString::number(ret);
    qDebug()<<devStatus;
    ui->edt_StatusStr->setText(devStatus);
    switch (ret) {
    case 0:
        ui->edt_StatusInfo->setText("打印机正常");
        qDebug()<<"打印机正常";
        break;
    case 1:
        ui->edt_StatusInfo->setText("打印机未连接或未上电");
        qWarning()<<"打印机未连接或未上电";
        break;
    case 2:
        ui->edt_StatusInfo->setText("打印机与调用库不匹配");
        qWarning()<<"打印机与调用库不匹配";
        break;
    case 3:
        ui->edt_StatusInfo->setText("打印头打开");
        qWarning()<<"打印头打开";
        break;
    case 4:
        ui->edt_StatusInfo->setText("切刀未复位");
        qWarning()<<"切刀未复位";
        break;
    case 5:
        ui->edt_StatusInfo->setText("打印头温度异常");
        qWarning()<<"打印头温度异常";
        break;
    case 6:
        ui->edt_StatusInfo->setText("黑标错误");
        qWarning()<<"黑标错误：传感器故障/黑标不标准";
        break;
    case 7:
        ui->edt_StatusInfo->setText("纸尽");
        qWarning()<<"纸尽";
        break;
    case 8:
        ui->edt_StatusInfo->setText("纸将尽");
        qWarning()<<"纸将尽";
        break;
    default:
        break;
    }
}

void Widget::on_btn_PrintStatus_clicked()
{
    int ret = GetStatusspecial();
    QString PrintStatus = "打印状态码：" + QString::number(ret);
    qDebug()<<PrintStatus;
    ui->edt_StatusStr->setText(PrintStatus);
    switch (ret) {
    case 0:
        ui->edt_StatusInfo->setText("打印机正常");
        qDebug()<<"打印机正常";
        break;
    case 1:
        ui->edt_StatusInfo->setText("打印机未连接或未上电");
        qWarning()<<"打印机未连接或未上电";
        break;
    case 2:
        ui->edt_StatusInfo->setText("打印机与调用库不匹配");
        qWarning()<<"打印机与调用库不匹配";
        break;
    case 3:
        ui->edt_StatusInfo->setText("当前打印机无特殊功能");
        qDebug()<<"当前打印机无特殊功能";
        break;
    case 4:
        ui->edt_StatusInfo->setText("容纸器没有可靠上纸");
        qWarning()<<"容纸器没有可靠上纸";
        break;
    case 5:
        ui->edt_StatusInfo->setText("纸张堵在出口，堆叠");
        qWarning()<<"纸张堵在出口，堆叠";
        break;
    case 6:
        ui->edt_StatusInfo->setText("卡纸");
        qWarning()<<"卡纸";
        break;
    case 7:
        ui->edt_StatusInfo->setText("拽纸");
        qWarning()<<"拽纸";
        break;
    case 8:
        ui->edt_StatusInfo->setText("出纸传感器有纸");
        qWarning()<<"出纸传感器有纸";
        break;
    default:
        break;
    }
}


void Widget::outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        ui->textEdit_log->setTextColor(Qt::blue);
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        ui->textEdit_log->setTextColor(Qt::red);
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        break;
    case QtFatalMsg:
        text = QString("Fatal:");
    }

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    //        QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);

    //保存log信息到本地
    QString message = QString("%1 %2 %3").arg(text).arg(msg).arg(current_date);
    //        QFile file("log.txt");
    //        file.open(QIODevice::WriteOnly | QIODevice::Append);
    //        QTextStream text_stream(&file);
    //        text_stream << message << "\r\n";

    ui->textEdit_log->append(message);
    //    ui->textEdit_log->append("/n");
    //        file.flush();
    //        file.close();
    mutex.unlock();


}

void Widget::on_btn_clearLog_clicked()
{
    ui->textEdit_log->clear();
}

void Widget::on_pushButton_clicked()
{
    QMessageBox mm(QMessageBox::Question,QString(tr("打印机故障，请排查！")),QString(tr("打印机故障")),QMessageBox::Yes| QMessageBox::No | QMessageBox::Cancel,this);
    mm.setButtonText(QMessageBox::Yes,QString(tr("已排除故障")));  //方式1
    mm.button(QMessageBox::No)->setText(tr("刷新状态"));    //方式2
    mm.button(QMessageBox::Cancel)->setText(tr("关闭打印机"));
    QPixmap printMap("./Printer.bmp");
    mm.setIconPixmap(printMap);
    int flag = 0;
    int ret=1;
    while(!flag)
    {
        if(ret == 4) ret = 0;
        int Ret = mm.exec();
        if(Ret == QMessageBox::Yes) //通过判断返回值确定用户点击了愿意还是取消。
        {
            qDebug()<<"打印一张新标签";
            flag = 1;
        }
        else if(Ret == QMessageBox::No)
        {
            int m_iInit = SetInit();
//            int ret = GetStatus();

            if(ret == 0)
            {
                mm.button(QMessageBox::Yes)->setStyleSheet("background: rgb(0,255,0)");
                mm.setText("打印机状态：正常");
                mm.show();
                qDebug()<<"打印机状态：正常";
            }
            else
            {
                QString errorInfo = "打印机故障，故障代码："+QString::number(ret);
                mm.setText(errorInfo);
                ret++;
            }


        }
        else{
            flag = 1;
        }

    }

}
