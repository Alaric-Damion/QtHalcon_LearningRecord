

#ifndef _sun_memory_h
#define _sun_memory_h

#include <list>
using namespace std;

/*
 * 内存流模块说明
 * 本模块最大可以读取的文件为 2GB
 * 在重设内存大小的时候可以选择保留数据，这样原来的数据将得到保留
 * 
 * 作者：梅文海
 * 日期：2009.3
 */

namespace sunLib{


typedef void (_stdcall* PSunMemoryStatusCallback)(void* a_pOwner,int a_iMin,int a_iMax,int a_iPos);

// 内存流
class SUN_DLL_DEFINE SunMemoryStream
{
public:
    enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };

    SunMemoryStream();
    SunMemoryStream(__int64 a_iMemSize);
    virtual ~SunMemoryStream();

    virtual void Init(byte a_iValue);                                   // 初始化数据
    virtual __int64 Read(void* a_pBuf,__int64 a_iCount);                // 读取数据
    virtual void Write(const void* a_pBuf,__int64 a_iCount);            // 写入数据
    virtual BOOL Seek(__int64 a_iOffset,UINT a_iFrom);                  // 移动指针
    virtual void SeekToBegin(){m_iPos=0;};                              // ....
    virtual __int64 SeekToEnd();                                        // ....
    virtual __int64 GetSize() const{return m_iSize;};                   // 获得内存大小
    virtual BOOL SetSize(__int64 a_iSize,BOOL a_bClear=true);           // 重设内存大小
    virtual __int64 GetPosition() const {return m_iPos;};               // 获取指针位置
    virtual byte* GetBuffer() const{return m_pBuf;} ;                   // 获得内存数据
    virtual byte** GetBufferAddr(){return &m_pBuf;};                    // 获得数据的内存地址
    virtual BOOL SaveToFile(LPCTSTR a_sFileName);                       // 保存数据到文件
    virtual BOOL LoadFromFile(LPCTSTR a_sFileName);                     // 从文件中读取数据
    virtual BOOL IfEof(){return m_iPos>=m_iSize;};                      // 是否已经结束
    virtual BOOL SaveToFile(LPCTSTR a_sFileName,void* a_pOwner,PSunMemoryStatusCallback a_pCallback);               // 保存数据到文件
    virtual BOOL LoadFromFile(LPCTSTR a_sFileName,void* a_pOwner,PSunMemoryStatusCallback a_pCallback);             // 从文件中读取数据

protected:
    byte*   m_pBuf;
    __int64 m_iSize;
    __int64 m_iPos;
    void*                       m_pCallbackOwner;       // 回调参数
    PSunMemoryStatusCallback    m_pStatusCallback;      // 回调函数指针
};

// 零散数据快速写入大文件 2018.2.21
class SUN_DLL_DEFINE SunQuickFile
{
protected:
public:
    enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };

    SunQuickFile();
    virtual ~SunQuickFile();

    virtual bool Open(CString   a_sFileName);                           // 初始化数据
    virtual __int64 Read(void* a_pBuf,__int64 a_iCount);                // 读取数据
    virtual __int64 Write(const void* a_pBuf,__int64 a_iCount);            // 写入数据
    virtual bool Seek(__int64 a_iOffset,UINT a_iFrom);                  // 移动指针
    virtual bool SeekToBegin();                                         // ....
    virtual __int64 SeekToEnd();                                        // ....
    virtual void CloseFile();                                           // 关闭文件
    virtual HANDLE GetFileHandle(){return m_hFile;}                     // 得到文件句柄
    virtual void SetMaxBufferSize(int a_iBytes){m_iMaxBufferSize=a_iBytes;} // 设置最大缓冲区大小
    virtual __int64 GetMaxBufferSize(){return m_iMaxBufferSize;}            // 获得最大缓冲区大小
    virtual __int64 GetWriteSize(){return m_iSize;};                    // 获得写入的文件大小
protected:
    virtual bool WriteCurrentToFile();                                  // 写入当前缓冲区数据到文件
    virtual __int64 RealRead(void* a_pBuf,__int64 a_iCount);            // 读取数据
    virtual __int64 RealWrite(const void* a_pBuf,__int64 a_iCount);     // 写入数据
protected:
    byte*   m_pBuf;             // 缓冲区
    __int64 m_iMaxBufferSize;   // 最大缓冲区大小
    __int64 m_iBufferSize;      // 缓冲区大小
    __int64 m_iSize;            // 总长度
    HANDLE  m_hFile;            // 文件句柄
    void*                       m_pCallbackOwner;       // 回调参数
};

// 小内存池管理
// 用于分配各种类型的内存，替代 c++ 的内存分配机制，统一管理分配和销毁，提升内存的利用率
// 机制：
// 允许设置池子的大小，用链表管理内存池
// 分配时根据当前需要分配的大小从池子中获取地址，如果需要分配的空间大于最大池子的大小，则直接进行分配
// 否则就根据需要的内存大小在所有的池子中进行查找，如果能找到合适的内存则立刻返回该指针，否则新建一个内存池
// 2018.2.22
class SUN_DLL_DEFINE CMemoryPool{
public:
    CMemoryPool();
    virtual ~CMemoryPool();

    virtual bool CreatePool(const __int64 a_iSize);                     // 创建内存池
    virtual byte* GetBuffer(const __int64 a_iLen);                      // 获取内存，如果不足则返回 NULL
    virtual bool Destroy();                                             // 销毁内存池
    virtual __int64 GetSize(){return m_iSize;}                          // 获得池子的大小
    virtual __int64 GetUsableSpace(){return m_iSize-m_iPos;}            // 获得可用空间大小
protected:

protected:
    byte*   m_pBuf;         // 内存池
    __int64 m_iSize;        // 池大小
    __int64 m_iPos;         // 下一个可分配的位置
};

typedef list<CMemoryPool*> MemoryPoolList;

// 内存池管理类
// 自由分配内存并且进行管理，需要使用内存时调用 GetBuffer 即可，最后统一进行销毁
// 默认池子的大小为 1 MByte
// 2018.2.23
class SUN_DLL_DEFINE CMemoryPoolManage{
public:
    CMemoryPoolManage();
    virtual ~CMemoryPoolManage();
    virtual byte* GetBuffer(const __int64 a_iLen);          // 分配内存
    virtual bool Destroy();                                 // 销毁所有内存池
    virtual void SetPoolMaxSize(const __int64 a_iSize){     // 设置每个池子的最大尺寸
        m_iMemoryPoolMaxSize=a_iSize;};
protected:

protected:
    __int64         m_iMemoryPoolMaxSize;       // 每个池子的最大尺寸（新创建的池子大小，如果某一个块的大小超过了该值则允许使用更大的值来创建）
    MemoryPoolList  m_aMemoryPoolList;          // 内存池列表

};




}

#endif