#ifndef JCPRINTER_H
#define JCPRINTER_H

#include <iostream>

namespace Printer {
/**
    * Load Printer SDK
    */
bool loadPrinterApiDll();

/**
    * 初始化字体路径，可默认空
    * @param[in] font 字体文件夹
    * @return 初始化成功返回true
    */
bool initSdk(char* fontDir);

/**
     * 获取打印机错误码
     * @return 错误码
    */
int getLastPrinterError();

/********************************************************打印接口*********************************************************************/
/**
        * 查找打印机
        * 以json形式返回{打印机名称，端口}
        * Example： "{"S6_0011223344":"4"}"
        * @return 返回数据的指针 char *
        */
char* getAllPrinters();

/**
        * 连接打印机
        * @param[in] printerName 打印机名称（使用 `getAllPrinters `获取的打印机名称）
        * @param[in] port 打印机端口, `getAllPrinters`获取的打印机端口
        * @return 连接成功返回true
        */
bool selectPrinter(char* printerName, int port);

/**
        * 断开连接
        * @return 操作成功返回true
        */
bool disconnectPrinter();

/**
        * 设置打印信息
        * @param[in] printLabelType 纸张类型：
        *                           1-间隙纸，2-⿊标纸，3-连续纸，4-过孔纸，5-透明纸
        * @param[in] printDensity 浓度，浓度范围1-5
        * @param[in] printMode 打印类型：
        *                      1-热敏
        *                      2-热转印
        * @param[in] printCount 打印份数
        * @return 设置成功返回true
        */
bool startJob(int printLabelType, int printDensity, int printMode, int printCount);

/**
        * 提交打印任务
        * 使用绘制接口绘制打印数据需要调用此接口打印数据
        */
bool commitJob();

/**
        * 图片打印
        * @param printData 图片数据
        * @param dataLenght 图片数据长度
        * @param verticalShift 水平偏移
        * @param horizontalShift 垂直偏移
        * @param threshold 二值化临界点（默认使用127）
        * @return
        */
bool picturePrint(char* printData, size_t dataLenght, int verticalShift, int horizontalShift, int threshold);

/**
        * 结束打印
        */
bool endJob();

/*
    * 检测设备连接状态
    */
bool isConnected();

/**
     * 查找Wifi打印机
     * 以json形式返回{打印机名称，端口}
     * Example： "{"S6_0011223344":"4"}"
     * @return 返回数据的指针 char *
     */
char* scanWifiPrinter();

/**
     * 连接Wifi打印机
     * @param[in] printerName 打印机名称（使用 `scanWifiPrinter `获取的打印机名称）
     * @param[out] port 打印机端口, `scanWifiPrinter`获取的打印机端口(以ip地址为基础模拟端口号)
     * @return 连接成功返回true
     */
bool connectWifiPrinter(char* printerName, int port);

/**
    * 配置Wifi网络
    * @param wifiName wifi热点名称
    * @param wifiPassword wifi热点密码
    * @return true为成功，false为失败
    */
bool configurationWifi(char* wifiName, char* wifiPassword);

/********************************************************绘制接口***********************************************************************/
/*
        * 初始化画板
        * @param width 画板宽度，单位mm
        * @param height 画板高度，单位mm
        * @param rotate 旋转角度，仅支持0,90,180,270
        * @param font 字体,默认使用 "ZT001.ttf"
        * @param verticalShift 垂直偏移
        * @param horizontalShift 水平偏移
        * @return 初始化成功返回 true
        */
bool InitDrawingBoard(float width, float height, int rotate, const char* font, int verticalShift, int HorizontalShift);

/*
        * 绘制文本
        * @param x 水平坐标
        * @param y 垂直坐标
        * @param width 宽度， 单位mm
        * @param height 高度，单位mm
        * @param value 绘制的文本（UTF8）
        * @param fontFamily 字体，默认使用 "ZT001.ttf"
        * @param fontSize 字体大小
        * @param textAlignHorizonral 水平对齐方式，0:左对齐 1:居中对齐 2:右对齐
        * @param textAlignVertical 垂直对齐方式，0:顶对齐 1:垂直居中 2:底对齐
        * @param lineMode 行模式，默认2
        *                  1:宽高固定，内容大小自适应（字号/字符间距/行间距 按比例缩放）
        *                  2:宽度固定，高度自适应
        *                  3:宽高固定，超出内容后面加...
        *                  4:宽高固定,超出内容直裁切
        * @param letterSpacing 字间距，单位mm
        * @param lineSpacing 行间距，单位mm
        * @param fontStyle 字体风格（加粗、斜体、下划线），数据格式[斜体，加粗，下划线，保留]
        * @return
        */
bool DrawLableText(float x, float y, float width, float height, const char* value, const char* fontFamily, float fontSize, int rotate, int textAlignHorizonral, int textAlignVertical, int lineMode, float letterSpacing, float lineSpacing, bool fontStyle[4]);

/*
        * 绘制一维码
        * @param x 水平坐标
        * @param y 垂直坐标
        * @param width 宽，单位mm
        * @param height 高，单位mm
        * @param rotate: 旋转角度，仅支持0,90,180,270
        * @param value 文本内容（UTF8）
        * @param fontSize 字号
        * @param codeType 条码类型：
        *               20:CODE12821:UPC-A
        *               22:UPC-E
        *               23:EAN8
        *               24:EAN13
        *               25:CODE93
        *               26:CODE39
        *               27:CODEBAR
        *               28:ITF25
        * @param textHeight 文本高度
        * @param textPosition 文本位置（上、下、无）
        *                      0：下方
        *                      1：上方，
        *                      2：不显示
        */
bool DrawLableBarCode(float x, float y, float width, float height, int codeType, const char* value, float fontSize, int rotate, float textHeight, int textPosition);

/*
        * 绘制二维码
        * @param x 起始点左上角水平方向位置
        * @param y 起始点左上角垂直方向位置
        * @param width 宽
        * @param height 高
        * @param value 文本内容（UTF8）
        * @param codeType 条码类型
        *                  31:QR_CODE
        *                  32:PDF417
        *                  33:DATA_MATRIX
        *                  34:AZTEC
        * @param rotate: 旋转角度，0、90、180、270
        */
bool DrawLableQrCode(float  x, float y, float width, float height, const char* value, int codeType, int rotate);

/*
        * 绘制线条
        * @param x 起始点左上角水平方向位置
        * @param y 起始点左上角垂直方向位置
        * @param width 宽
        * @param height 高
        * @param rotate: 旋转角度，0、90、180、270
        * @param lineType 线类型（虚线、实线）
        * @param dashwidth 数据格式[实线段长度，空线段长度]
        */
bool DrawLableLine(float x, float y, float width, float height, int rotate, int lineType, float dashwidth[2]);

/*
        * 绘制图形
        * @param x 起始点左上角水平方向位置
        * @param y 起始点左上角垂直方向位置
        * @param width 宽
        * @param height 高
        * @param graphType 图形类型
        *                  1:圆
        *                  2:椭圆
        *                  3:矩形
        * @param rotate 旋转角度, 0、90、180、270
        * @param cornerRadius 圆角半径
        * @param lineWidth 线宽
        * @param lineType 线类型,
        *                  1:实线
        *                  2:虚线类型
        * @param dashwidth 数据格式[实线段长度，空线段长度]
        */
bool DrawLableGraph(float x, float y, float width, float height, int graphType, int rotate, float cornerRadius, float lineWidth, int lineType, float dashwidth[2]);

/*
        * 绘制图片
        * @param imageData 图片数据
        * @param x 起始点左上角水平方向位置
        * @param y 起始点左上角垂直方向位置
        * @param width 宽
        * @param height 高
        * @param imageProcessingType 图像处理方式（默认使用0）
        *  							1:阈值法
        *								2:渐变
        * @param imageProcessingValue 图像处理阈值（默认使用0）
        */
bool DrawLableImage(const char* imageData, float x, float y, float width, float height, int rotate, int imageProcessingType, float imageProcessingValue);


/*
     * 生成预览图
     * @param displayMultiple 显示器倍率（如8.0f）
     * @return 图像数据
     */
char* GeneratePrintPreviewImage(float displayMultiple);

/*
     * 释放缓存buffer
     */
void FreeBuffer(char* ptr);
}



#endif // JCPRINTER_H
