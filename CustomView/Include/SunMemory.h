

#ifndef _sun_memory_h
#define _sun_memory_h

#include <list>
using namespace std;

/*
 * �ڴ���ģ��˵��
 * ��ģ�������Զ�ȡ���ļ�Ϊ 2GB
 * �������ڴ��С��ʱ�����ѡ�������ݣ�����ԭ�������ݽ��õ�����
 * 
 * ���ߣ�÷�ĺ�
 * ���ڣ�2009.3
 */

namespace sunLib{


typedef void (_stdcall* PSunMemoryStatusCallback)(void* a_pOwner,int a_iMin,int a_iMax,int a_iPos);

// �ڴ���
class SUN_DLL_DEFINE SunMemoryStream
{
public:
    enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };

    SunMemoryStream();
    SunMemoryStream(__int64 a_iMemSize);
    virtual ~SunMemoryStream();

    virtual void Init(byte a_iValue);                                   // ��ʼ������
    virtual __int64 Read(void* a_pBuf,__int64 a_iCount);                // ��ȡ����
    virtual void Write(const void* a_pBuf,__int64 a_iCount);            // д������
    virtual BOOL Seek(__int64 a_iOffset,UINT a_iFrom);                  // �ƶ�ָ��
    virtual void SeekToBegin(){m_iPos=0;};                              // ....
    virtual __int64 SeekToEnd();                                        // ....
    virtual __int64 GetSize() const{return m_iSize;};                   // ����ڴ��С
    virtual BOOL SetSize(__int64 a_iSize,BOOL a_bClear=true);           // �����ڴ��С
    virtual __int64 GetPosition() const {return m_iPos;};               // ��ȡָ��λ��
    virtual byte* GetBuffer() const{return m_pBuf;} ;                   // ����ڴ�����
    virtual byte** GetBufferAddr(){return &m_pBuf;};                    // ������ݵ��ڴ��ַ
    virtual BOOL SaveToFile(LPCTSTR a_sFileName);                       // �������ݵ��ļ�
    virtual BOOL LoadFromFile(LPCTSTR a_sFileName);                     // ���ļ��ж�ȡ����
    virtual BOOL IfEof(){return m_iPos>=m_iSize;};                      // �Ƿ��Ѿ�����
    virtual BOOL SaveToFile(LPCTSTR a_sFileName,void* a_pOwner,PSunMemoryStatusCallback a_pCallback);               // �������ݵ��ļ�
    virtual BOOL LoadFromFile(LPCTSTR a_sFileName,void* a_pOwner,PSunMemoryStatusCallback a_pCallback);             // ���ļ��ж�ȡ����

protected:
    byte*   m_pBuf;
    __int64 m_iSize;
    __int64 m_iPos;
    void*                       m_pCallbackOwner;       // �ص�����
    PSunMemoryStatusCallback    m_pStatusCallback;      // �ص�����ָ��
};

// ��ɢ���ݿ���д����ļ� 2018.2.21
class SUN_DLL_DEFINE SunQuickFile
{
protected:
public:
    enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };

    SunQuickFile();
    virtual ~SunQuickFile();

    virtual bool Open(CString   a_sFileName);                           // ��ʼ������
    virtual __int64 Read(void* a_pBuf,__int64 a_iCount);                // ��ȡ����
    virtual __int64 Write(const void* a_pBuf,__int64 a_iCount);            // д������
    virtual bool Seek(__int64 a_iOffset,UINT a_iFrom);                  // �ƶ�ָ��
    virtual bool SeekToBegin();                                         // ....
    virtual __int64 SeekToEnd();                                        // ....
    virtual void CloseFile();                                           // �ر��ļ�
    virtual HANDLE GetFileHandle(){return m_hFile;}                     // �õ��ļ����
    virtual void SetMaxBufferSize(int a_iBytes){m_iMaxBufferSize=a_iBytes;} // ������󻺳�����С
    virtual __int64 GetMaxBufferSize(){return m_iMaxBufferSize;}            // �����󻺳�����С
    virtual __int64 GetWriteSize(){return m_iSize;};                    // ���д����ļ���С
protected:
    virtual bool WriteCurrentToFile();                                  // д�뵱ǰ���������ݵ��ļ�
    virtual __int64 RealRead(void* a_pBuf,__int64 a_iCount);            // ��ȡ����
    virtual __int64 RealWrite(const void* a_pBuf,__int64 a_iCount);     // д������
protected:
    byte*   m_pBuf;             // ������
    __int64 m_iMaxBufferSize;   // ��󻺳�����С
    __int64 m_iBufferSize;      // ��������С
    __int64 m_iSize;            // �ܳ���
    HANDLE  m_hFile;            // �ļ����
    void*                       m_pCallbackOwner;       // �ص�����
};

// С�ڴ�ع���
// ���ڷ���������͵��ڴ棬��� c++ ���ڴ������ƣ�ͳһ�����������٣������ڴ��������
// ���ƣ�
// �������ó��ӵĴ�С������������ڴ��
// ����ʱ���ݵ�ǰ��Ҫ����Ĵ�С�ӳ����л�ȡ��ַ�������Ҫ����Ŀռ���������ӵĴ�С����ֱ�ӽ��з���
// ����͸�����Ҫ���ڴ��С�����еĳ����н��в��ң�������ҵ����ʵ��ڴ������̷��ظ�ָ�룬�����½�һ���ڴ��
// 2018.2.22
class SUN_DLL_DEFINE CMemoryPool{
public:
    CMemoryPool();
    virtual ~CMemoryPool();

    virtual bool CreatePool(const __int64 a_iSize);                     // �����ڴ��
    virtual byte* GetBuffer(const __int64 a_iLen);                      // ��ȡ�ڴ棬��������򷵻� NULL
    virtual bool Destroy();                                             // �����ڴ��
    virtual __int64 GetSize(){return m_iSize;}                          // ��ó��ӵĴ�С
    virtual __int64 GetUsableSpace(){return m_iSize-m_iPos;}            // ��ÿ��ÿռ��С
protected:

protected:
    byte*   m_pBuf;         // �ڴ��
    __int64 m_iSize;        // �ش�С
    __int64 m_iPos;         // ��һ���ɷ����λ��
};

typedef list<CMemoryPool*> MemoryPoolList;

// �ڴ�ع�����
// ���ɷ����ڴ沢�ҽ��й�����Ҫʹ���ڴ�ʱ���� GetBuffer ���ɣ����ͳһ��������
// Ĭ�ϳ��ӵĴ�СΪ 1 MByte
// 2018.2.23
class SUN_DLL_DEFINE CMemoryPoolManage{
public:
    CMemoryPoolManage();
    virtual ~CMemoryPoolManage();
    virtual byte* GetBuffer(const __int64 a_iLen);          // �����ڴ�
    virtual bool Destroy();                                 // ���������ڴ��
    virtual void SetPoolMaxSize(const __int64 a_iSize){     // ����ÿ�����ӵ����ߴ�
        m_iMemoryPoolMaxSize=a_iSize;};
protected:

protected:
    __int64         m_iMemoryPoolMaxSize;       // ÿ�����ӵ����ߴ磨�´����ĳ��Ӵ�С�����ĳһ����Ĵ�С�����˸�ֵ������ʹ�ø����ֵ��������
    MemoryPoolList  m_aMemoryPoolList;          // �ڴ���б�

};




}

#endif