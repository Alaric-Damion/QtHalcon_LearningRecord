/*////////////////////////////////////////////////////////////////////////

  ���ܣ��׽���ͨ��ģ�� SocketBase.h

  ��ģ�麬�����¹��ܣ�
  1.ʹ��ͬ�� Socket����֤���ݵ��ȶ���
  2.ͨ����Ϣ����(ͨ���������Ա����ʵ��)���д�������Ҫ���ڼ��ɴ�����Ϣ
  3.�Զ����壬�������ý��ջ�������С
  4.�Զ�����������(�����ֹ��������������������)
  5.���Խ��ն����ƺ��ı��Ĺ���

  �����ͻ��˳���Ĵ�������:
    1.���� CClientSocket ����
      ���� Create �������������Ϊ�ͻ�����Ϣ����ָ��
    2.�������ݻᴥ����Ϣ����� OnReceive �¼�
    3.�������ڽ������ݳ���󣬻ᴥ����Ϣ����� OnClose �¼�

  ��������������Ĵ�������:
    1.���� CServerSocket ����
      ���� CServerSocket �� Create ���������������
        a.��Ϣ����ָ��(CServerMessage)
        b.�����˿�(int)
    2.CServerSocket �������������յ��ͻ������Ӻ󣬻ᴥ�� CServerSocket �� ClientMessageFactory ������
      �ڸ÷�������Ҫ����һ�� CClientMessage ����ָ�룬�ú������غ����̴������� CClientMessage �����
      OnConnected �¼�
    3.�� CClientSocket ���յ����ݺ󣬻ᴥ�� CClientMessage �� OnReceive �¼�
    4.�������ڽ������ݳ���󣬻ᴥ�� CClientMessage �� OnClose �¼�
    
  �������ݵĴ���
    1.����ָ���ɽ������ݵĴ�С������ͷ��С���磺SetRecvBuffSize(8)������ CClientSocket ����� 8 ���ֽ�
    �����ݺ����̴��� OnReceive �¼���������¼�������Խ������ݽ��յĴ���
      ���ڴ������¼�ֻ�᷵�����ô�С�����ݣ����Ի�������� Receive �������õ�ȫ��������
    2.���˿�������������������ݣ��������趨�����Զ����ȵ����ݣ�������Ҫָ���������ţ�����:0x13,0x10

  CSocketMessage: ������Ҫ����������Ϣ���д���ʹ��ʱֻ��Ҫ����һ�������������Ȼ����һ������Ķ��󼴿�
    ����:
        class CMyMessage:public CClientMessage{
        public:
            virtual void OnReceive(CSocketRoot*,_TCHAR* a_pBuf,int a_iBufLen){AfxMessageBox("�յ�����");};
        };
        �ڳ����ж���һ������
        CMyMessage m_mmMessage;
        ������������ָ�봫�� CSocketRoot ���󼴿�

  CSocketRoot: ʵ���˻����� Socket �����͡����Ӻ����ٵȹ���

  CClientSocket: �ͻ����׽���
    ����ʹ�� SetRecvBuffSize ���趨���ջ������Ĵ�С������һ��
    ����ʹ�� SetRecvText �����ý����ַ���(���Զ����ַ�����������)
    ����ʹ�� SetRecvBin �����ý��ն���������(���ݽ��ջ���������������)
    ע�⣺��ʹ�� SetRecvBuffSize��SetRecvText �� SetRecvBin ʱ�ᵼ�»����������ݶ�ʧ
    ��������������Ͽ������ӣ�����������ʱ��Ҳ��Ҫ���� Create() ��������ʼ�� socket
  

  CServerSocket: ���������׽���
    ÿ���пͻ����ӷ�����ʱ�����������Զ�����һ�� Socket ��֮��Ӧ��
    ͬʱ����һ�������̣߳����յ�����ʱ��ͨ����Ϣ����


����������ʾ����
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
    CServerSocket   m_ssServer;         // TCP ������

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

const int SOCKET_RECEIVE_DATA_BUFSIZE=1024*8;       // ���ջ�������С

// ��ַת��
SUN_DLL_DEFINE void InitAddr(SOCKADDR *sa, const _TCHAR *strIPAddr, int nPort);

// �ͻ�����Ϣ��
class SUN_DLL_DEFINE CClientMessage
{
public:
    virtual void OnReceive(CClientSocket* a_pSocket,char* a_pBuf,int a_iBufLen){};
    virtual void OnConnect(CClientSocket* a_pSocket){};
    virtual void OnClose(CClientSocket* a_pSocket){};
    virtual void OnExecute(CClientSocket* a_pSocket,_EXECUTE_TYPE a_etType,int a_iBytes){};
};

// ��������Ϣ��
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
    BOOL                m_bStop;                // ֹͣ���
    DWORD               m_dwData;               // �Զ������ݴ�Ŵ�
    sockaddr_in         m_siRemoteAddr;         // Զ�̻�����Ϣ
    bool                m_bOutputLog;           // �����־
    
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
    // ���� socket ���
    void SetSocket(SOCKET a_hSocket);
    // ����Զ�̵�ַ��Ϣ
    void SetRemoteAddr(sockaddr_in a_siRemoteAddr){m_siRemoteAddr=a_siRemoteAddr;};
    // ���Զ�̵�ַ��Ϣ
    sockaddr_in GetRemoteAddr(){return m_siRemoteAddr;};

    virtual int Connect(LPCTSTR a_sAddr,int a_iPort);
    virtual int Close();
    virtual int Receive(char* a_pBuf,int a_iBufLen,int a_nFlag=0);
    virtual int ReceiveFree(char* a_pBuf,int a_iMaxBufLen,const char* a_sEndText);
    virtual int Send(const char* a_pBuf,int a_iBufLen,int a_nFlag=0);
    DWORD GetData(){return m_dwData;};
    void SetData(DWORD a_dwData){m_dwData=a_dwData;};
    // �����Ƿ������־
    void SetOutputLog(const bool a_bValue){m_bOutputLog=a_bValue;};
    
public: // �¼�Ӧ��
};

class SUN_DLL_DEFINE CClientSocket :public CSocketRoot,public CIDItem
{
protected:
    DWORD               m_dwRecvBuffSize;       // ���ջ�������С
    bool                m_bReceiveAll;          // �������л���������
    bool                m_bRecvText;            // Ϊ������ı���Ϣ(���Զ����ַ�������)��������ն���������(�����趨��С��������)
    char*               m_pBuffer;              // ���ջ�����
    char                m_sTextEnd[MAX_PATH];   // �ı���Ϣ�����ַ���
    _TCHAR              m_sHost[MAX_PATH];      // ���ӵķ�������ַ
    DWORD               m_iPort;                // ���ӵĶ˿�
    
    HANDLE              m_hReceiveThread;       // ���������߳�
    UINT                m_dwThreadID;           // �����̵߳� ID
    CClientMessage*     m_pClientMessage;       // �ͻ�����Ϣ�ص�����
    bool                m_bConnected;           // �Ƿ�������״̬
    bool                m_bServerClient;        // �Ƿ���Ϊ�������Ľ��տͻ�(����Ƿ������Ľ��նˣ�����Ҫ����ж����Ϣ����)
    CRITICAL_SECTION    m_csLock;               // �ٽ���ͬ������
public:
    CClientSocket();
    virtual ~CClientSocket();
    void Lock(void);
    void UnLock(void);
    DWORD GetReceiveThreadID(){return m_dwThreadID;};
    static UINT _stdcall ReceiveThread(LPVOID AParam);   // ��ʼ�߳�
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
    virtual CIDItemBase* CreateInstance(){          // ���������ʵ��
        return new CClientSocket;
    }
    virtual LPCTSTR GetClassName(){             // �������
        GET_ITEM_CLASS_NAME(CClientSocket);
    }

    virtual void SetConnectInfo(LPCTSTR a_sAddr,int a_iPort);
    virtual int Connect(LPCTSTR a_sAddr,int a_iPort);
    virtual int Close();
    virtual int Send(const char* a_pBuf,int a_iBufLen,int a_nFlag=0);
        
    virtual LPCTSTR GetHost(){return m_sHost;};
    virtual DWORD GetPort(){return m_iPort;};

    CClientMessage* GetMessageObject(){return m_pClientMessage;};
    void SetRecvBuffSize(DWORD a_dwSize);           // ���ý��ջ�������С
    void SetRecvAll(bool a_bValue){m_bReceiveAll=a_bValue;};// ���ý����������ݣ�ֱ�����ݽ�����ϣ����ڶ�����ģʽ��
    void SetRecvText(const char* a_sTextEnd);       // ���ý����ı�
    void SetRecvBin();                              // ���ý��ն���������
    bool GetConnected(){return m_bConnected;};      // �Ƿ�����
    // ��������(�Զ�׷�ӻس�����)
    int SendCommand(LPCTSTR a_sCommand);
};

class SUN_DLL_DEFINE CServerSocket :public CSocketRoot
{
protected:
    //CWinThread*             m_pAcceptThread;        // ��������
    HANDLE                  m_hAcceptThread;        // ���������߳�
    CItemList               m_aClientSocket;        // ���ںͿͻ��˴򽻵�������(CClientSocket*)
    CServerMessage*         m_pServerMessage;       // ��Ϣ�¼�
    DWORD                   m_dwPort;               // ����˿�
    
    static UINT _stdcall AcceptThread(LPVOID AParam);        // ���������̺߳���
public:
    CServerSocket();
    virtual ~CServerSocket();

    virtual bool Create(CServerMessage* AMessage,int a_iPort);
    virtual int Close();
public:
    DWORD GetServerPort(){return m_dwPort;};
    // ����������ӵ�����
    int GetConnectCount();
    // ���һ���յ����ӣ����û�пյ��򴴽�һ���µ�
    CClientSocket* GetNeverUseSocket();
    // �ͷ�����δ�õ�����
    bool FreeAllClient();
    // �õ����еĿͻ�����
    CItemList* GetAllClientSocket(){return &m_aClientSocket;};
};

class SUN_DLL_DEFINE CUDPSocket :public CSocketRoot
{
protected:
    DWORD               m_dwRecvBuffSize;       // ���ջ�������С
    bool                m_bReveText;            // Ϊ������ı���Ϣ(���Զ����ַ�������)��������ն���������(�����趨��С��������)
    char*               m_pBuffer;              // ���ջ�����
    //CWinThread*         m_pReceiveThread;       // �����߳�
    HANDLE              m_hReceiveThread;       // �����߳�
    UINT                m_dwThreadID;           // �̱߳��

    CUDPMessage*        m_pMessage;

    static UINT _stdcall ReceiveThread(LPVOID AParam);   // ��ʼ�߳�
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

    // ���һ���Իس����н������ı�
    string ReceiveText();
    // ���ý��ջ�������С
    void SetRecvBuffSize(DWORD a_dwSize);
    // ���ý����ı�
    void SetRecvText();
    // ���ý��ն���������
    void SetRecvBin();
    virtual int Close();
};


}
#endif // !defined(AFX_SOCKETBASE_H__CE000BE8_369E_4DA6_BA0C_D871263CF682__INCLUDED_)
