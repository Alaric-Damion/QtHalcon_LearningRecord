
#ifndef _SunDatabase_Fun_h
#define _SunDatabase_Fun_h

#include "SunADO.h"
#include "SunDefine.h"
#include <afxmt.h>

namespace sunLib{

// 功能：为 TComboBox 添加项
SUN_DLL_DEFINE bool SetComboBoxItemEx(CADODatabase& a_connect,CComboBox& cobX,CString TableSQL,CString sField1,CString sField2);
// 同步不同数据库数据表数据
SUN_DLL_DEFINE bool SyncTableData(CADODatabase& a_cntSource,CADODatabase& a_cntTarget,CString a_sSourceTable,CString a_sTargetTable,int a_iFieldCount);


/********************************************************************
  功能：多线程写数据库类
  作者：梅文海
  日期：2007
  版本：1.0
  说明：
  使用示例：
  CThreadWriteDatabase twdUpdate;
  twdUpdate.SetConnectText("Provider=SQLOLEDB.1;Password=aaa;User ID=aaa;Initial Catalog=ddd;Data Source=luyserver;Use Procedure=1");
  twdUpdate.Begin();
  twdUpdate.AddSQL("insert into t1(id,name) values(1,'hd')");
*********************************************************************/
class SUN_DLL_DEFINE CThreadWriteDatabase
{
private:
    CString         m_sConnect;         // 数据库链接字符串
    CSemaphore      m_spLock;           // 信号量同步对象

    CStringArray    m_aExecuteSQL;      // 操作数据库的 SQL 语句
    bool            m_bStop;            // 是否停止线程
    int             m_iInternal;        // 执行 SQL 语句的间隔时间

    CADODatabase    m_adoConnect;       // 数据库操作类
    CWinThread*     m_pThread;

    static UINT Execute(LPVOID pParam);     // 线程函数
    // 执行 SQL 语句
    bool ExecuteSQL(CString& ASQL);         // 执行 SQL 语句
public:
	CThreadWriteDatabase();
	virtual ~CThreadWriteDatabase();

    // 设置数据库链接字符串
    bool SetConnectText(CString& AConnect){
        m_sConnect=AConnect;
        m_adoConnect.SetConnectionString(AConnect);
        return m_adoConnect.Open()?true:false;
    };
    // 获得缓冲区数据的条数
    int GetSQLCount(){
        int iSize=0;
        m_spLock.Lock();
        iSize=(int)m_aExecuteSQL.GetSize();
        m_spLock.Unlock();
        return iSize;
    };
    // 设置执行 SQL 语句的间隔时间(ms 默认为 50ms)
    void SetInternalTime(int AInternal){m_iInternal=AInternal;};

    // 添加一条 SQL 语句到缓冲区
    void AddSQL(CString& ASQL){
        m_spLock.Lock();
        m_aExecuteSQL.Add(ASQL);
        m_spLock.Unlock();
    };
    // 开始线程
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