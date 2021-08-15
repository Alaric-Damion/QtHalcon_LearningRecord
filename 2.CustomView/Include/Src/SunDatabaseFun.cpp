
#include "stdafx.h"
#include "SunDatabaseFun.h"

namespace sunLib{

/////////////////////////////////////////////////////////
// 功能：为 TComboBox 添加项
// 参数：
// cobX:    组合框
// TableSQL:表名或SQL语句（当字串包含'SELECT'时为SQL语句）
// sField1:字段１
// sField2:字段２
// 返回值：是否成功
// 日期：2002/2/25
/////////////////////////////////////////////////////////
bool SetComboBoxItemEx(CADODatabase& a_connect,CComboBox& cobX,CString TableSQL,CString sField1,CString sField2)
{
    CADORecordset rs;
    CString sSql;
    bool bResult=true;
    cobX.Clear();
    cobX.AddString(_T(""));
    CString sUpperSQL(TableSQL);
    sUpperSQL.MakeUpper();
    if(sUpperSQL.Left(6)!=_T("SELECT")){
        sSql.Format(_T("SELECT %s,%s FROM %s\r\n"),sField1,sField2,TableSQL);
        sSql+=_T("ORDER BY ")+sField1+Crlf;
    }
    else{
        sSql=TableSQL;
    }
    rs.Open(a_connect.GetActiveConnection(),sSql);
    while(!rs.IsEof()){
        CString sF1,sF2;
        rs.GetFieldValue(sField1,sF1);
        rs.GetFieldValue(sField2,sF2);
        cobX.AddString(sF1+" "+sF2);
        rs.MoveNext();
    }
    rs.Close();
    return bResult;
}

// 同步不同数据库数据表数据
// a_sSourceTable: 源表
// a_sTargetTable: 目的表
// a_iFieldCount:  需要复制的字段个数
// 2007.7.20
bool SyncTableData(CADODatabase& a_cntSource,CADODatabase& a_cntTarget,CString a_sSourceTable,CString a_sTargetTable,int a_iFieldCount)
{
    CString sSql;
    sSql=_T("delete from ")+a_sTargetTable;
    CADORecordset rsSource,rsTarget;
    if(!a_cntTarget.ExecuteSQL(sSql)) return false;

    sSql=_T("SELECT * FROM ")+a_sSourceTable;
    if(!rsSource.Open(a_cntSource.GetActiveConnection(),sSql)) return false;
    sSql=_T("SELECT * FROM ")+a_sTargetTable;
    if(!rsTarget.Open(a_cntTarget.GetActiveConnection(),sSql)) return false;

    while (!rsSource.IsEof()) {
        rsTarget.AddNew();
        for(int i=0;i<a_iFieldCount;i++){
            CString sFieldValue;
            if(rsSource.GetFieldValue(i,sFieldValue)){
                rsTarget.SetFieldValue(i,sFieldValue);
            }
        }
        rsTarget.Update();
        rsSource.MoveNext();
    }
    return true;
}

CThreadWriteDatabase::CThreadWriteDatabase()
{
    m_bStop =true;
    m_sConnect  ="";
    m_iInternal =50;
    m_pThread   =NULL;
}

CThreadWriteDatabase::~CThreadWriteDatabase()
{
    Stop();
    m_adoConnect.Close();
}

// 执行 SQL 语句
// 2007.3.6
bool CThreadWriteDatabase::ExecuteSQL(CString& ASQL)
{
    return m_adoConnect.ExecuteSQL(ASQL)?true:false;
}

UINT CThreadWriteDatabase::Execute(LPVOID pParam)
{
    CThreadWriteDatabase* me=(CThreadWriteDatabase*)pParam;
    while (!me->m_bStop) {
        CString sSql;
        // 检查缓冲区，是否还有未执行的 SQL 语句
        me->m_spLock.Lock();
        if(me->m_aExecuteSQL.GetSize()>0){
            sSql=me->m_aExecuteSQL.GetAt(0);
        }
        me->m_spLock.Unlock();

        if(me->ExecuteSQL(sSql)){
            me->m_spLock.Lock();
            me->m_aExecuteSQL.RemoveAt(0);
            me->m_spLock.Unlock();
        }
        else{
            me->m_adoConnect.Close();
            me->m_adoConnect.Open(me->m_sConnect);
        }

        Sleep(me->m_iInternal);
    }
    me->m_pThread=NULL;
    return 0;
}

}//namespace