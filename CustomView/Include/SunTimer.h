
/********************************************************************
  功能：时间处理库
  作者：梅文海
  日期：2003
  版本：1.0
  说明：
*********************************************************************/

#ifndef _sun_timer_h
#define _sun_timer_h

#include "SunDefine.h"
#include "SunItemList.h"
#include "SunPublic.h"

namespace sunLib{

class SUN_DLL_DEFINE CProTimer
{
public:
    CProTimer();
    void Reset();
    double GetTime(bool reset);
    
protected:
    LONGLONG m_CounterFrequency;
    LONGLONG m_ResetTime;
};
    
// 触发定时器消息 2018.9.14
class CTimerEventInterface{
public:
    virtual void TimeEvent(int a_iTimerID,void* a_pUserData)=NULL;
};

class CTimerEventInfo:public CIDItem{
public:
    virtual bool Assign(const CIDItemBase& AOther){         // 拷贝函数(不能设为纯虚方法，否则 ID 不能被设值)
        CIDItem::Assign(AOther);
        CTimerEventInfo* pOther=(CTimerEventInfo*)&AOther;
        pUserData       =pOther->pUserData;
        iDelay          =pOther->iDelay;
        return true;        
    }
    virtual CIDItemBase* CreateInstance(){return new CTimerEventInfo;};         // 创建该类的实例
    virtual LPCTSTR GetClassName(){GET_ITEM_CLASS_NAME(CTimerEventInfo);};  // 获得类名
    virtual bool IfOverTime(){                                              // 是否已经超时
        return (m_ptTimer.GetTime(false)*1000>=iDelay);
    }
    virtual void Reset(){m_ptTimer.Reset();}                                // 重置时间

    void*       pUserData;      // 参数
    int         iDelay;         // 延迟时间（单位是毫秒）
protected:
    CProTimer   m_ptTimer;      // 计算时间
};

// 高精度定时器 2018.9.14
// 使用方法：
// 1.声明 CSunTimerEvent
// 2.从 CTimerEventInterface 派生一个类，并且实现消息触发函数 TimeEvent
// 3.调用 CSunTimerEvent 的 Init 进行初始化
// 4.添加定时器 AddEvent ，延时的单位为毫秒，返回值为定时器编号，用于删除定时器
// 5.删除定时器 DeleteEvent
class CSunTimerEvent:public CSunThread
{
public:
    CSunTimerEvent(void);
    ~CSunTimerEvent(void);

    virtual void Init(CTimerEventInterface* a_pTimeEventInterface){m_pEventCallback=a_pTimeEventInterface;};
    virtual bool SunExecute(void* a_pOwner);
    virtual int AddEvent(int a_iDelay,void* a_pUserData);
    virtual void DeleteEvent(int a_iEventID);

    CItemList       m_aEvent;
    CSemaphore      m_lock;
    CTimerEventInterface* m_pEventCallback; // 事件回调
};   

// ******************************* 时间处理相关 *******************************
SUN_DLL_DEFINE INT64 GetSysTickCount64();
// 转换时间为 SQL 格式
SUN_DLL_DEFINE enum TIME_STR_TYPE{tstNone,tstSQL,tstAccess,tstOracle};
SUN_DLL_DEFINE CString GetSQLTime(CTime ATime,TIME_STR_TYPE AFlag=tstNone);
SUN_DLL_DEFINE CString GetSQLTime(COleDateTime ATime,TIME_STR_TYPE AFlag=tstNone);
// 获得８位数据的日期
SUN_DLL_DEFINE CString Get8DigitalDate(COleDateTime ATime);
// 字符串转换为日期
SUN_DLL_DEFINE SYSTEMTIME StringToTime(CString ATime);
SUN_DLL_DEFINE COleDateTime OleStringToTime(CString ATime);
SUN_DLL_DEFINE CTime StringToTime2(CString ATime);


}
#endif
