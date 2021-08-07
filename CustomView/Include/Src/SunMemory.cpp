

#include "stdafx.h"
#include "SunMemory.h"


namespace sunLib{
    

SunMemoryStream::SunMemoryStream()
{
    m_pBuf  =NULL;
    m_iSize =0;
    m_iPos  =0;
    m_pCallbackOwner    =NULL;
    m_pStatusCallback   =NULL;
}

SunMemoryStream::SunMemoryStream(__int64 a_iMemSize)
{
    m_pBuf  =new byte[a_iMemSize];
    m_iSize =a_iMemSize;
    m_iPos  =0;
    m_pCallbackOwner    =NULL;
    m_pStatusCallback   =NULL;
}

SunMemoryStream::~SunMemoryStream()
{
    if(m_pBuf){
        delete m_pBuf;
        m_pBuf=NULL;
    }
}

void SunMemoryStream::Init(byte a_iValue)
{
    if(m_iSize>0){
        memset(m_pBuf,a_iValue,m_iSize);
    }
}

__int64 SunMemoryStream::Read(void* a_pBuf,__int64 a_iCount)
{
    if(a_iCount<=0) return 0;
    __int64 iRead;
    if(a_iCount+m_iPos>m_iSize) iRead=m_iSize-m_iPos;
    else                        iRead=a_iCount;
    memcpy(a_pBuf,m_pBuf+m_iPos,iRead);
    m_iPos+=iRead;
    return iRead;
}

void SunMemoryStream::Write(const void* a_pBuf,__int64 a_iCount)
{
    memcpy(m_pBuf+m_iPos,a_pBuf,a_iCount);
    m_iPos+=a_iCount;
}

BOOL SunMemoryStream::Seek(__int64 a_iOffset,UINT a_iFrom)
{
    switch(a_iFrom) {
    case begin:
        m_iPos=0;
    	break;
    case current:
        m_iPos+=a_iOffset;
    	break;
    case end:
        m_iPos=m_iSize;
        break;
    default:;
    }
    return TRUE;
}

__int64 SunMemoryStream::SeekToEnd()
{
    m_iPos=m_iSize;
    return m_iPos;
}

// 重设内存流大小
// a_bClear: 是否清除原始数据，否则保留原始数据
// 2009.3.6
BOOL SunMemoryStream::SetSize(__int64 a_iSize,BOOL a_bClear)
{
    if(a_iSize<=0) return FALSE;
    if(a_bClear || m_iSize<=0){
        if(m_pBuf){
            delete m_pBuf;
        }
        m_pBuf=new byte[a_iSize];
    }
    else{   // 如果 m_iSize 大于 0 则一定分配了空间
        byte* pBack=new byte[m_iSize];
        memcpy(pBack,m_pBuf,m_iSize);
        delete m_pBuf;
        m_pBuf=new byte[a_iSize];
        __int64 iCopySize;
        if(a_iSize>m_iSize) iCopySize=m_iSize;
        else                iCopySize=a_iSize;
        memcpy(m_pBuf,pBack,iCopySize);
        delete pBack;
    }
    m_iPos=0;
    m_iSize=a_iSize;
    return TRUE;
}

// 保存数据到文件
BOOL SunMemoryStream::SaveToFile(LPCTSTR a_sFileName)
{
    if(m_iSize<=0 || !m_pBuf) return FALSE;

    CFile f;
    if(!f.Open(a_sFileName,CFile::modeCreate|CFile::modeReadWrite)) return FALSE;
    __int64 iNeedWriteSize=m_iSize;
    UINT iWriteSize=0;
    __int64 iRealWriteSize=0;
    while(iNeedWriteSize>0){
        if(iNeedWriteSize>1024*1024*10) iWriteSize=1024*1024*10;
        else                              iWriteSize=(UINT)iNeedWriteSize;
        f.Write(m_pBuf+iRealWriteSize,iWriteSize);
        iNeedWriteSize-=iWriteSize;
        iRealWriteSize+=iWriteSize;
        if(m_pCallbackOwner && m_pStatusCallback){
            m_pStatusCallback(m_pCallbackOwner,0,(int)(m_iSize/1048576),(int)(iRealWriteSize/1048576));
        }
    }
    f.Close();

//     FILE* f;
//     f=fopen(a_sFileName,"wb");
//     if(!f) return FALSE;
//     fwrite(m_pBuf,m_iSize,1,f);
//     fclose(f);
    return TRUE;
}

// 从文件中读取数据
BOOL SunMemoryStream::LoadFromFile(LPCTSTR a_sFileName)
{
    CFile f;
    if(!f.Open(a_sFileName,CFile::modeRead)) return FALSE;

    if(m_pBuf) {
        delete m_pBuf;
        m_pBuf=NULL;
    }
    m_iSize=0;
    m_iPos=0;
    
    f.SeekToEnd();
    __int64 iFileSize=f.GetLength();
    f.SeekToBegin();
    m_pBuf=new byte[iFileSize];
	__int64 iNeedReadSize=iFileSize;
	UINT iReadSize=0;
    __int64 iRealReadSize=0;
	while(iNeedReadSize>0){
		if(iNeedReadSize>1024*1024*10) iReadSize=1024*1024*10;
		else                             iReadSize=(UINT)iNeedReadSize;
		f.Read(m_pBuf+iRealReadSize,iReadSize);
		iNeedReadSize-=iReadSize;
        iRealReadSize+=iReadSize;
        if(m_pCallbackOwner && m_pStatusCallback){
            m_pStatusCallback(m_pCallbackOwner,0,(int)(iFileSize/1048576),(int)(iRealReadSize/1048576));
        }
	}

    m_iSize=iFileSize;
    f.Close();


        /*
    if(m_pBuf) {
        delete m_pBuf;  
        m_pBuf=NULL;
    }
    m_iSize=0;
    m_iPos=0;

    FILE* f;
    f=fopen(a_sFileName,"rb");
    if(!f) return FALSE;
    fseek(f,0L,SEEK_END);
    __int64 iFileSize=ftell(f);
    fseek(f,0L,SEEK_SET);
    m_pBuf=new byte[iFileSize];
    fread(m_pBuf,iFileSize,1,f);
//    int iAlreadyRead=0;
//    int iLeaveBytes=iFileSize;
//    while(iAlreadyRead<iFileSize){
//        int iRead=fread(m_pBuf,iLeaveBytes,1,f);
//        iAlreadyRead+=iRead;
//        iLeaveBytes-=iRead;
//    }
    m_iSize=iFileSize;
    fclose(f);
    */
    return TRUE;
}

// 保存数据到文件
BOOL SunMemoryStream::SaveToFile(LPCTSTR a_sFileName,void* a_pOwner,PSunMemoryStatusCallback a_pCallback)
{
    SunMemoryStream* me=(SunMemoryStream*)a_pOwner;
    if(me){
        m_pCallbackOwner    =a_pOwner;
        m_pStatusCallback   =a_pCallback;
    }
    BOOL bResult=SaveToFile(a_sFileName);
    m_pCallbackOwner=NULL;
    m_pStatusCallback=NULL;
    return bResult;
}

// 从文件中读取数据
BOOL SunMemoryStream::LoadFromFile(LPCTSTR a_sFileName,void* a_pOwner,PSunMemoryStatusCallback a_pCallback)
{
    SunMemoryStream* me=(SunMemoryStream*)a_pOwner;
    if(me){
        m_pCallbackOwner    =a_pOwner;
        m_pStatusCallback   =a_pCallback;
    }
    BOOL bResult=LoadFromFile(a_sFileName);
    m_pCallbackOwner=NULL;
    m_pStatusCallback=NULL;
    return bResult;
}



//////////////////////////////////////////////////////////////////////////
// SunQuickFile 
// 快速文件操作类（写入）
// 工作原理：
// 可以设置缓冲大小，每次操作都写入到缓冲区，如果需要写入的大于缓冲区则直接写入
// 需要提前设置缓冲区大小，如果不设置，那么缓冲区为 1MByte
// 需要注意的是 m_iBufferSize 有可能比 m_iMaxBufferSize 大
// 正常情况下，m_iBufferSize 为当前需要存储的大小
//////////////////////////////////////////////////////////////////////////

SunQuickFile::SunQuickFile()
{
    m_hFile         =NULL;
    m_pBuf          =NULL;
    m_iBufferSize   =0;
    m_iMaxBufferSize=1024*1024;
    m_iSize         =0;
}

SunQuickFile::~SunQuickFile()
{
    CloseFile();
    if(m_pBuf){
        delete[] m_pBuf;
        m_pBuf=NULL;
    } 
}

void SunQuickFile::CloseFile()
{
    if(m_hFile){
        WriteCurrentToFile();
        CloseHandle(m_hFile);
        m_hFile=NULL;
    }
}

bool SunQuickFile::WriteCurrentToFile()
{
    bool bResult=false;
    if(m_pBuf && m_iBufferSize>0){
        if(RealWrite(m_pBuf,m_iBufferSize)>0){
            bResult=true;
            m_iBufferSize=0;
        }
    }
    return bResult;
}

bool SunQuickFile::Open(CString a_sFileName)
{
    if(m_pBuf){
        delete[] m_pBuf;
        m_pBuf=NULL;
        m_iBufferSize=0;
        m_iSize=0;
    }
    if(FileExists(a_sFileName)){
        m_hFile=CreateFile(a_sFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (m_hFile == INVALID_HANDLE_VALUE){
            //printf("创建文件对象失败,错误代码:%d ", GetLastError());
            return false;
        }
    }
    else{
        m_hFile=CreateFile(a_sFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        if (m_hFile == INVALID_HANDLE_VALUE){
            //printf("创建文件对象失败,错误代码:%d ", GetLastError());
            return false;
        }
    }
    return true;
}

__int64 SunQuickFile::Read(void* a_pBuf,__int64 a_iCount)
{
    return RealRead(a_pBuf,a_iCount);
}

__int64 SunQuickFile::Write(const void* a_pBuf,__int64 a_iCount)
{
    __int64 iSize=0;
    if(!m_pBuf){
        m_pBuf=new byte[m_iMaxBufferSize];
        m_iSize=0;
        m_iBufferSize=0;
    }
    if(a_iCount+m_iBufferSize>m_iMaxBufferSize){
        iSize+=m_iBufferSize;
        WriteCurrentToFile();
        if(a_iCount>m_iMaxBufferSize){
            iSize=RealWrite(a_pBuf,a_iCount);
        }
        else{
            memcpy(m_pBuf+m_iBufferSize,a_pBuf,a_iCount);
            m_iBufferSize+=a_iCount;
            iSize=a_iCount;
        }
    }
    else{
        memcpy(m_pBuf+m_iBufferSize,a_pBuf,a_iCount);
        m_iBufferSize+=a_iCount;
        iSize=a_iCount;
    }
        
    m_iSize+=iSize;
    return iSize;
}

__int64 SunQuickFile::RealRead(void* a_pBuf,__int64 a_iCount)
{
//     OVERLAPPED ol;
//     memset(&ol,0,sizeof(OVERLAPPED));
    byte* pPos=(byte*)a_pBuf;
    __int64 iNeedSize=a_iCount;
    while(iNeedSize>0){
        __int64 iRead=iNeedSize;
        if(iNeedSize>1024*1024*1024) iRead=1024*1024*1024;
        DWORD iSize;
        if(ReadFile(m_hFile,pPos,(DWORD)iRead,&iSize,NULL)){
            iNeedSize-=iSize;
            pPos+=iSize;
        }
    }
    return a_iCount-iNeedSize;
}

__int64 SunQuickFile::RealWrite(const void* a_pBuf,__int64 a_iCount)
{
    if(!m_hFile) return 0;

//     OVERLAPPED ol;
//     memset(&ol,0,sizeof(OVERLAPPED));

//     WriteFileEx(m_hFile,a_pBuf,a_iCount,Overlapped,NULL);
//     
//     return a_iCount;
    byte* pPos=(byte*)a_pBuf;
    __int64 iNeedSize=a_iCount;
    while(iNeedSize>0){
        __int64 iWrite=iNeedSize;
        if(iNeedSize>1024*1024*1024) iWrite=1024*1024*1024;
        DWORD iSize;
        SeekToEnd();
        if(WriteFile(m_hFile,pPos,(DWORD)iWrite,&iSize,NULL)){
            iNeedSize-=iSize;
            pPos+=iSize;
        }
    }
    return a_iCount-iNeedSize;
}

bool SunQuickFile::Seek(__int64 a_iOffset,UINT a_iFrom)
{
    LARGE_INTEGER iOffice;
    iOffice.QuadPart=a_iOffset;
    LARGE_INTEGER iRealOffice;
    DWORD dwFrom=FILE_BEGIN;
    if(a_iFrom==SunQuickFile::begin) dwFrom=FILE_BEGIN;
    if(a_iFrom==SunQuickFile::current) dwFrom=FILE_CURRENT;
    if(a_iFrom==SunQuickFile::end) dwFrom=FILE_END;
    return Bool(SetFilePointerEx(m_hFile,iOffice,&iRealOffice,dwFrom));
}

bool SunQuickFile::SeekToBegin()
{
    return Seek(0,begin);
}

__int64 SunQuickFile::SeekToEnd()
{
    LARGE_INTEGER iOffice;
    iOffice.QuadPart=0;
    LARGE_INTEGER iRealOffice;
    if(SetFilePointerEx(m_hFile,iOffice,&iRealOffice,FILE_END)==0) return -1;
    return iRealOffice.QuadPart;
}

//////////////////////////////////////////////////////////////////////////
// CMemoryPool
//////////////////////////////////////////////////////////////////////////

CMemoryPool::CMemoryPool(){
    m_pBuf  =NULL;
    m_iSize =0;
    m_iPos  =0;
}
CMemoryPool::~CMemoryPool(){
    Destroy();
}

// 创建内存池
bool CMemoryPool::CreatePool(__int64 a_iSize){
    if(m_pBuf){
        Destroy();
    }
    m_pBuf=new byte[a_iSize];
    return true;
}

// 获取内存，如果不足则返回 NULL
byte* CMemoryPool::GetBuffer(__int64 a_iLen){
    if(a_iLen>GetUsableSpace()){
        return NULL;
    }
    byte* pBuffer=m_pBuf+m_iPos;
    m_iPos+=a_iLen;
    return pBuffer;
}

// 销毁内存池
bool CMemoryPool::Destroy(){
    if(m_pBuf){
        delete[] m_pBuf;
        m_iPos=0;
        m_iSize=0;
    }
    return true;
}


//////////////////////////////////////////////////////////////////////////
// CMemoryPoolManage
//////////////////////////////////////////////////////////////////////////
CMemoryPoolManage::CMemoryPoolManage(){
    m_iMemoryPoolMaxSize=1024*1024;
}
CMemoryPoolManage::~CMemoryPoolManage(){
    Destroy();
}

// 分配内存
byte* CMemoryPoolManage::GetBuffer(__int64 a_iLen){
    byte* pResult=NULL;
    for(MemoryPoolList::iterator i=m_aMemoryPoolList.begin();i!=m_aMemoryPoolList.end();i++){
        pResult=(*i)->GetBuffer(a_iLen);
        if(pResult) return pResult;
    }
    if(!pResult){
        __int64 iPoolSize=m_iMemoryPoolMaxSize;
        if(a_iLen>m_iMemoryPoolMaxSize){
            iPoolSize=a_iLen;
        }
        CMemoryPool* pPool=new CMemoryPool;
        pPool->CreatePool(iPoolSize);
        m_aMemoryPoolList.push_back(pPool);
        pResult=pPool->GetBuffer(a_iLen);
    }
    return pResult;
}

// 销毁所有内存池
bool CMemoryPoolManage::Destroy(){
    for(MemoryPoolList::iterator i=m_aMemoryPoolList.begin();i!=m_aMemoryPoolList.end();i++){
        delete *i;
    }
    m_aMemoryPoolList.clear();
    return true;
}


}
