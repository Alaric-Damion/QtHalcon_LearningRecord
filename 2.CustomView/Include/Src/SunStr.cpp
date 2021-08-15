
#include "stdafx.h"
#include "SunStr.h"
#include "SunPublic.h"

namespace sunLib{


// ����ļ�·������\��
CString GetFilePath(CString AFileName)
{
    CString sPath;
    int iPos,iCount;
    // ������ļ�������ȡ��������ַ�
    iCount=AFileName.GetLength();
    if(DirectoryExists(AFileName)){
        iPos=iCount-1;
        sPath=AFileName;
    }
    else{
        iPos=AFileName.ReverseFind(_T('\\'));
        if(iPos>=0)
        {
            sPath=AFileName.Left(iPos+1);
        }
        else
        {
            sPath="";
        }
    }
    if(sPath.Right(1)!=_T("\\")){
        sPath+=_T("\\");
    }
    return sPath;
}

// ����ļ���
CString GetFileName(CString AFilePath)
{
    // CString sFileName;
    int iCount=AFilePath.GetLength();
    int iIndex=AFilePath.ReverseFind(_T('\\'));
    if(iIndex>=0) return AFilePath.Right(iCount-iIndex-1);
    if(iIndex<0){
        iIndex=AFilePath.ReverseFind(_T(':'));
        if(iIndex>=0) return AFilePath.Right(iCount-iIndex-1);
    }
    return _T("");
}

// ����ļ�����չ��(a_bExtend:True ������չ�� ���򷵻ز�����չ�����ļ���)
CString GetFileNameExtend(CString a_sFileName,bool a_bExtend){
    CString sFileName=GetFileName(a_sFileName);
    if(sFileName.IsEmpty()) sFileName=a_sFileName;
    int iPos=sFileName.ReverseFind(_T('.'));
    if(a_bExtend){
        return sFileName.Right(sFileName.GetLength()-iPos);
    }
    return sFileName.Left(iPos);
}


// ����ת��Ϊ�ַ���
// 2004.9.23
CString IntToStr(int AValue)
{
    CString s;
    s.Format(_T("%d"),AValue);
    return s;
}
CString IntToStr(INT64 AValue)
{
    CString s;
    s.Format(_T("%lld"),AValue);
    return s;
}

// ������ת��Ϊ�ַ���
// 2006.4.25
CString FloatToStr(double AValue)
{
    CString s;
    s.Format(_T("%f"),AValue);
    return s;
}

// �ַ����ϲ�����
// 2008.11.13
CString Format(const _TCHAR* strLine, ...)
{
    va_list argList;
    va_start(argList, strLine);
    CString s;
    s.FormatV(strLine,argList);
    va_end(argList);
    return s;
}

// 2009.5.12
#ifdef _UNICODE
    wstring FormatString(const _TCHAR *strLine, ...)
    {
        va_list argList;
        va_start(argList, strLine);
        CString s;
        s.FormatV(strLine,argList);
        wstring FormatString(const _TCHAR *strLine, ...);
        wstring sResult(s.GetBuffer(0));
        va_end(argList);
        return sResult;
    }
#else
    string FormatString(const _TCHAR *strLine, ...)
    {
        va_list argList;
        va_start(argList, strLine);
        CString s;
        s.FormatV(strLine,argList);
        string FormatString(const _TCHAR *strLine, ...);
        string sResult(s.GetBuffer(0));
        va_end(argList);
        return sResult;
    }
#endif

// �����һ���ָ���������һ���ִ�(����Ϊ 1)
// 2004.12.14
CString GetStringSign(CString ASource,CString AFind,int AIndex)
{
    CString sResult;
    int iFind=0;
    int iOldFind=0;
    int iFindLen=AFind.GetLength();
    for(int i=0;i<AIndex;i++){
        iFind=ASource.Find(AFind,iOldFind);
        if(AIndex==(i+1)){
            if(iFind<0) {
                sResult=ASource.Right(ASource.GetLength()-iOldFind);
            }
            else {
                sResult=ASource.Mid(iOldFind,iFind-iOldFind);
            }
            break;
        }
        iOldFind=iFind+iFindLen;
    }
    return sResult;
}

// ���ܣ��ֽ��зָ��������⣩���ַ���
// 2006.3.18
int ExpandStrToArray(CString a_sSource,CString a_sSign,CStringArray& a_aExp)
{
    int i;
    int iLen;
    int iP=-1;
    int iResult=0;
    i=0;
    int iSignLen=a_sSign.GetLength();
    iLen=a_sSource.GetLength();
    while(true){
        iP=a_sSource.Find(a_sSign,i);
        if(iP<0) break;
        if(iP>i){
            a_aExp.Add(a_sSource.Mid(i,iP-i));
            iResult++;
        }
        i=iP+iSignLen;
    }
    if(i<iLen){   
        a_aExp.Add(a_sSource.Mid(i,iLen-i));
        iResult++;
    }
    return iResult;
}

// �ж�һ���ַ����Ƿ���ת��Ϊ����
// 2005.6.16
BOOL IsNumeric(CString AValue)
{
    BOOL bResult=TRUE;
    if(AValue.IsEmpty()) return FALSE;
    for(int i=0;i<AValue.GetLength();i++) {
        // ����Ϊ�������ַ�
        if(AValue[i]<48 || AValue[i]>57){
            // �жϸ��ַ��Ƿ�Ϊ��һ���ַ�������������ת��Ϊ����
            if(i==0 && AValue[i]=='-') continue;
            bResult=FALSE;
            break;
        }
    }
    return bResult;
}

// �ַ�����ʾһ������(����)
// AValue: �����ķ���
// AMax:   �����ķ�ĸ
// ABack:  ��ʾ�յ��ַ�
// AFront: ��ʾ���ȵ��ַ�
// ACount: �ö��ٸ��ַ���ʾ�ñ���
// ʾ����ConvPercentToString(50,100,"��","��",10);
// 2007.6.12
CString ConvPercentToString(double AValue,double AMax,CString ABack,CString AFront,int ACount)
{
    CString sResult("");
    int i;
    int iPosition=(int)((AValue/AMax)*ACount);
    for(i=0;i<iPosition;i++){
        sResult+=AFront;
    }
    for(i=iPosition;i<ACount;i++){
        sResult+=ABack;
    }
    return sResult;
}

// ת������������Ϊ�ַ���
// a_pStr : ���� _TCHAR* �ĵ�ַ�������ڲ����Զ�����ռ䣬����ʹ����Ϻ������ͷſռ�
// 2009.3.24
void TranBinaryToStr(IN const BYTE* a_pBin,IN const UINT a_iSize,OUT _TCHAR** a_pStr)
{
    _TCHAR a[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    *a_pStr=new _TCHAR[a_iSize*2+1];
    *((*a_pStr)+a_iSize*2)=0;
    for(UINT i=0;i<a_iSize;i++){
        //sprintf(*a_pStr+i*2,"%2.2X",a_pBin[i]);
        int iValue=a_pBin[i];
        *(*a_pStr+i*2)  =a[iValue/16];
        *(*a_pStr+i*2+1)=a[iValue % 16];
    }
}

// �����ֽ���(����Ȼ���������ֽ���) 2016.7.28
CString TranslateBytesToNormal(__int64 a_iBytes){
    __int64 aUnit[]={0,1024,1048576,1073741824,1099511627776,1125899906842624};
    CString aUnitText[]={_T("B"),_T("K"),_T("M"),_T("G"),_T("T"),_T("P")};
    int iSize=sizeof(aUnit)/sizeof(aUnit[0]);
    int iIndex;
    for(iIndex=iSize-1;iIndex>=0;iIndex--){
        if(a_iBytes>=aUnit[iIndex]) break;
    }
    double fCount;
    if(aUnit[iIndex]==0) fCount=(double)a_iBytes;
    else                 fCount=(double)a_iBytes/(double)aUnit[iIndex];
    return Format(_T("%.2f%s"),fCount,aUnitText[iIndex]);
}

// ����һ���ַ������ļ�
// 2007.2.15
bool SaveStringToFile(CString AString,const CString AFileName)
{
    try{
        CFile f;
        if(FileExists(AFileName)){
            if(!f.Open(AFileName,CFile::modeReadWrite)){
                return false;
            }
            f.SeekToEnd();
        }
        else{
            if(!f.Open(AFileName,CFile::modeCreate | CFile::modeWrite)){
                return false;
            }
        }
        f.Write(AString.GetBuffer(0),AString.GetLength());
        f.Write(Crlf,2);
        f.Close();
    }
    catch (...) {
        return false;
    }
    return true;
}

// �������ݵ��ļ� 2009.8.25
bool SaveLogToFile(CString a_sLog,CString a_sFileTitle)
{
    CString sLogFileName;
    CString sTime;
    SYSTEMTIME t;
    GetLocalTime(&t);
    sTime.Format(_T("[%2.2d:%2.2d:%2.2d.%3.3d]"),t.wHour,t.wMinute,t.wSecond,t.wMilliseconds);

    sLogFileName.Format(_T("%sLog\\%s%s.log"),GetAppExePath(),Format(_T("%d-%2.2d-%2.2d"),t.wYear,t.wMonth,t.wDay),a_sFileTitle);
    MakeSureDirectoryFileExist(sLogFileName);
    //LockFile();
    bool bResult=SaveStringToFile(sTime+_T(" ")+a_sLog,sLogFileName);
    //UnlockFile();
    return bResult;
}

// �������ݵ��ļ� 2009.8.25
bool SaveTextToFile(CString a_sLog,CString a_sFileName)
{
    MakeSureDirectoryFileExist(a_sFileName);
    //LockFile();
    bool bResult=SaveStringToFile(a_sLog,a_sFileName);
    //UnlockFile();
    return bResult;
}

// �ַ��������ж�
// ? ��������һ���ַ���������Ϊ��
// * ���������ַ���������Ϊ��
// 2011.11.16
bool TextRuleCheckEx(LPCTSTR a_sTextRule,LPCTSTR a_sText)
{
    if(_tcsstr(a_sText,_T("*"))) return false;
    if(_tcsstr(a_sText,_T("\?"))) return false;
    int iRuleLen=(int)_tcslen(a_sTextRule);
    int iTextLen=(int)_tcslen(a_sText);
    _TCHAR cRule;
    bool bAllMatchModel=false;          // ȫƥ��ģʽ

    int iTextIndex=0;
    for(int i=0;i<iRuleLen;i++){
        cRule=a_sTextRule[i];
        if(bAllMatchModel){
            bool bFind=false;
            while(iTextIndex<iTextLen){
                if(a_sText[iTextIndex]==cRule){
                    bAllMatchModel=false;
                    bFind=true;
                    break;
                }
                iTextIndex++;
            }
            if(!bFind) return false;
        }
        else{
            if(cRule=='*') {
                bAllMatchModel=true;
                continue;
            }
            else if(cRule!=a_sText[iTextIndex] && cRule!='?' && cRule!='#' && cRule!='$'){
                return false;
            }
            if(cRule=='\?'){
                if(!a_sText[iTextIndex]) return false;
            }
            if(cRule=='#'){
                if(!IsNumeric(CString(a_sText[iTextIndex]))) return false;
            }
            if(cRule=='$'){
                if(a_sText[iTextIndex]<'A' || a_sText[iTextIndex]>'Z') return false;
            }
        }
        iTextIndex++;
    }
    if(!bAllMatchModel) if(iTextIndex<iTextLen) return false;
    return true;
}

// ��ò�����չ�����ļ���
// ����������·�����ļ���
CString GetSimpleFileName(CString a_sFileName)
{
    int iPos=a_sFileName.ReverseFind('.');
    return a_sFileName.Left(iPos);
}

wstring MutilByteToWide(const string& _src)
{
    // �����ַ��� string ת�� w_TCHAR_t ֮��ռ�õ��ڴ��ֽ���
    int nBufSize = MultiByteToWideChar(GetACP(),0,_src.c_str(),-1,NULL,0);

    // Ϊ wsbuf �����ڴ� BufSize ���ֽ�
    wchar_t *wsBuf = new wchar_t[nBufSize];

    // ת��Ϊ unicode �� WideString
    MultiByteToWideChar(GetACP(),0,_src.c_str(),-1,wsBuf,nBufSize);

    wstring wstrRet(wsBuf);

    delete []wsBuf;
    wsBuf = NULL;

    return wstrRet;
}

string WideToMutilByte(const wstring& _src)
{
    int nBufSize = WideCharToMultiByte(GetACP(), 0, _src.c_str(),-1, NULL, 0, 0, FALSE);
    char *szBuf = new char[nBufSize];
    WideCharToMultiByte(GetACP(), 0, _src.c_str(),-1, szBuf, nBufSize, 0, FALSE);
    string strRet(szBuf);
    delete []szBuf;
    szBuf = NULL;

    return strRet;
}

}