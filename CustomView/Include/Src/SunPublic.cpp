
#include "stdafx.h"
#include "SunPublic.h"
#include <math.h>
#include "SunDIBapi.h"
#include "SunStr.h"
#include "TreePackInterface.h"


namespace sunLib{
// 获得屏幕的宽
// 2004.8.8
long GetScreenWidth()
{
    return GetSystemMetrics(SM_CXSCREEN);
};

// 获得屏幕的高
long GetScreenHeight()
{
    return GetSystemMetrics(SM_CYSCREEN);
};

// 等待系统
BOOL DoEvents()
{
    static MSG msg;
    
    while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) {
        if (!AfxGetApp()->PumpMessage()) {
            ::PostQuitMessage(0);
            return FALSE;
        } 
    }
    return TRUE;
}

// 运行一个可执行程序
// 2004.9.29
BOOL LaunchApplication(LPCTSTR pCmdLine,PROCESS_INFORMATION* pProcessInfo)
{
    STARTUPINFO stStartUpInfo;
    memset(&stStartUpInfo,0,sizeof(STARTUPINFO));
    stStartUpInfo.cb=sizeof(STARTUPINFO);
    stStartUpInfo.dwFlags=STARTF_USESHOWWINDOW;
    stStartUpInfo.wShowWindow=SW_SHOWDEFAULT;
    return CreateProcess(NULL,(LPTSTR)pCmdLine,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,
        NULL,&stStartUpInfo,pProcessInfo);
}

// 显示一个消息框
UINT MyMessageBox(CString ACaption,int AType)
{
    return AfxMessageBox(ACaption,MB_OK|MB_ICONINFORMATION);
}

// 显示文字到 DC
// 2009.9.25
void ShowTextToDC(CDC& a_dc,CString a_sText,CRect a_rRect,COLORREF a_dwFontColor,
    int a_iFontSize,UINT nFormat,int nWeight,CString a_sFontName,UINT a_iChartSet)
{
    CFont font;
    VERIFY(font.CreateFont(
        a_iFontSize,               // nHeight
        0,                         // nWidth
        0,                         // nEscapement
        0,                         // nOrientation
        nWeight,                   // nWeight
        FALSE,                     // bItalic
        FALSE,                     // bUnderline
        0,                         // cStrikeOut
        a_iChartSet,            // nCharSet
        OUT_DEFAULT_PRECIS,        // nOutPrecision
        CLIP_DEFAULT_PRECIS,       // nClipPrecision
        DEFAULT_QUALITY,           // nQuality
        DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
        a_sFontName));             // lpszFacename
    a_dc.SelectObject(font);
    a_dc.SetTextColor(a_dwFontColor);
    a_dc.DrawText(a_sText,a_rRect,nFormat);
    font.DeleteObject();
}

////////////////////////////////////////////////////////////////////////////////
// 功能     : 获得一个图片在一个容器中显示的形式
// 参数     :
// APanelRect   : 容器的大小
// APicRect     : 图片的大小
// AAutoZoomIn  : 是否自动放大
// AWidthSpace  : 预留边框的大小（默认为0）
// AHeightSpace : 预留边框的大小（默认为0）
// 返回值   :
// 日期     : 2003.9.14
// 说明     : C++ 版本于2011.3.28日转换自 Delphi 版本
////////////////////////////////////////////////////////////////////////////////
CRect GetPictionRect(CPoint APanelRect,CPoint APicRect,bool AAutoZoomIn,bool AAutoZoomOut,int AWidthSpace,int AHeightSpace)
{
    double dblPencent;  // 缩放比
    double wx;          // 宽缩放比
    double hx;          // 高缩放比
    CPoint ptPanel     ;
    int    x,y         ;
    CRect  r;

    CRect rResult(0,0,0,0);
    if(APicRect.x<=0) return rResult;
    if (APicRect.y<=0) return rResult;

    ptPanel.x=APanelRect.x-AWidthSpace*2;
    ptPanel.y=APanelRect.y-AHeightSpace*2;

    // 获得缩放比
    wx=(double)ptPanel.x/(double)APicRect.x;
    hx=(double)ptPanel.y/(double)APicRect.y;
    if(wx>hx) dblPencent=hx;
    else      dblPencent=wx;
    // 是否自动缩小
    if(!AAutoZoomOut){
        if(dblPencent<1) dblPencent=1;
    }       
    // 是否不会自动放大
    if(!AAutoZoomIn) if(dblPencent>1) dblPencent=1;

    // 获得图片最后的大小
    x=int(dblPencent*APicRect.x);
    y=int(dblPencent*APicRect.y);

    // 计算出图片的位置

    r.left=int((double)(ptPanel.x-x) / 2)+AWidthSpace;
    r.top=int((double)(ptPanel.y-y) / 2)+AHeightSpace;

    r.right=r.left+x;
    r.bottom=r.top+y;

    rResult=r;
    return rResult;
}

