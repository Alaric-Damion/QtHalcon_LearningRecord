#ifndef MSPRINTSDK_H
#define MSPRINTSDK_H

int (*SetPrintport)(const char* strPort,int iBaudrate);
int (*SetUsbportauto)(); 
int (*SetInit)();
int (*SetClean)();
int (*SetClose)();
int (*SetAlignment)(int iAlignment);
int (*SetAlignmentLeftRight)(int iAlignment);
int (*SetSizetext)(int iHeight,int iWidth);
int (*SetSizechar)(int iHeight,int iWidth,int iUnderline,int iAsciitype);
int (*SetSizechinese)(int iHeight,int iWidth,int iUnderline,int iChinesetype);
int (*SetLinespace)(int iLinespace);
int (*SetBold)(int iBold);
int (*SetSpacechar)(int iSpace);
int (*SetSpacechinese)(int iChsleftspace,int iChsrightspace);
int (*SetNvbmp)(int iNums,const char* strPath);
int (*SetMarkoffsetcut)(int iOffset);
int (*SetMarkoffsetprint)(int iOffset);
int (*SetLeftmargin)(int iLeftspace); 
int (*SetDirection)(int iDirection);
int (*SetItalic)(int iItalic);
int (*SetRotate)(int iRotate); 
int (*SetWhitemodel)(int iWhite);
int (*SetCommandmode)(int iMode);
int (*SetUnderline)(int underline);
int (*SetReadZKmode)(int mode);
int (*SetHTseat)(const char* bHTseat,int iLength);
int (*PrintNextHT)();
int (*PrintSelfcheck)();
int (*PrintString)(const char* strData, int iImme) ;
int (*PrintFeedline)(int iLine);
int (*PrintCutpaper)(int iMode);
int (*PrintQrcode)(const char* strData,int iLmargin,int iMside,int iRound); 
int (*PrintQRcode500II)(int iwidth,const char* strData); 
int (*Print1Dbar)(int iWidth,int iHeight,int iHrisize,int iHriseat,int iCodetype,const char* strData);
int (*PrintPdf417)(int iDotwidth,int iDotheight,int iDatarows,int iDatacolumns,const char* strData);
int (*PrintDiskbmpfile)(const char* strPath);
int (*PrintNvmbp)(int iNvindex,int iMode);
int (*PrintMarkposition)();
int (*PrintMarkpositionprint)();
int (*PrintMarkpositioncut)();
int (*PrintMarkcutpaper)(int iMode);
int (*PrintTransmit)(const char* bCmd,int iLength);
int (*PrintChangeRow)();
int (*PrintRemainQR)();
int (*PrintFeedDot)(int Lnumber);
int (*GetProductinformation)(int iFstype,char *bFiddata,int iFidlen);

int (*GetTransmit)(const char* bCmd,int iLength,char* bRecv,int iRelen);
int (*GetStatus)();
int (*GetStatusspecial)();
int (*GetSDKinformation)(char *bInfodata);

int (*SetRotation_Intomode)();
int (*PrintRotation_Data)();
int (*PrintRotation_Sendtext)(const unsigned char* strData,int iImme);
int (*PrintRotation_Sendcode)(int leftspace,int iWidth,int iHeight,int iCodetype,const char* strData);
int (*PrintRotation_Changeline)();
int (*SetRotation_Leftspace)(int iLeftspace);
int (*SetPrintIDorName)(const char* strIDorNAME);
int (*GetPrintIDorName)(char* strIDorNAME); 
int (*SetComportauto)();
int (*SetPrintportFlowCtrl)(int iFlowCtrlFlag);
void (*SetTraceLog)(int iLog);

int (*PrintPDF_CCCB_A)(const char* strPath);
int (*PrintPDF_CCCB_B)(const char* strPath);
int (*GetStatus_TS)();
int (*SetPrintConn)(int iConnWay,const char *strName,const char *strValue);
int (*PrintDataMatrix)(const char* strData, int iSize) ;
#endif


