/*////////////////////////////////////////////////////////////////////////

  功能：套接字通用模块 SocketBase.h

  本模块含有如下功能：
  1.使用同步 Socket，保证数据的稳定性
  2.通过消息机制(通过虚拟类成员函数实现)进行处理，不需要窗口即可触发消息
  3.自动缓冲，可以设置接收缓冲区大小
  4.自动服务器功能(无须手工处理服务器触发的连接)
  5.可以接收二进制和文本的功能

  创建客户端程序的处理流程:
    1.创建 CClientSocket 对象
      调用 Create 方法，传入参数为客户端消息对象指针
    2.接收数据会触发消息对象的 OnReceive 事件
    3.最后如果在接收数据出错后，会触发消息对象的 OnClose 事件

  创建服务器程序的处理流程:
    1.创建 CServerSocket 对象
      调用 CServerSocket 的 Create 方法，传入参数：
        a.消息对象指针(CServerMessage)
        b.监听端口(int)
    2.CServerSocket 进入监听后，如果收到客户端连接后，会触发 CServerSocket 的 ClientMessageFactory 函数，
      在该方法里需要返回一个 CClientMessage 对象指针，该函数返回后，立刻触发传入 CClientMessage 对象的
      OnConnected 事件
    3.当 CClientSocket 接收到数据后，会触发 CClientMessage 的 OnReceive 事件
    4.最后如果在接收数据出错后，会触发 CClientMessage 的 OnClose 事件
    
  接收数据的处理：
    1.可以指定可接收数据的大小，即包头大小，如：SetRecvBuffSize(8)，这样 CClientSocket 会接收 8 个字节
    的数据后立刻触发 OnReceive 事件，在这个事件里面可以进行数据接收的处理
      由于触发的事件只会返回设置大小的数据，所以还必须调用 Receive 函数来得到全部的数据
    2.除了可以这样处理二进制数据，还可以设定接收自动长度的数据，但是需要指定结束符号，例如:0x13,0x10

  CSocketMessage: 该类主要是用来对消息进行处理，使用时只需要创建一个该类的派生类然后定义一个该类的对象即可
    例如:
        class CMyMessage:public CClientMessage{
        public:
            virtual void OnReceive(CSocketRoot*,_TCHAR* a_pBuf,int a_iBufLen){AfxMessageBox("收到数据");};
        };
        在程序中定义一个对象
        CMyMessage m_mmMessage;
        最后把这个对象的指针传给 CSocketRoot 对象即可

  CSocketRoot: 实现了基本的 Socket 创建和、连接和销毁等功能

  CClientSocket: 客户端套接字
    可以使用 SetRecvBuffSize 来设定接收缓冲区的大小，并且一定
    可以使用 SetRecvText 来设置接收字符串(以自定义字符串做结束符)
    可以使用 SetRecvBin 来设置接收二进制数据(根据接收缓冲区来接收数据)
    注意：当使用 SetRecvBuffSize、SetRecvText 和 SetRecvBin 时会导致缓冲区的数据丢失
    如果服务器主动断开了连接，则重新连接时，也需要调用 Create() 函数来初始化 socket
  

  CServerSocket: 服务器端套接字
    每当有客户连接服务器时，服务器会自动产生一个 Socket 与之对应，
    同时产生一个接收线程，当收到数据时，通过消息返回


服务器代码示例：
class CDlgMain;

class CMyClientMessage:public CClientMessage
{
protected:
    CDlgMain*   m_pMain;
public:
    void SetMain(CDlgMain* a_pMain){m_pMain=a_pMain;};

    virtual void OnReceive(CClientSocket* a_pSocket,char* a_pBuf,int a_iBufLen);
    virtual void OnConnect(CClientSocket* a_pSocket);
    virtual void OnClose(CClientSocket* a_pSocket);
    virtual void OnExecute(CClientSocket* a_pSocket,_EXECUTE_TYPE a_etType,int a_iBytes);
};

class CDlgMain : public CDialog,public CServerMessage
{
private:
    CServerSocket   m_ssServer;         // TCP 服务器

    virtual CClientMessage* ClientMessageFactory(CClientSocket* a_pSocket){
        CMyClientMessage* pMessage=new CMyClientMessage;
        pMessage->SetMain(this);
        return pMessage;
    }
...
}

////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_SOCKETBASE_H__CE000BE8_369E_4DA6_BA0C_D871263CF682__INCLUDED_)
#define AFX_SOCKETBASE_H__CE000BE8_369E_4DA6_BA0C_D871263CF682__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SunItemList.h"
#include"winsock2.h"
#pragma comment(lib,"ws2_32.lib")
#include <process.h>


namespace sunLib{

enum _SOCKET_TYPE{stTCP,stUDP};
enum _EXECUTE_TYPE{etReceive,etSend};

class CSocketRoot;
class CClientSocket;
class CServerSocket;
class CUDPSocket;

const int SOCKET_RECEIVE_DATA_BUFSIZE=1024*8;       // 接收缓冲区大小

// 地址转换
SUN_DLL_DEFINE void InitAddr(SOCKADDR *sa, const _TCHAR *strIPAddr, int nPort);

// 客户端消息类
class SUN_DLL_DEFINE CClientMessage
{
public:
    virtual void OnReceive(CClientSocket* a_pSocket,char* a_pBuf,int a_iBufLen){};
    virtual void OnConnect(CClientSocket* a_pSocket){};
    virtual void OnClose(CClientSocket* a_pSocket){};
    virtual void OnExecute(CClientSocket* a_pSocket,_EXECUTE_TYPE a_etType,int a_iBytes){};
};

// 服务器消息类
class SUN_DLL_DEFINE CServerMessage
{
public:
    virtual CClientMessage* ClientMessageFactory(CClientSocket* a_pSocket)=NULL;
};

class SUN_DLL_DEFINE CUDPMessage
{
public:
    virtual void OnReceiveFrom(CUDPSocket* a_pSocket,char* a_pBuf,int a_iBufLen,sockaddr* a_saFrom){};
    virtual void OnClose(CUDPSocket* a_pSocket){};
};

class SUN_DLL_DEFINE CSocketRoot
{
protected:
    SOCKET              m_hSocket;
    BOOL                m_bStop;                // 停止标记
    DWORD               m_dwData;               // 自定义数据存放处
    sockaddr_in         m_siRemoteAddr;         // 远程机器信息
    bool                m_bOutputLog;           // 输出日志
    
    virtual void Begin(){};
    virtual bool Initialize(_SOCKET_TYPE a_bFlag);
public:
    CSocketRoot();
    virtual ~CSocketRoot();
    
    virtual void Stop() {
        m_bStop=TRUE;
    };
    BOOL IfStop(){return m_bStop;};
    SOCKET GetSocket(){return m_hSocket;};
    // 设置 socket 句柄
    void SetSocket(SOCKET a_hSocket);
    // 设置远程地址信息
    void SetRemoteAddr(sockaddr_in a_siRemoteAddr){m_siRemoteAddr=a_siRemoteAddr;};
    // 获得远程地址信息
    sockaddr_in GetRemoteAddr(){return m_siRemoteAddr;};

    virtual int Connect(LPCTSTR a_sAddr,int a_iPort);
    virtual int Close();
    virtual int Receive(char* a_pBuf,int a_iBufLen,int a_nFlag=0);
    virtual int ReceiveFree(char* a_pBuf,int a_iMaxBufLen,const char* a_sEndText);
    virtual int Send(const char* a_pBuf,int a_iBufLen,int a_nFlag=0);
    DWORD GetData(){return m_dwData;};
    void SetData(DWORD a_dwData){m_dwData=a_dwData;};
    // 设置是否输出日志
    void SetOutputLog(const bool a_bValue){m_bOutputLog=a_bValue;};
    
public: // 新加应用
};

class SUN_DLL_DEFINE CClientSocket :public CSocketRoot,public CIDItem
{
protected:
    DWORD               m_dwRecvBuffSize;       // 接收缓冲区大小
    bool                m_bReceiveAll;          // 接收所有缓冲区数据
    bool                m_bRecvText;            // 为真接收文本信息(以自定义字符串结束)，否则接收二进制数据(根据设定大小接收数据)
    char*               m_pBuffer;              // 接收缓冲区
    char                m_sTextEnd[MAX_PATH];   // 文本信息结束字符串
    _TCHAR              m_sHost[MAX_PATH];      // 连接的服务器地址
    DWORD               m_iPort;                // 连接的端口
    
    HANDLE              m_hReceiveThread;       // 管理连接线程
    UINT                m_dwThreadID;           // 接收线程的 ID
    CClientMessage*     m_pClientMessage;       // 客户端消息回调对象
    bool                m_bConnected;           // 是否处于连接状态
    bool                m_bServerClient;        // 是否作为服务器的接收客户(如果是服务器的接收端，则需要自行卸载消息对象)
    CRITICAL_SECTION    m_csLock;               // 临界区同步对象
public:
    CClientSocket();
    virtual ~CClientSocket();
    void Lock(void);
    void UnLock(void);
    DWORD GetReceiveThreadID(){return m_dwThreadID;};
    static UINT _stdcall ReceiveThread(LPVOID AParam);   // 开始线程
    virtual bool Create(CClientMessage* AMessage,bool a_bServerClient){
        m_pClientMessage=AMessage;
        m_bServerClient =a_bServerClient;
        return CSocketRoot::Initialize(stTCP);
    };
    virtual void BeginReceiveData(){
        m_bStop=FALSE;
        //m_pReceiveThread=AfxBeginThread(ReceiveThread,this,THREAD_PRIORITY_NORMAL);
        m_dwThreadID=0;
        //(LPTHREAD_START_ROUTINE)
        m_hReceiveThread=(HANDLE)_beginthreadex(NULL,0,ReceiveThread,this,CREATE_SUSPENDED,&m_dwThreadID);
        ResumeThread(m_hReceiveThread);
    };
    virtual CIDItemBase* CreateInstance(){          // 创建该类的实例
        return new CClientSocket;
    }
    virtual LPCTSTR GetClassName(){             // 获得类名
        GET_ITEM_CLASS_NAME(CClientSocket);
    }

    virtual void SetConnectInfo(LPCTSTR a_sAddr,int a_iPort);
    virtual int Connect(LPCTSTR a_sAddr,int a_iPort);
    virtual int Close();
    virtual int Send(const char* a_pBuf,int a_iBufLen,int a_nFlag=0);
        
    virtual LPCTSTR GetHost(){return m_sHost;};
    virtual DWORD GetPort(){return m_iPort;};

    CClientMessage* GetMessageObject(){return m_pClientMessage;};
    void SetRecvBuffSize(DWORD a_dwSize);           // 设置接收缓冲区大小
    void SetRecvAll(bool a_bValue){m_bReceiveAll=a_bValue;};// 设置接收所有数据，直到数据接收完毕（用于二进制模式）
    void SetRecvText(const char* a_sTextEnd);       // 设置接收文本
    void SetRecvBin();                              // 设置接收二进制数据
    bool GetConnected(){return m_bConnected;};      // 是否连接
    // 发送命令(自动追加回车换行)
    int SendCommand(LPCTSTR a_sCommand);
};

class SUN_DLL_DEFINE CServerSocket :public CSocketRoot
{
protected:
    //CWinThread*             m_pAcceptThread;        // 管理连接
    HANDLE                  m_hAcceptThread;        // 管理连接线程
    CItemList               m_aClientSocket;        // 用于和客户端打交道的连接(CClientSocket*)
    CServerMessage*         m_pServerMessage;       // 消息事件
    DWORD                   m_dwPort;               // 服务端口
    
    static UINT _stdcall AcceptThread(LPVOID AParam);        // 接收连接线程函数
public:
    CServerSocket();
    virtual ~CServerSocket();

    virtual bool Create(CServerMessage* AMessage,int a_iPort);
    virtual int Close();
public:
    DWORD GetServerPort(){return m_dwPort;};
    // 获得正常连接的数量
    int GetConnectCount();
    // 获得一个空的连接，如果没有空的则创建一个新的
    CClientSocket* GetNeverUseSocket();
    // 释放所有未用的连接
    bool FreeAllClient();
    // 得到所有的客户连接
    CItemList* GetAllClientSocket(){return &m_aClientSocket;};
};

class SUN_DLL_DEFINE CUDPSocket :public CSocketRoot
{
protected:
    DWORD               m_dwRecvBuffSize;       // 接收缓冲区大小
    bool                m_bReveText;            // 为真接收文本信息(以自定义字符串结束)，否则接收二进制数据(根据设定大小接收数据)
    char*               m_pBuffer;              // 接收缓冲区
    //CWinThread*         m_pReceiveThread;       // 接收线程
    HANDLE              m_hReceiveThread;       // 接收线程
    UINT                m_dwThreadID;           // 线程编号

    CUDPMessage*        m_pMessage;

    static UINT _stdcall ReceiveThread(LPVOID AParam);   // 开始线程
public:
    CUDPSocket();
    virtual ~CUDPSocket();
    virtual bool Create(CUDPMessage* AMessage,int a_iPort);
    virtual void BeginReceiveData(){
        m_bStop=FALSE;
        //m_pReceiveThread=AfxBeginThread(ReceiveThread,this,THREAD_PRIORITY_NORMAL);
        m_hReceiveThread=(HANDLE)_beginthreadex(NULL,0,ReceiveThread,this,CREATE_SUSPENDED,&m_dwThreadID);
        ResumeThread(m_hReceiveThread);
    };
    int ReceiveFrom(char FAR * buf, int len, int flags, struct sockaddr FAR *from, int FAR * fromlen);
    int SendTo(byte* a_pSend,int a_iLen,LPCTSTR a_sAddr,int a_iPort);

    // 获得一个以回车换行结束的文本
    string ReceiveText();
    // 设置接收缓冲区大小
    void SetRecvBuffSize(DWORD a_dwSize);
    // 设置接收文本
    void SetRecvText();
    // 设置接收二进制数据
    void SetRecvBin();
    virtual int Close();
};


}
#endif // !defined(AFX_SOCKETBASE_H__CE000BE8_369E_4DA6_BA0C_D871263CF682__INCLUDED_)