// 按键盘键菜单组合键(即 Alt 键)
// AKey : 按键(如: 'A')
// 2004.8.19
void MenuKeyboardClick(byte AKey)
{
    keybd_event((BYTE)VK_MENU,(BYTE)MapVirtualKey(VK_MENU,0),0,0);
    keybd_event(AKey,(BYTE)MapVirtualKey(AKey,0),0,0);
    keybd_event(AKey,(BYTE)MapVirtualKey(AKey,0),KEYEVENTF_KEYUP,0);
    keybd_event((BYTE)VK_MENU,(BYTE)MapVirtualKey(VK_MENU,0),KEYEVENTF_KEYUP,0);
}

// 将两个点转换为一个矩形
// 自动取得一个最小点和最大点，然后组成一个矩形
// AExpand	: 用于扩展，即向周围扩展像素，使得该矩形变大
// 2004.7.6
CRect GetPointToRect(CPoint APoint1,CPoint APoint2,int AExpand)
{
    CRect r;
    if(APoint1.x>APoint2.x) 
    {
        r.left=APoint2.x-AExpand;
        r.right=APoint1.x+AExpand;
    }
    else
    {
        r.left=APoint1.x-AExpand;
        r.right=APoint2.x+AExpand;
    }
    if(APoint1.y>APoint2.y)
    {
        r.top=APoint2.y-AExpand;
        r.bottom=APoint1.y+AExpand;
    }
    else
    {
        r.top=APoint1.y-AExpand;
        r.bottom=APoint2.y+AExpand;
    }
    return r;
}

// 限制一个矩形的范围
// ARect	: 给定的矩形
// ALimit	: 限制的范围
// 2004.7.8
CRect SpeciRectLimit(CRect ARect,CRect ALimit)
{
    CRect r=ARect;
    if(ARect.left<ALimit.left       || ARect.left>=ALimit.right)    r.left=ALimit.left;
    if(ARect.top<ALimit.top         || ARect.top>=ALimit.bottom)    r.top=ALimit.top;
    if(ARect.right>ALimit.right     || ARect.right<=ALimit.left)    r.right=ALimit.right;
    if(ARect.bottom>ALimit.bottom   || ARect.bottom<=ALimit.top)    r.bottom=ALimit.bottom;
    return r;
}

// 限制一个点的范围
// APoint	: 给定的点
// ALimit	: 限制的范围
// 2004.7.8
CPoint SpeciPointLimit(CPoint APoint,CRect ALimit)
{
    CPoint pointTemp=APoint;
    if(APoint.x<ALimit.left)	pointTemp.x=ALimit.left;
    if(APoint.y<ALimit.top)		pointTemp.y=ALimit.top;
    if(APoint.x>ALimit.right)	pointTemp.x=ALimit.right;
    if(APoint.y>ALimit.bottom)	pointTemp.y=ALimit.bottom;
    return pointTemp;
}

// 移动一个矩形区域
// ARect    : 矩形区域
// APoint   : 移动距离
// AMove    : 是否移动，如果为否，则将开始位置定为要移动的坐标
// 2004.7.29
CRect MoveRect(CRect ARect,CPoint APoint,BOOL AMove)
{
    int w,h;
    CRect r;
    w=ARect.Width();            h=ARect.Height();
    if(AMove) 
    {
        r.left=ARect.left+APoint.x; 
        r.top=ARect.top+APoint.y;
    }
    else
    {
        r.left=APoint.x;
        r.top=APoint.y;
    }
    r.right=r.left+w;           r.bottom=r.top+h;
    return r;
}

// 移动一个点
// 2004.8.18
CPoint MovePoint(CPoint ASource,CPoint APoint)
{
    CPoint p;
    p.x=ASource.x+APoint.x;
    p.y=ASource.y+APoint.y;
    return p;
}

// 返回一个矩形是否全部包含另一个矩形区域(ASource 包含 ATarget)
// 2006.3.29
bool IfContainRect(CRect ASource,CRect ATarget)
{
    return (ATarget.left>=ASource.left && ATarget.top>=ASource.top && ATarget.right<=ASource.right && ATarget.bottom<=ASource.bottom);
}

