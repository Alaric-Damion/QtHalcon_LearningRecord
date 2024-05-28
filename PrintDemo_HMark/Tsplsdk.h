#ifndef TSPLSDK_H
#define TSPLSDK_H
int (*PrinterCreator)(void** handle, const char* model);
int (*PortClose)(void* handle);
int (*PortOpen)(void* handle, const char* ioSettings);
int (*PrinterDestroy)(void* handle);
int (*TSPL_Setup)(void* handle,
             int labelWidth,
             int labelHeight,
             int speed,
             int density,
             int type,
             int gap,
             int offset);
int (*TSPL_ImagePrint)(void* handle,
                  const TCHAR* chImagePath,
                  int iXPos,
                  int iYPos,
                  int iPrintImageMode,
                  int iBrightness,
                  int iContrast,
                  int iHtMode,
                  int iTagNum,
                  int iCopies);
int (*TSPL_TextPrint)(void* handle,
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
                 int iFontSize);
int (*TSPL_QrCodePrint)(void* handle,
                   const TCHAR* chText,
                   int iTextLenth,
                   int iXPos,
                   int iYPos,
                   int iPrintImageMode,
                   int iBrightness,
                   int iContrast,
                   int iHtMode,
                   int iTagNum,
                   int iCopies);
int (*TSPL_Text)(void* handle,
                 int xPos,
                 int yPos,
                 int font,
                 int rotation,
                 int xMultiplication,
                 int yMultiplication ,
                 int alignment,
                 const char* data);

#endif // TSPLSDK_H
