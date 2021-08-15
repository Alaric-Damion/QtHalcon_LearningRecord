
/********************************************************************
  ���ܣ�ʱ�䴦���
  ���ߣ�÷�ĺ�
  ���ڣ�2003
  �汾��1.0
  ˵����
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
    
// ������ʱ����Ϣ 2018.9.14
class CTimerEventInterface{
public:
    virtual void TimeEvent(int a_iTimerID,void* a_pUserData)=NULL;
};

class CTimerEventInfo:public CIDItem{
public:
    virtual bool Assign(const CIDItemBase& AOther){         // ��������(������Ϊ���鷽�������� ID ���ܱ���ֵ)
        CIDItem::Assign(AOther);
        CTimerEventInfo* pOther=(CTimerEventInfo*)&AOther;
        pUserData       =pOther->pUserData;
        iDelay          =pOther->iDelay;
        return true;        
    }
    virtual CIDItemBase* CreateInstance(){return new CTimerEventInfo;};         // ���������ʵ��
    virtual LPCTSTR GetClassName(){GET_ITEM_CLASS_NAME(CTimerEventInfo);};  // �������
    virtual bool IfOverTime(){                                              // �Ƿ��Ѿ���ʱ
        return (m_ptTimer.GetTime(false)*1000>=iDelay);
    }
    virtual void Reset(){m_ptTimer.Reset();}                                // ����ʱ��

    void*       pUserData;      // ����
    int         iDelay;         // �ӳ�ʱ�䣨��λ�Ǻ��룩
protected:
    CProTimer   m_ptTimer;      // ����ʱ��
};

// �߾��ȶ�ʱ�� 2018.9.14
// ʹ�÷�����
// 1.���� CSunTimerEvent
// 2.�� CTimerEventInterface ����һ���࣬����ʵ����Ϣ�������� TimeEvent
// 3.���� CSunTimerEvent �� Init ���г�ʼ��
// 4.��Ӷ�ʱ�� AddEvent ����ʱ�ĵ�λΪ���룬����ֵΪ��ʱ����ţ�����ɾ����ʱ��
// 5.ɾ����ʱ�� DeleteEvent
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
    CTimerEventInterface* m_pEventCallback; // �¼��ص�
};   

// ******************************* ʱ�䴦����� *******************************
SUN_DLL_DEFINE INT64 GetSysTickCount64();
// ת��ʱ��Ϊ SQL ��ʽ
SUN_DLL_DEFINE enum TIME_STR_TYPE{tstNone,tstSQL,tstAccess,tstOracle};
SUN_DLL_DEFINE CString GetSQLTime(CTime ATime,TIME_STR_TYPE AFlag=tstNone);
SUN_DLL_DEFINE CString GetSQLTime(COleDateTime ATime,TIME_STR_TYPE AFlag=tstNone);
// ��ã�λ���ݵ�����
SUN_DLL_DEFINE CString Get8DigitalDate(COleDateTime ATime);
// �ַ���ת��Ϊ����
SUN_DLL_DEFINE SYSTEMTIME StringToTime(CString ATime);
SUN_DLL_DEFINE COleDateTime OleStringToTime(CString ATime);
SUN_DLL_DEFINE CTime StringToTime2(CString ATime);


}
#endif