// 获得应用程序的全名称（含路径）
// 2004.8.8
CString GetAppExeName()
{
    CString sExeName;
    TCHAR exeFullPath[MAX_PATH];
    GetModuleFileName(NULL,exeFullPath,MAX_PATH);
    sExeName.Format(_T("%s"),exeFullPath);
    return sExeName;
}

// 获得应用程序所在的目录
// 2004.8.8
CString GetAppExePath()
{
    return GetFilePath(GetAppExeName());
}

// 获得当前进程信息 2009.1.5
PROCESSENTRY32 SunGetCurrentProcessInfo()
{
    PROCESSENTRY32 pe={sizeof(PROCESSENTRY32)};
    HANDLE hthSnapshot = NULL;
    DWORD hProcId=0;
    DWORD dwCurrProcessID;
    BOOL bProcRet;
    GetWindowThreadProcessId(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), &hProcId);
    if(hProcId==0) {
        goto End;
    }
    hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, hProcId);
    if (hthSnapshot == NULL) goto End;
    
    dwCurrProcessID=GetCurrentProcessId();
    
    bProcRet=Process32First(hthSnapshot,&pe);
    for(;bProcRet;bProcRet=Process32Next(hthSnapshot,&pe)){
        if(dwCurrProcessID==pe.th32ProcessID){
            goto End;
        }
    }
End:
    CloseHandle(hthSnapshot);
    return pe;
}

// 判断一个文件是否存在
BOOL FileExists(CString AFileName)
{
	WIN32_FIND_DATA f;
    HANDLE hWnd;
    BOOL bResult;
    hWnd=FindFirstFile(AFileName,&f);
    bResult=(hWnd!=INVALID_HANDLE_VALUE);
    FindClose(hWnd);
	return bResult;
}

