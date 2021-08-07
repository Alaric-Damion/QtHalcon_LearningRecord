
#include "stdafx.h"
#include "SunTimer.h"

namespace sunLib{
    
CProTimer::CProTimer()
{
    QueryPerformanceFrequency((LARGE_INTEGER *) &m_CounterFrequency);
    Reset();
}

//##ModelId=43CF40B60280
void CProTimer::Reset()
{
    QueryPerformanceCounter((LARGE_INTEGER *) &m_ResetTime);
}

//##ModelId=43CF40B60281
double CProTimer::GetTime(bool reset)
{
    // Get current time
    LONGLONG currentTime;
    QueryPerformanceCounter((LARGE_INTEGER *) &currentTime);
    
    // Compute difference from last reset
    LONGLONG diffTime = currentTime - m_ResetTime;
    
    // Reset time
    if (reset)
        m_ResetTime = currentTime;
    
    // Return time in seconds
    return (double) diffTime / (double) m_CounterFrequency;
}

CSunTimerEvent::CSunTimerEvent(void)
{
    m_pEventCallback=NULL;
}


CSunTimerEvent::~CSunTimerEvent(void)
{
}

bool CSunTimerEvent::SunExecute(void* a_pOwner){
    while(true){
        int iCount=0;
        m_lock.Lock();
        iCount=m_aEvent.GetItemCount();
        m_lock.Unlock();
        if(iCount>0){
            CItemList aEvent;
            m_lock.Lock();
            for(int i=0;i<m_aEvent.GetItemCount();i++){
                CTimerEventInfo* pEventInfo=(CTimerEventInfo*)m_aEvent.GetValue(i);
                if(pEventInfo->IfOverTime()){
                    CTimerEventInfo* pNewEvent=new CTimerEventInfo;
                    aEvent.AddItem(pNewEvent);
                    pNewEvent->Assign(*pEventInfo);
                    pEventInfo->Reset();
                }
            }
            m_lock.Unlock();
            if(m_pEventCallback){
                for(int i=aEvent.GetItemCount()-1;i>=0;i--){
                    CTimerEventInfo* pEventInfo=(CTimerEventInfo*)aEvent.GetValue(i);
                    m_pEventCallback->TimeEvent(pEventInfo->GetID(),pEventInfo->pUserData);
                }
            }
        }
        else{
            break;
        }
        Sleep(1);
    }
    return true;
}

int CSunTimerEvent::AddEvent(int a_iDelay,void* a_pUserData){
    int iCount=0;
    m_lock.Lock();
    iCount=m_aEvent.GetItemCount();
    m_lock.Unlock();
    if(iCount==0){
        SunCreateThread();
    }

    CTimerEventInfo* pEventInfo=new CTimerEventInfo;
    pEventInfo->pUserData   =a_pUserData;
    pEventInfo->iDelay      =a_iDelay;
    int iTimerID=0;
    m_lock.Lock();
    m_aEvent.AddItem(pEventInfo);
    m_lock.Unlock();
    iTimerID=pEventInfo->GetID();
    return iTimerID;
}

void CSunTimerEvent::DeleteEvent(int a_iEventID){
    m_lock.Lock();
    m_aEvent.DelItemOfID(a_iEventID);
    m_lock.Unlock();
}

// 获得开机以来的毫秒数
// 2010.3.31
INT64 GetSysTickCount64()
{
    static LARGE_INTEGER TicksPerSecond = {0};
    LARGE_INTEGER Tick; 

    if(!TicksPerSecond.QuadPart)
    {
        QueryPerformanceFrequency(&TicksPerSecond);
    } 

    QueryPerformanceCounter(&Tick); 

    INT64 Seconds = Tick.QuadPart/TicksPerSecond.QuadPart;
    INT64 LeftPart = Tick.QuadPart - (TicksPerSecond.QuadPart*Seconds);
    INT64 MillSeconds = LeftPart*1000/TicksPerSecond.QuadPart;
    INT64 Ret = Seconds*1000+MillSeconds;
    _ASSERT(Ret>0);
    return Ret;
}

// 转换时间为 SQL 格式
// AAccessFlag     : 是否使用 Access 的格式，如果是则加上 # 号
// 2004.11.11
CString GetSQLTime(CTime ATime,TIME_STR_TYPE AFlag)
{
    CString s(ATime.Format("%Y-%m-%d %H:%M:%S"));
    
    switch(AFlag) {
    case tstNone:
        break;
    case tstSQL:
        s=_T("'") + s + _T("'");
        break;
    case tstAccess:
        s=_T("#") + s + _T("#");
        break;
    case tstOracle:
        s=_T("TO_DATE('")+s+_T("','yyyy-mm-dd hh24:mi:ss')");
        break;
    default:
        {}
    }
    return s;
}

CString GetSQLTime(COleDateTime ATime,TIME_STR_TYPE AFlag)
{
    CString s(ATime.Format(_T("%Y-%m-%d %H:%M:%S")));
    
    switch(AFlag) {
    case tstNone:
        break;
    case tstSQL:
        s=_T("'") + s + _T("'");
        break;
    case tstAccess:
        s=_T("#") + s + _T("#");
        break;
    case tstOracle:
        s=_T("TO_DATE('")+s+_T("','yyyy-mm-dd hh24:mi:ss')");
        break;
    default:
        {}
    }
    return s;
}

// 获得８位数据的日期
CString Get8DigitalDate(COleDateTime ATime)
{
    CString sResult;
    sResult.Format(_T("%d%.2d%.2d"),ATime.GetYear(),ATime.GetMonth(),ATime.GetDay());
    return sResult;
}

// 字符串转换为日期
SYSTEMTIME StringToTime(CString ATime)
{
    COleDateTime t;
    t.ParseDateTime(ATime);
    SYSTEMTIME tt;
    tt.wYear=(WORD)t.GetYear();
    tt.wMonth=(WORD)t.GetMonth();
    tt.wDay=(WORD)t.GetDay();
    tt.wHour=(WORD)t.GetHour();
    tt.wMinute=(WORD)t.GetMinute();
    tt.wSecond=(WORD)t.GetSecond();
    return tt;
}

CTime StringToTime2(CString ATime)
{
    COleDateTime tt;
    if(tt.ParseDateTime(ATime)){
        CTime t1(tt.GetYear(),tt.GetMonth(),tt.GetDay(),tt.GetHour(),tt.GetMinute(),tt.GetSecond());
        return t1;
    }
    CTime t1(0,0,0,0,0,0);
    return t1;
}

COleDateTime OleStringToTime(CString ATime)
{
    COleDateTime t;
    t.ParseDateTime(ATime);
    return t;
}
    
}//namespace