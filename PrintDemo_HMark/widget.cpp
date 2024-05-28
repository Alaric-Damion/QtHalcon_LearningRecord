#include "widget.h"
#include "ui_widget.h"
#include "Tsplsdk.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    init_printer();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init_printer()
{
    QString DLLpath = "./TSPL_SDK.dll";

    m_hInstancePrint =LoadLibrary(DLLpath.toStdWString().c_str());
    if(m_hInstancePrint!=NULL)
    {
        qDebug()<<"dll load success";
        PrinterCreator = (int (*)(void** handle, const char* model))GetProcAddress(m_hInstancePrint, "PrinterCreator");
        PortClose = (int (*)(void* handle))GetProcAddress(m_hInstancePrint, "PortClose");
        PortOpen = (int (*)(void* handle, const char* ioSettings))GetProcAddress(m_hInstancePrint, "PortOpen");
        PrinterDestroy = (int (*)(void* handle))GetProcAddress(m_hInstancePrint, "PrinterDestroy");
        TSPL_Setup = (int (*)(void* handle,
                              int labelWidth,
                              int labelHeight,
                              int speed,
                              int density,
                              int type,
                              int gap,
                              int offset))GetProcAddress(m_hInstancePrint, "TSPL_Setup");
        TSPL_ImagePrint = (int (*)(void* handle,
                                   const TCHAR* chImagePath,
                                   int iXPos,
                                   int iYPos,
                                   int iPrintImageMode,
                                   int iBrightness,
                                   int iContrast,
                                   int iHtMode,
                                   int iTagNum,
                                   int iCopies))GetProcAddress(m_hInstancePrint, "TSPL_ImagePrint");
        TSPL_TextPrint = (int (*)(void* handle,
                                  const TCHAR* chText,
                                  int iTextLenth,
                                  int iXPos,
                                  int iYPos,
                                  int iPrintImageMode,
                                  int iBrightness,
                                  int iContrast,
                                  int iHtMode,
                                  int iTagNum,
                                  int iCopies,
                                  int iXImage,
                                  int iYImage,
                                  int iFontSize))GetProcAddress(m_hInstancePrint, "TSPL_TextPrint");
        TSPL_QrCodePrint = (int (*)(void* handle,
                                    const TCHAR* chText,
                                    int iTextLenth,
                                    int iXPos,
                                    int iYPos,
                                    int iPrintImageMode,
                                    int iBrightness,
                                    int iContrast,
                                    int iHtMode,
                                    int iTagNum,
                                    int iCopies))GetProcAddress(m_hInstancePrint, "TSPL_QrCodePrint");
        TSPL_Text = (int (*)(void* handle,
                             int xPos,
                             int yPos,
                             int font,
                             int rotation,
                             int xMultiplication,
                             int yMultiplication ,
                             int alignment,
                             const char* data))GetProcAddress(m_hInstancePrint, "TSPL_Text");
    }
    else
    {
        qWarning()<<"调用库加载失败";
    }

    int ret;
    QString strName = "HT100";
    QByteArray string;
    string = strName.toLocal8Bit();
    char *c_strName = string.data();
//    const char *c_strName = "HT300";
    ret = PrinterCreator(&hprinter, c_strName);
    qDebug()<<"ret0"<<ret<<endl;

    QString strPort = "USB";
    string = strPort.toLocal8Bit();
    char *c_strPort = string.data();
    ret = PortOpen(hprinter, c_strPort);
    qDebug()<<"ret1"<<ret<<endl;

    ret =TSPL_Setup(hprinter, 50, 40, 2, 6, 1, 0, 0);
    qDebug()<<"ret2"<<ret<<endl;

    QString ProfileClientStr = "12345678";
    string = ProfileClientStr.toLocal8Bit();
    char *c_ProfileClientStr = string.data();
//    ret = TSPL_QrCodePrint(hprinter,(TCHAR*)c_ProfileClientStr, ProfileClientStr.length(),10,10,1,0,0,0,1,1);
//    ret = TSPL_TextPrint(hprinter,(TCHAR*)c_ProfileClientStr, ProfileClientStr.length(),10,10,2,0,0,0,1,1,60,40,25);
    ret = TSPL_Text();

}