// 判断一个文件夹是否存在
// 2004.11.11
BOOL DirectoryExists(CString AFileName)
{
    CString sDirPath=AFileName;
    if(sDirPath.GetLength()>0) {    // 去掉最后一个反斜杠
        if(sDirPath.Right(1)=='\\') sDirPath=AFileName.Left(AFileName.GetLength()-1);
    }
    WIN32_FIND_DATA f;
    HANDLE h;
    h=FindFirstFile(sDirPath,&f);
    if((f.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0)
    {
        FindClose(h);
        return (h!=INVALID_HANDLE_VALUE);
    }
    else
    {
        FindClose(h);
        return FALSE;
    }
}

// 删除一个目录树
// 2005.7.19
bool DelDirTree(CString APath)
{ 
    CFileFind ff;
    CString szDir=APath;
    if (szDir.Right(1)!=_T("\\")) szDir+=_T("\\");
    szDir+=_T("*.*");
    
    BOOL bfile=ff.FindFile(szDir);
    while (bfile)
    {
        bfile=ff.FindNextFile();
        if (ff.IsDirectory() && !ff.IsDots()) {
            RemoveDirectory(ff.GetFilePath());
            DelDirTree(ff.GetFilePath());
        }
        else if (!ff.IsDots() && !ff.IsDirectory())  DeleteFile(ff.GetFilePath());
    }
    ff.Close();
    return Bool(RemoveDirectory(APath));
}

// 创建目录(可以自动创建不存在的目录)
// 说明：目录名要包含最后一个 '\'
// 2005.12.12
bool MakeSureDirectoryFileExist(CString APath)
{
    int i;
    int iCount;
    CString sPath;
    bool bResult=true;
    
    iCount=APath.GetLength();
    for(i=3;i<iCount;i++){
        if(APath.Mid(i,1)=="\\"){
            sPath=APath.Left(i);
            if(!DirectoryExists(sPath)) {
                bResult=CreateDirectory(sPath,NULL)?true:false;
                if(!bResult) return false;
            }
        }
    }
    return bResult;
}

// 获得一个随机数
// 2004.12.26
long GetRandom(long AMaxValue)
{
    double dblR;
    dblR=(double)rand();
    return (long)(dblR/(double)RAND_MAX*(double)AMaxValue);
}

// 返回Base的Exponent次方
__int64 IntPower(int Base,int Exponent)
{
    __int64 iResult = 1;
    for(int i=0;i<Exponent;i++){
        iResult = iResult * Base;
    }
    return iResult;
}

// 任意进制转换为整数(最多支持 36 进制)
// 返回进制表示转换成整数;mScale指定多少进制 
__int64 DigitToInt(CString mDigit,byte mScale)
{
    const CString cScaleChar = _T("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    int  L;
    __int64 iResult=0;
    mDigit.MakeUpper();
    L = mDigit.GetLength();
    for(int i=0;i<L;i++){
        int iIndex=cScaleChar.Find(mDigit[L-i-1],0);
        iResult=iResult+iIndex*IntPower(mScale,i);
    }
    return iResult;
}

// 四舍五入函数(正数向上取整，负数向下取整)
// count : 为保留小数点的位数(如: 
//         Round(0.555,0)=1
//         Round(0.555,1)=0.6
// 2006.3.25
double Round(double AValue,int count)
{
    double dblSign=1;
    double dblValue=AValue;
    if(AValue<0) {
        dblSign=-1;
        dblValue=-AValue;
    }
    double temp=pow(10.0,count);
    temp=temp?temp:1;
    return int(dblValue*temp+0.5)/temp*dblSign;
    
}


//////////////////////////////////////////////////////////////////////////
// CSunThread 2016.11.10 

CSunThread::CSunThread(){
    m_dwSunThreadID =0;
    m_hSunThread    =NULL;
    m_iSunThreadCount  =0;
};

HANDLE CSunThread::SunCreateThread(void* a_pOwner){
    tagThreadParamInfo* pThreadParamInfo=new tagThreadParamInfo; // 创建线程参数
    pThreadParamInfo->m_pThis   =this;
    pThreadParamInfo->m_pOwner  =a_pOwner;
    m_hSunThread=(HANDLE)_beginthreadex(NULL,0,SunThreadExecute,(void*)pThreadParamInfo,CREATE_SUSPENDED,&m_dwSunThreadID);
    ResumeThread(m_hSunThread);
    Lock();
    m_iSunThreadCount++;
    Unlock();
    return m_hSunThread;
}

UINT _stdcall CSunThread::SunThreadExecute(void* a_pParam){
    UINT iResult=0;
    if(!a_pParam) return iResult;
    tagThreadParamInfo* pThreadParamInfo=(tagThreadParamInfo*)a_pParam;
    CSunThread* me=pThreadParamInfo->m_pThis;

    if(me){
        iResult=me->SunExecute(pThreadParamInfo->m_pOwner);
        me->Lock();
        me->m_iSunThreadCount--;
        me->Unlock();
        me->SunEnd();
    }
    if(pThreadParamInfo) delete pThreadParamInfo;
    return iResult;
}

void CSunThread::SunEnd(){
    
}

//////////////////////////////////////////////////////////////////////////
// CIniFile 类开始
CIniFile::CIniFile()
{
}
CIniFile::~CIniFile()
{
    if(m_aFileContainer.GetSize() > 0){
        m_aFileContainer.RemoveAll();
    }
}
bool CIniFile::Load(const CString a_sFileName)
{
    m_sFileName=a_sFileName;
    CStdioFile      stfFile;
    m_aFileContainer.RemoveAll();
    if(!FileExists(a_sFileName)){
        return true;
    }

    if(!stfFile.Open(a_sFileName,CFile::modeRead)){
        return false;
    }
    CString strFileLine;
    while(true){
        if(!stfFile.ReadString(strFileLine)){
            break;
        }
        strFileLine.TrimRight();
        m_aFileContainer.Add(strFileLine);
    }
    stfFile.Close();
    return true;
}

bool CIniFile::Save()
{
    return Save(m_sFileName);
}

bool CIniFile::Save(const CString a_sFileName)
{
    CStdioFile      stfFile;
    CString sFile=a_sFileName+_T("~.tmp");
    if(stfFile.Open(sFile,CFile::modeCreate | CFile::modeWrite) && m_aFileContainer.GetSize() > 0){
        CString strParam;
        for(int i = 0; i< m_aFileContainer.GetSize();i++){
            strParam = m_aFileContainer[i];
            stfFile.WriteString(strParam+_T("\n"));
        }
    }
    stfFile.Close();
    CIniFile iniFile;
    iniFile.Load(sFile);
    if(iniFile.GetData()->GetSize()>0){
        DeleteFile(a_sFileName);
        _trename(sFile,a_sFileName);
    }
    else{
        return false;
    }
    return true;
}

// 设置文本数据
// 2006.3.17
void CIniFile::SetData(const CStringArray* AData)
{
    m_aFileContainer.RemoveAll();
    for(int i=0;i<AData->GetSize();i++){
        m_aFileContainer.Add(AData->GetAt(i));
    }
}

// 设置文本数据
// 2006.4.19
void CIniFile::SetTextData(const CString& AValue)
{
    CStringArray aString;
    int iBegin=0,iEnd;
    CString s;
    while (true) {
        iEnd=AValue.Find(Crlf,iBegin);
        if(iEnd<0) break;
        s=AValue.Mid(iBegin,iEnd-iBegin);
        aString.Add(s);
        
        iBegin=iEnd+2;
    }
    int iCount=AValue.GetLength();
    s=AValue.Mid(iBegin,iCount-iBegin);
    if(iBegin<=iCount-3) aString.Add(s);
    // 设置数据
    SetData(&aString);
}
 
// 获得文本数据
// 2006.3.17
CStringArray* CIniFile::GetData()
{
    return &m_aFileContainer;
}

// 获得文本数据
// 2006.3.17
CString CIniFile::GetTextData()
{
    CString sText;
    for(int i=0;i<m_aFileContainer.GetSize();i++){
        sText+=(m_aFileContainer.GetAt(i)+Crlf);
    }
    return sText;
}

// 获得所有分组 2010.10.16
void CIniFile::GetAllGroup(OUT CStringArray* a_aOutGroup)
{
    if(!a_aOutGroup) return;
    CString sLine;
    for(int i=0;i<m_aFileContainer.GetSize();i++){
        sLine=m_aFileContainer.GetAt(i);
        sLine.TrimLeft();
        sLine.TrimRight();
        int iBegin=sLine.Find(_T("["));
        int iEnd  =sLine.Find(_T("]"));
        if(iBegin==0 && iEnd>0){
            a_aOutGroup->Add(sLine.Mid(iBegin+1,iEnd-iBegin-1));
        }
    }
}

// 获得一个分组的节点数据(CIniFileItem*)
void CIniFile::GetGroupItem(IN LPCTSTR a_sGroupName,OUT CItemList* a_aOutItem)
{
    if(!a_aOutItem) return;
    CString sLine,sGroupName,sTemp;
    int iBegin=-1;
    INT_PTR iEnd=-1;
    int i;
    for(i=0;i<m_aFileContainer.GetSize();i++){
        sLine=m_aFileContainer.GetAt(i);
        sLine.TrimLeft();
        sLine.TrimRight();
        // 找到分组
        if(iBegin<0){
            sGroupName.Format(_T("[%s]"),a_sGroupName);
            sGroupName.MakeUpper();
            sTemp=sLine;
            sTemp.MakeUpper();
            if(sTemp.Find(sGroupName)==0){
                iBegin=i;
            }
        }
        else{
            if(sLine.Find(_T("["))==0){
                iEnd=i;
                break;
            }
        }
    }
    // 取出节点数据
    if(iBegin>=0){
        if(iEnd<0) iEnd=m_aFileContainer.GetSize()-1;
        for(i=iBegin+1;i<iEnd;i++){
            CIniFileItem* pItem=new CIniFileItem;
            sLine=m_aFileContainer.GetAt(i);
            int iEqual=sLine.Find(_T("="));
            if(iEqual>0){
                // 禁止非字符的名称前缀
                WORD c=sLine[0];
                if((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>127)){
                    CString sName,sValue;
                    sName   =sLine.Left(iEqual);
                    sValue  =sLine.Right(sLine.GetLength()-iEqual-1);
                    sName.TrimLeft();sName.TrimRight();
                    sValue.TrimLeft();sValue.TrimRight();
                    pItem->SetName(sName);
                    pItem->SetValue(sValue);
                    a_aOutItem->AddItem(pItem);
                }
            }
        }
    }
}

bool CIniFile::GetVar(const CString strSection,const CString strVarName,CString* strReturnValue)
{
    if(m_aFileContainer.GetSize() < 0) return false;
    
    int iLine = SearchLine(strSection,strVarName);
    if(iLine >= 0)
    {
        CString		strParam = m_aFileContainer[iLine -1];
        *strReturnValue = strParam.Mid(strParam.Find(_T("=")) + 1);
        return true;
    }
    //*strReturnValue="";
    return false;
}
bool CIniFile::GetVarStr(const CString strSection,const CString strVarName,CString* strReturnValue)
{
    return(GetVar(strSection,strVarName,strReturnValue));
}

bool CIniFile::GetVarInt(const CString strSection,const CString strVarName,int* iValue)
{
    CString  strReturnVar;
    if(GetVar(strSection,strVarName,&strReturnVar))
    {
        strReturnVar.TrimLeft();
        int iLen = strReturnVar.GetLength();
        *iValue = _tstoi(strReturnVar.GetBuffer(iLen));
        return true;
    }
    *iValue=0;
    return false;
}

bool CIniFile::GetVarDouble(const CString strSection,const CString strVarName,double* iValue)
{
    CString  strReturnVar;
    if(GetVar(strSection,strVarName,&strReturnVar))
    {
        strReturnVar.TrimLeft();
        int iLen = strReturnVar.GetLength();
        *iValue = _tstof(strReturnVar.GetBuffer(iLen));
        return true;
    }
    return false;
}

bool CIniFile::GetVarFloat(const CString strSection,const CString strVarName,float* iValue)
{
    CString  strReturnVar;
    if(GetVar(strSection,strVarName,&strReturnVar))
    {
        strReturnVar.TrimLeft();
        int iLen = strReturnVar.GetLength();
        *iValue = (float)_tstof(strReturnVar.GetBuffer(iLen));
        return true;
    }
    return false;
}

// iType: 如果为 0 表示数据项不存在时也进行添加，否则不添加(2006.3.22)
bool CIniFile::SetVar(const CString strSection,const CString strVarName,CString strVar,const int iType)
{
    if(m_aFileContainer.GetSize() == 0)
    {
        m_aFileContainer.Add(_T("[") + strSection + _T("]"));
        m_aFileContainer.Add(strVarName + _T("=") + strVar);
        return true;
    }
    
    int i = 0;
    INT_PTR iFileLines = m_aFileContainer.GetSize();
    //for(pInterator;pInterator != m_aFileContainer.end();++pInterator)
    //{
    while(i< iFileLines)
    {
        CString		strValue = m_aFileContainer.GetAt(i++);
        strValue.TrimLeft();
        if((strValue.Find(_T("[")) >=0) && (strValue.Find(strSection) >=0))
        {	
            while(i < iFileLines)
            {
                CString strSectionList = m_aFileContainer[i++];
                strSectionList.TrimLeft();
                if(strSectionList.Find(_T("//")) ==0)//找到注释行
                    continue;
                
                CString sTemp;
                int iFindEqual=strSectionList.Find(_T("="));
                if(iFindEqual>=0){
                    sTemp=strSectionList.Left(iFindEqual);
                    sTemp.TrimRight();
                }
                if(sTemp==strVarName)//找到
                {
                    CString strParam = strVarName + _T("=") + strVar;
                    //m_aFileContainer.SetAt(i-1,strParam);
                    m_aFileContainer[i-1] = strParam;
                    return true;
                }
                if(strSectionList.Find(_T("["),0)>=0)//在原来文件的SECTION中,没有相应的变量需要添加而且,这种情况下,下边还有别的section
                {
                    //处理流程是这样的,首先把当前的数值依次向后移动,然后在当前位置加入新出现的数值
                    if(iType !=0)
                        return false;
                    CString strParam;
                    m_aFileContainer.Add(strParam);
                    INT_PTR iPre = m_aFileContainer.GetSize()-1;
                    while(iPre >= i)
                    {
                        CString strBehind = m_aFileContainer[iPre -1];
                        m_aFileContainer[iPre] = strBehind;
                        iPre --;
                    }//*/
                    strParam = strVarName + _T("=") + strVar;
                    m_aFileContainer.SetAt(i-1,strParam);
                    return true;
                }
                if(i == iFileLines && iType == 0)
                {
                    m_aFileContainer.Add(strVarName + _T("=") + strVar);
                    return true;
                }
            }
        }
    }
    if(iType == 0)
    {
        m_aFileContainer.Add(_T("[") + strSection + _T("]"));
        m_aFileContainer.Add(strVarName + _T("=") + strVar);
    }
    return true;
}

bool CIniFile::SetVarStr(const CString strSection,const CString strVarName,CString strValue,const int iType)
{
    return SetVar(strSection,strVarName,strValue,iType);
}

// 设置布尔变量数据 2014.3.11
bool CIniFile::SetBool(const CString strSection,const CString strVarName,const bool bValue,const int iType)
{
    return SetVar(strSection,strVarName,bValue?_T("TRUE"):_T("FALSE"),iType);
}


bool CIniFile::SetVarInt(const CString strSection,const CString strVarName,int iValue,const int iType)
{
    CString strVar;
    strVar.Format(_T("%d"),iValue);
    return (SetVar(strSection,strVarName,strVar,iType));
}

bool CIniFile::SetVarDouble(const CString strSection,const CString strVarName,double iValue,const int iType)
{
    CString strVar;
    strVar.Format(_T("%f"),iValue);
    return (SetVar(strSection,strVarName,strVar,iType));
}

int CIniFile::SearchLine(const CString strSection,const CString & strVarName)
{
    if(m_aFileContainer.GetSize() > 0)
    {
        int i = 0;
        INT_PTR iFileLines = m_aFileContainer.GetSize();
        while(i< iFileLines)
        {
            CString strValue = m_aFileContainer[i++];
            strValue.TrimLeft();
            CString sFind;
            sFind.Format(_T("[%s]"),strSection);
            if(strValue.Find(sFind)==0)
            {	
                while(i < iFileLines)
                {
                    CString strSectionList = m_aFileContainer[i++];
                    strSectionList.TrimLeft();
                    if(strSectionList.GetLength()>0){
                        if(strSectionList.Left(1)=="["){
                            return -1;
                        }
                    }
                    if(strSectionList.Find(_T("//")) ==0)//找到注释行
                        continue;
                    CString sTemp;
                    int iFindEqual=strSectionList.Find(_T("="));
                    if(iFindEqual>=0){
                        sTemp=strSectionList.Left(iFindEqual);
                        sTemp.TrimRight();
                    }
                    if(sTemp==strVarName)//找到
                    {
                        return i;
                    }
                    if(strSectionList.Find(_T("["),0) == 0)//另外一个段落出现,寻找失败
                    {
                        return -2;
                    }
                }
            }
        }
    }
    return -1;
}

// 2006.08.01
CString CIniFile::GetStr(CString ASection,CString AItemName,CString ADefValue)
{
    CString sValue;
    if(GetVarStr(ASection,AItemName,&sValue)){
        return sValue;
    }
    else{
        return ADefValue;
    }
}

// 2006.08.01
int CIniFile::GetInt(CString ASection,CString AItemName,int ADefValue)
{
    int iValue;
    if(GetVarInt(ASection,AItemName,&iValue)){
        return iValue;
    }
    else{
        return ADefValue;
    }
}

// 2006.08.01
double CIniFile::GetDouble(CString ASection,CString AItemName,double ADefValue)
{
    double dblValue;
    if(GetVarDouble(ASection,AItemName,&dblValue)){
        return dblValue;
    }
    else{
        return ADefValue;
    }
}

// 2006.08.01
float CIniFile::GetFloat(CString ASection,CString AItemName,float ADefValue)
{
    float fValue;
    if(GetVarFloat(ASection,AItemName,&fValue)){
        return fValue;
    }
    else{
        return ADefValue;
    }
}

// 2006.08.01
bool CIniFile::GetBool(CString ASection,CString AItemName,bool ADefValue)
{
    CString sValue;
    if(GetVarStr(ASection,AItemName,&sValue)){
        sValue.MakeUpper();
        if(sValue==_T("TRUE")){
            return true;
        }
        else{
            return false;
        }
    }
    return ADefValue;
}

//////////////////////////////////////////////////////////////////////////
// CConfigFile

CConfigFile::CConfigFile()
{
    
}

CConfigFile::~CConfigFile()
{
    
}

// 设置文本
// 2007.8.4
void CConfigFile::SetText(LPCTSTR a_sText)
{
    ExpandStrToArray(a_sText,Crlf,m_aContainer);
}

bool CConfigFile::Load(const CString AFileName)
{
    CStdioFile stfFile;
    bool bFileExsit = false;
    m_aContainer.RemoveAll();
    // 如果文件不存在就创建一个新的文件
    if(!FileExists(AFileName))
    {
        CFile f(AFileName,CFile::modeCreate);
        f.Close();
    }
    
    if(!stfFile.Open(AFileName,CFile::modeRead))
    {
        return bFileExsit;
    }
    CString strFileLine;
    while(TRUE)
    {
        if(!stfFile.ReadString(strFileLine)){
            break;
        }
        
        m_aContainer.Add(strFileLine);
    }
    stfFile.Close();
    bFileExsit = true;
    return bFileExsit;
}

bool CConfigFile::Save(const CString AFileName)
{
    CStdioFile stfFile;
    if(stfFile.Open(AFileName,CFile::modeCreate | CFile::modeWrite) && m_aContainer.GetSize() > 0)
    {
        CString strParam;
        for(int i = 0; i< m_aContainer.GetSize();i++)
        {
            strParam = m_aContainer[i];
            stfFile.WriteString(strParam+_T("\n"));
        }
    }
    stfFile.Close();
    return true;
}

// 保证一个字符串后面中指定子串后面必须是一个 = 号
// 2009.1.9
bool MatchSubString(CString a_sSource,CString a_sSub)
{
    int iLen=a_sSource.GetLength();
    int iSubLen=a_sSub.GetLength();
    if(iLen<=iSubLen) return false;
    int iBegin      =a_sSource.Find(a_sSub,0);
    if(iBegin<0) return false;
    CString sBlank=a_sSource.Mid(0,iBegin);
    sBlank.TrimLeft();
    if(!sBlank.IsEmpty()) return false;
    int iEqualPos   =a_sSource.Find(_T("="),iBegin+iSubLen);
    if(iEqualPos<0) return 0;
    sBlank=a_sSource.Mid(iBegin+iSubLen,iEqualPos-iBegin-iSubLen);
    sBlank.TrimLeft();
    if(!sBlank.IsEmpty()) return false;
    return true;
}

// 获得一个指定节点的值
// 2006.9.16
CString CConfigFile::GetValue(const CString AItem)
{
    CString sFindItem(AItem);
    sFindItem.TrimLeft();
    sFindItem.MakeUpper();

    CString sResult(_T(""));
    INT_PTR iCount=m_aContainer.GetSize();
    for(int i=0;i<iCount;i++){
        CString sItem=m_aContainer.GetAt(i);
        int iItemSize=sItem.GetLength();
        CString sItemUpper(sItem);
        sItemUpper.MakeUpper();
        if(MatchSubString(sItemUpper,sFindItem)){
            int iFindIndex=sItemUpper.Find(_T("="),0);
            if(iFindIndex>=0){
                sResult=sItem.Right(iItemSize-iFindIndex-1);
                break;
            }
        }
    }
    return sResult;
}

// 获得一个指定节点的值
// 2006.9.16
CString CConfigFile::GetValue(const CString AItem,const CString ASign)
{
    CString sFindItem(AItem);
    sFindItem.TrimLeft();
    sFindItem.MakeUpper();
    
    CString sResult(_T(""));
    INT_PTR iCount=m_aContainer.GetSize();
    for(int i=0;i<iCount;i++){
        CString sItem=m_aContainer.GetAt(i);
        int iItemSize=sItem.GetLength();
        CString sItemUpper(sItem);
        sItemUpper.MakeUpper();
        if(sItemUpper.Find(sFindItem)==0){
            int iFindIndex=sItemUpper.Find(ASign,0);
            if(iFindIndex>=0){
                sResult=sItem.Right(iItemSize-iFindIndex-1);
                break;
            }
        }
    }
    return sResult;
}

// 设置一个节点的值
// 2007.5.8
void CConfigFile::SetValue(const CString AItem,const CString AValue)
{
    bool bExist=false;
    CString sNew;
    sNew.Format(_T("%s=%s"),AItem,AValue);
    INT_PTR iCount=m_aContainer.GetSize();
    for(int i=0;i<iCount;i++){
        CString sItem=m_aContainer.GetAt(i);
        int iItemSize=sItem.GetLength();
        CString sItemUpper(sItem);
        // sItemUpper.MakeUpper();
        if(MatchSubString(sItemUpper,AItem)){
            int iFindIndex=sItemUpper.Find(_T("="),0);
            if(iFindIndex>=0){
                m_aContainer.SetAt(i,sNew);
                bExist=true;
                break;
            }
        }
    }
    // 如果不存在则添加
    if(!bExist){
        m_aContainer.Add(sNew);
    }
}

// 获得全部的文本数据
// 2007.7.6
void CConfigFile::GetAllText(CString& AValue)
{
    for(int i=0;i<m_aContainer.GetSize();i++){
        CString s;
        s=m_aContainer.GetAt(i);
        if(i==0){
            AValue+=s;
        }
        else{
            AValue+=(Crlf+s);
        }
    }
}

// 获得指定索引数据 2010.11.15
bool CConfigFile::GetValueOfIndex(const int a_iIndex,OUT CString& a_sName,OUT CString& a_sValue) const
{
    if(a_iIndex>=m_aContainer.GetSize()) return false;
    if(a_iIndex<0) return false;
    CString sLine=m_aContainer.GetAt(a_iIndex);
    a_sName =GetStringSign(sLine,_T("="),1);
    a_sValue=GetStringSign(sLine,_T("="),2);
    return true;
}

// 根据名称删除子节点(支持模糊匹配)，请自行添加包含文件：#include <list>
void DeleteTreePackNodeOfName(CPackTreeBase* a_pPack,LPCTSTR a_sName,bool a_bUseRule){
    list<CPackTreeBase*> aPackList;
    CPackTreeBase* pNode=a_pPack->ChildBegin();
    while(pNode){
        bool bPass;
        if(a_bUseRule) bPass=TextRuleCheckEx(a_sName,pNode->GetName());
        else bPass=(CString(pNode->GetName())).Find(a_sName)>=0;
        if(bPass){
            aPackList.push_back(pNode);
        }
        pNode=a_pPack->ChildNext();
    }
    for(list<CPackTreeBase*>::const_iterator i=aPackList.begin();i!=aPackList.end();i++){
        a_pPack->DeleteChild((*i)->GetName());
    }
}


}//namespace

