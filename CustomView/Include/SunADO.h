
#ifndef _ADO_H_
#define _ADO_H_

#if _MSC_VER >= 1000
#pragma once
#endif 
#include <afx.h>
#include <afxdisp.h>
#include <math.h>
#include "SunDefine.h"

#pragma warning (disable: 4146)
#import "c:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "adoEOF")
//using namespace ADOCG;


#include "icrsint.h"

struct CADOFieldInfo
{
    TCHAR m_strName[255]; 
    short m_nType;
    long m_lSize; 
    long m_lDefinedSize;
    long m_lAttributes;
    short m_nOrdinalPosition;
    BOOL m_bRequired;   
    BOOL m_bAllowZeroLength; 
    long m_lCollatingOrder;  
};

//CString IntToStr(int nVal);

SUN_DLL_DEFINE CString LongToStr(long lVal);

SUN_DLL_DEFINE CString ULongToStr(unsigned long ulVal);

SUN_DLL_DEFINE CString DblToStr(double dblVal, int ndigits = 20);

SUN_DLL_DEFINE CString DblToStr(float fltVal);


class SUN_DLL_DEFINE CADODatabase
{
public:
    CADODatabase();
    
    ~CADODatabase()
    {
        Close();
        if(m_pConnection){
        m_pConnection.Release();
        m_pConnection = NULL;
        }
        m_strConnection = _T("");
        m_strLastError = _T("");
        m_dwLastError = 0;
        ::CoUninitialize();
    }
    
    BOOL Open(LPCTSTR lpstrConnection = _T(""), LPCTSTR lpstrUserID = _T(""), LPCTSTR lpstrPassword = _T(""));
    _ConnectionPtr GetActiveConnection() 
    {return m_pConnection;};
    BOOL ExecuteSQL(LPCTSTR strSQL);
    DWORD GetRecordCount(_RecordsetPtr m_pRs);
    long BeginTransaction() 
    {return m_pConnection->BeginTrans();};
    long CommitTransaction() 
    {return m_pConnection->CommitTrans();};
    long RollbackTransaction() 
    {return m_pConnection->RollbackTrans();};
    BOOL IsOpen();
    void Close();
    void SetConnectionString(LPCTSTR lpstrConnection)
    {m_strConnection = lpstrConnection;};
    CString GetConnectionString()
    {return m_strConnection;};
    CString GetLastErrorString() 
    {return m_strLastError;};
    DWORD GetLastError()
    {return m_dwLastError;};
    void SetConnectionTimeout(long nConnectionTimeout = 30)
    {m_nConnectionTimeout = nConnectionTimeout;};
    
protected:
    static bool m_bInitOle;             // 是否初始化过
    static void InitOle();              // 初始化Ole
    void dump_com_error(_com_error &e);
    
public:
    _ConnectionPtr m_pConnection;
    
protected:
    CString m_strConnection;
    CString m_strLastError;
    DWORD m_dwLastError;
    long m_nConnectionTimeout;
};

class SUN_DLL_DEFINE CADORecordset
{
public:
    enum cadoOpenEnum
    {
        openUnknown = 0,
        openQuery = 1,
        openTable = 2,
        openStoredProc = 3
    };
    
    enum cadoEditEnum
    {
        dbEditNone = 0,
        dbEditNew = 1,
        dbEdit = 2
    };
    
    enum cadoPositionEnum
    {
        
        positionUnknown = -1,
        positionBOF = -2,
        positionEOF = -3
    };
    
    enum cadoSearchEnum
    {	
        searchForward = 1,
        searchBackward = -1
    };
    
    enum cadoDataType
    {
        typeEmpty = adEmpty,
        typeTinyInt = adTinyInt,
        typeSmallInt = adSmallInt,
        typeInteger = adInteger,
        typeBigInt = adBigInt,
        typeUnsignedTinyInt = adUnsignedTinyInt,
        typeUnsignedSmallInt = adUnsignedSmallInt,
        typeUnsignedInt = adUnsignedInt,
        typeUnsignedBigInt = adUnsignedBigInt,
        typeSingle = adSingle,
        typeDouble = adDouble,
        typeCurrency = adCurrency,
        typeDecimal = adDecimal,
        typeNumeric = adNumeric,
        typeBoolean = adBoolean,
        typeError = adError,
        typeUserDefined = adUserDefined,
        typeVariant = adVariant,
        typeIDispatch = adIDispatch,
        typeIUnknown = adIUnknown,
        typeGUID = adGUID,
        typeDate = adDate,
        typeDBDate = adDBDate,
        typeDBTime = adDBTime,
        typeDBTimeStamp = adDBTimeStamp,
        typeBSTR = adBSTR,
        typeChar = adChar,
        typeVarChar = adVarChar,
        typeLongVarChar = adLongVarChar,
        typeWChar = adWChar,
        typeVarWChar = adVarWChar,
        typeLongVarWChar = adLongVarWChar,
        typeBinary = adBinary,
        typeVarBinary = adVarBinary,
        typeLongVarBinary = adLongVarBinary,
        typeChapter = adChapter,
        typeFileTime = adFileTime,
        typePropVariant = adPropVariant,
        typeVarNumeric = adVarNumeric,
        typeArray = adVariant
    };
    
