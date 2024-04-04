#include "jcPrinter.h"
#include "jcPrinterApi.h"


bool Printer::loadPrinterApiDll() {
    return jcPrinterApi::loadPrinterApiDll();
}

bool Printer::initSdk(char* fontDir)
{
    return jcPrinterApi::initSdk(fontDir);
}

int Printer::getLastPrinterError()
{
    return jcPrinterApi::getPrintLastError();
}

char* Printer::getAllPrinters()
{
    return jcPrinterApi::getAllPrinters();
}

bool Printer::selectPrinter(char* printerName, int port)
{
    return jcPrinterApi::selectPrinter(printerName, port);
}

bool Printer::disconnectPrinter()
{
    return jcPrinterApi::disconnectPrinter();
}

bool Printer::startJob(int printLabelType, int printDensity, int printMode, int printCount)
{
    return jcPrinterApi::startJob(printLabelType, printDensity, printMode, printCount);
}

bool Printer::commitJob()
{
    return jcPrinterApi::commitJob();
}

bool Printer::picturePrint(char* printData, size_t dataLenght, int verticalShift, int horizontalShift, int threshold)
{
    return jcPrinterApi::picturePrint(printData, dataLenght, verticalShift, horizontalShift, threshold);
}

bool Printer::endJob()
{
    return jcPrinterApi::endJob();
}

bool Printer::isConnected()
{
    return jcPrinterApi::isConnected();
}

char* Printer::scanWifiPrinter()
{
    return jcPrinterApi::scanWifiPrinter();
}

bool Printer::connectWifiPrinter(char* printerName, int port)
{
    return jcPrinterApi::connectWifiPrinter(printerName, port);
}

bool Printer::configurationWifi(char* wifiName, char* wifiPassword)
{
    return jcPrinterApi::configurationWifi(wifiName, wifiPassword);
}

bool Printer::InitDrawingBoard(float width, float height, int rotate, const char* font, int verticalShift, int HorizontalShift)
{
    return jcPrinterApi::InitDrawingBoard(width, height, rotate, font, verticalShift, HorizontalShift);
}

bool Printer::DrawLableText(float x, float y, float width, float height, const char* value, const char* fontFamily, float fontSize, int rotate,
                            int textAlignHorizonral, int textAlignVertical, int lineMode, float letterSpacing, float lineSpacing, bool fontStyle[4])
{
    return jcPrinterApi::DrawLableText(x, y, width, height, value, fontFamily, fontSize, rotate,
                                       textAlignHorizonral, textAlignVertical, lineMode, letterSpacing, lineSpacing, fontStyle);
}

bool Printer::DrawLableBarCode(float x, float y, float width, float height, int codeType, const char* value, float fontSize,
                               int rotate, float textHeight, int textPosition)
{
    return jcPrinterApi::DrawLableBarCode(x, y, width, height, codeType, value, fontSize,
                                          rotate, textHeight, textPosition);
}

bool Printer::DrawLableQrCode(float  x, float y, float width, float height, const char* value, int codeType, int rotate)
{
    return jcPrinterApi::DrawLableQrCode(x, y, width, height, value, codeType, rotate);
}

bool Printer::DrawLableLine(float x, float y, float width, float height, int rotate, int lineType, float dashwidth[2])
{
    return jcPrinterApi::DrawLableLine(x, y, width, height, rotate, lineType, dashwidth);
}

bool Printer::DrawLableGraph(float x, float y, float width, float height, int graphType, int rotate,
                             float cornerRadius, float lineWidth, int lineType, float dashwidth[2])
{
    return jcPrinterApi::DrawLableGraph(x, y, width, height, graphType, rotate, cornerRadius, lineWidth, lineType, dashwidth);
}

bool Printer::DrawLableImage(const char* imageData, float x, float y, float width, float height, int rotate,
                             int imageProcessingType, float imageProcessingValue)
{
    return jcPrinterApi::DrawLableImage(imageData, x, y, width, height, rotate, imageProcessingType, imageProcessingValue);
}

char* Printer::GeneratePrintPreviewImage(float displayMultiple)
{
    return jcPrinterApi::GeneratePrintPreviewImage(displayMultiple);
}

void Printer::FreeBuffer(char* ptr)
{
    jcPrinterApi::FreeBuffer(ptr);
}
