
#ifndef _SunDatabase_Fun_h
#define _SunDatabase_Fun_h

#include "SunADO.h"
#include "SunDefine.h"
#include <afxmt.h>

namespace sunLib{

// ���ܣ�Ϊ TComboBox �����
SUN_DLL_DEFINE bool SetComboBoxItemEx(CADODatabase& a_connect,CComboBox& cobX,CString TableSQL,CString sField1,CString sField2);
// ͬ����ͬ���ݿ����ݱ�����
SUN_DLL_DEFINE bool SyncTableData(CADODatabase& a_cntSource,CADODatabase& a_cntTarget,CString a_sSourceTable,CString a_sTargetTable,int a_iFieldCount);


/********************************************************************
  ���ܣ����߳�д���ݿ���
  ���ߣ�÷�ĺ�
  ���ڣ�2007
  �汾��1.0
  ˵����
  ʹ��ʾ����
  CThreadWriteDatabase twdUpdate;
  twdUpdate.SetConnectText("Provider=SQLOLEDB.1;Password=aaa;User ID=aaa;Initial Catalog=ddd;Data Source=luyserver;Use Procedure=1");
  twdUpdate.Begin();
  twdUpdate.AddSQL("insert into t1(id,name) values(1,'hd')");
*********************************************************************/
class SUN_DLL_DEFINE CThreadWriteDatabase
{
private:
    CString         m_sConnect;         // ���ݿ������ַ���
    CSemaphore      m_spLock;           // �ź���ͬ������

    CStringArray    m_aExecuteSQL;      // �������ݿ�� SQL ���
    bool            m_bStop;            // �Ƿ�ֹͣ�߳�
    int             m_iInternal;        // ִ�� SQL ���ļ��ʱ��

    CADODatabase    m_adoConnect;       // ���ݿ������
    CWinThread*     m_pThread;

    static UINT Execute(LPVOID pParam);     // �̺߳���
    // ִ�� SQL ���
    bool ExecuteSQL(CString& ASQL);         // ִ�� SQL ���
public:
	CThreadWriteDatabase();
	virtual ~CThreadWriteDatabase();

    // �������ݿ������ַ���
    bool SetConnectText(CString& AConnect){
        m_sConnect=AConnect;
        m_adoConnect.SetConnectionString(AConnect);
        return m_adoConnect.Open()?true:false;
    };
    // ��û��������ݵ�����
    int GetSQLCount(){
        int iSize=0;
        m_spLock.Lock();
        iSize=(int)m_aExecuteSQL.GetSize();
        m_spLock.Unlock();
        return iSize;
    };
    // ����ִ�� SQL ���ļ��ʱ��(ms Ĭ��Ϊ 50ms)
    void SetInternalTime(int AInternal){m_iInternal=AInternal;};

    // ���һ�� SQL ��䵽������
    void AddSQL(CString& ASQL){
        m_spLock.Lock();
        m_aExecuteSQL.Add(ASQL);
        m_spLock.Unlock();
    };
    // ��ʼ�߳�
    void Begin(){m_bStop=false;
        m_pThread=AfxBeginThread(Execute,this,THREAD_PRIORITY_BELOW_NORMAL,0,0,NULL);
    };
    void Stop(){
        m_bStop=true;
        if(m_pThread){
            WaitForSingleObject(m_pThread->m_hThread,1000);
        }
    };
};

}//namespace

#endif