    BOOL SetFieldValue(int nIndex, int nValue);
    BOOL SetFieldValue(LPCTSTR lpFieldName, int nValue);
    BOOL SetFieldValue(int nIndex, long lValue);
    BOOL SetFieldValue(LPCTSTR lpFieldName, long lValue);
    BOOL SetFieldValue(int nIndex, unsigned long lValue);
    BOOL SetFieldValue(LPCTSTR lpFieldName, unsigned long lValue);
    BOOL SetFieldValue(int nIndex, double dblValue);
    BOOL SetFieldValue(LPCTSTR lpFieldName, double dblValue);
    BOOL SetFieldValue(int nIndex, CString strValue);
    BOOL SetFieldValue(LPCTSTR lpFieldName, CString strValue);
    BOOL SetFieldValue(int nIndex, COleDateTime time);
    BOOL SetFieldValue(LPCTSTR lpFieldName, COleDateTime time);
    BOOL SetFieldValue(int nIndex, bool bValue);
    BOOL SetFieldValue(LPCTSTR lpFieldName, bool bValue);
    BOOL SetFieldValue(int nIndex, COleCurrency cyValue);
    BOOL SetFieldValue(LPCTSTR lpFieldName, COleCurrency cyValue);
    BOOL SetFieldValue(int nIndex, _variant_t vtValue);
    BOOL SetFieldValue(LPCTSTR lpFieldName, _variant_t vtValue);
    
    BOOL Update();
    void Edit();
    BOOL AddNew();
    
    CADORecordset();
    
    CADORecordset(CADODatabase* pAdoDatabase);
    
    ~CADORecordset()
    {
        Close();
        if(m_pRecordset)
            m_pRecordset.Release();
        if(m_pCmd)
            m_pCmd.Release();
        m_pRecordset = NULL;
        m_pCmd = NULL;
        m_strQuery = _T("");
        m_strLastError = _T("");
        m_dwLastError = 0;
        m_nEditStatus = dbEditNone;
    }
    
    CString GetQuery() 
    {return m_strQuery;};
    void SetQuery(LPCTSTR strQuery) 
    {m_strQuery = strQuery;};
    DWORD GetRecordCount();
    BOOL IsFieldEmpty(LPCTSTR lpFieldName);
    BOOL IsOpen();
    void Close();
    BOOL Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec = _T(""), int nOption = CADORecordset::openUnknown);
    BOOL Open(LPCTSTR lpstrExec = _T(""), int nOption = CADORecordset::openUnknown);
    BOOL GetFieldValue(LPCTSTR lpFieldName, int& nValue);
    BOOL GetFieldValue(int nIndex, int& nValue);
    BOOL GetFieldValue(LPCTSTR lpFieldName, long& lValue);
    BOOL GetFieldValue(int nIndex, long& lValue);
    BOOL GetFieldValue(LPCTSTR lpFieldName, unsigned long& ulValue);
    BOOL GetFieldValue(int nIndex, unsigned long& ulValue);
    BOOL GetFieldValue(LPCTSTR lpFieldName, double& dbValue);
    BOOL GetFieldValue(int nIndex, double& dbValue);
    BOOL GetFieldValue(LPCTSTR lpFieldName, CString& strValue, CString strDateFormat = _T(""));
    BOOL GetFieldValue(int nIndex, CString& strValue, CString strDateFormat = _T(""));
    BOOL GetFieldValue(LPCTSTR lpFieldName, COleDateTime& time);
    BOOL GetFieldValue(int nIndex, COleDateTime& time);
    BOOL GetFieldValue(int nIndex, bool& bValue);
    BOOL GetFieldValue(LPCTSTR lpFieldName, bool& bValue);
    BOOL GetFieldValue(int nIndex, COleCurrency& cyValue);
    BOOL GetFieldValue(LPCTSTR lpFieldName, COleCurrency& cyValue);
    BOOL GetFieldValue(int nIndex, _variant_t& vtValue);
    BOOL GetFieldValue(LPCTSTR lpFieldName, _variant_t& vtValue);
    _variant_t GetFieldByName(CString AField);                      // 获得字段数据
    
    BOOL IsEof()
    {return m_pRecordset->adoEOF == VARIANT_TRUE;};
    BOOL IsEOF()
    {return m_pRecordset->adoEOF == VARIANT_TRUE;};
    BOOL IsBof()
    {return m_pRecordset->BOF == VARIANT_TRUE;};
    BOOL IsBOF()
    {return m_pRecordset->BOF == VARIANT_TRUE;};
    void MoveFirst() 
    {m_pRecordset->MoveFirst();};
    void MoveNext() 
    {m_pRecordset->MoveNext();};
    void MovePrevious() 
    {m_pRecordset->MovePrevious();};
    void MoveLast() 
    {m_pRecordset->MoveLast();};
    BOOL GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo);
    BOOL GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo);
    
    CString GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows = 0);
    CString GetLastErrorString() 
    {return m_strLastError;};
    DWORD GetLastError()
    {return m_dwLastError;};
    BOOL Delete();
    BOOL IsConnectionOpen()
    {return m_pConnection != NULL && m_pConnection->GetState() != adStateClosed;};
    _RecordsetPtr GetRecordset()
    {return m_pRecordset;};
    _ConnectionPtr GetActiveConnection() 
    {return m_pConnection;};
    
public:
    _RecordsetPtr m_pRecordset;
    _CommandPtr m_pCmd;
    
protected:
    _ConnectionPtr m_pConnection;
    int m_nEditStatus;
    CString m_strLastError;
    DWORD m_dwLastError;
    void dump_com_error(_com_error &e);
    CString m_strQuery;
    
protected:
    BOOL PutFieldValue(LPCTSTR lpFieldName, _variant_t vtFld);
    BOOL PutFieldValue(_variant_t vtIndex, _variant_t vtFld);
    BOOL GetFieldInfo(FieldPtr pField, CADOFieldInfo* fldInfo);
    BOOL FindFirst(LPCTSTR lpFind);	
};
#endif
