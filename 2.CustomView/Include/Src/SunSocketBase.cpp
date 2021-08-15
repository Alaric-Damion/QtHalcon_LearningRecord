// SocketBase.cpp: implementation of the CSocketBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunSocketBase.h"
#include "SunStr.h"


#ifdef _DEBUG
#undef THIS_FILE
static _TCHAR THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace sunLib{
    
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void InitAddr(SOCKADDR *sa, const _TCHAR *strIPAddr, int nPort)
{
    SOCKADDR_IN *sin=(SOCKADDR_IN *)sa;
    memset(sin, 0, sizeof(SOCKADDR_IN));
#ifdef _UNICODE
    string s=WideToMutilByte(strIPAddr);
    sin->sin_addr.S_un.S_addr=inet_addr(s.c_str());
#else
    sin->sin_addr.S_un.S_addr=inet_addr(strIPAddr);
#endif
    sin->sin_family=AF_INET;
    sin->sin_port=htons(nPort);
}


CSocketRoot::CSocketRoot()
{
    m_hSocket       =NULL;
    m_bStop         =true;
    m_dwData        =NULL;
    m_bOutputLog    =false;
    memset(&m_siRemoteAddr,0,sizeof(m_siRemoteAddr));
}

CSocketRoot::~CSocketRoot()
{
    if(m_hSocket) Close();
}

bool CSocketRoot::Initialize(_SOCKET_TYPE a_bFlag)
{   
    if(m_hSocket){
        Close();
    }
    struct protoent *ppe;
    if(a_bFlag==stTCP){
        ppe=getprotobyname("tcp");
        m_hSocket=socket(PF_INET,SOCK_STREAM,ppe->p_proto);
    }
    else{
        ppe=getprotobyname("udp");
        m_hSocket=socket(PF_INET,SOCK_DGRAM,ppe->p_proto);
    }
    if(m_hSocket==INVALID_SOCKET){
        return false;
    }
    else{
        // Set the socket options:
        const int one = 1;
//         if (setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (_TCHAR *)&one, sizeof(one))){
//             return false;
//         }
        if (setsockopt(m_hSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&one, sizeof(one))){
            return false;
        }
//         int iBufLen=0;
//         setsockopt(m_hSocket,SOL_SOCKET,SO_SNDBUF,(_TCHAR*)&iBufLen,sizeof(iBufLen));
//         setsockopt(m_hSocket,SOL_SOCKET,SO_RCVBUF,(_TCHAR*)&iBufLen,sizeof(iBufLen));
//         int on=1;
//         setsockopt(m_hSocket,SOL_SOCKET,SO_REUSEADDR,(_TCHAR*)&on,sizeof(on));
        return true;
    }
}

int CSocketRoot::Connect(LPCTSTR a_sAddr,int a_iPort)
{
    if( _tcslen( a_sAddr ) == 0 || a_iPort == 0 )
        return SOCKET_ERROR;
    struct sockaddr_in daddr;
    memset((void *)&daddr,0,sizeof(daddr));
    daddr.sin_family=AF_INET;
    daddr.sin_port=htons(a_iPort);
#ifdef _UNICODE
    string s=WideToMutilByte(a_sAddr);
    daddr.sin_addr.s_addr=inet_addr(s.c_str());
#else
    daddr.sin_addr.s_addr=inet_addr(a_sAddr);
#endif
    int iResult=connect(m_hSocket,(struct sockaddr *)&daddr,sizeof(daddr));
    if(iResult!=SOCKET_ERROR){
        return 1;
    }
    SetRemoteAddr(daddr);
    return iResult;
}

int CSocketRoot::Close()
{
    if(!m_hSocket) return 0;
    memset(&m_siRemoteAddr,0,sizeof(m_siRemoteAddr));
    if(m_hSocket){
        int iRet=closesocket(m_hSocket);
        m_hSocket=NULL;
        return iRet;
    }
    else{
        return -1;
    }
}

void CSocketRoot::SetSocket(SOCKET a_hSocket)
{
    // 先把原套接字关闭
    if(m_hSocket){
        int iRet=closesocket(m_hSocket);
        m_hSocket=NULL;
    }
    m_hSocket=a_hSocket;
};

int CSocketRoot::Receive(char* a_pBuf,int a_iBufLen,int a_nFlag)
{
    //int iResult=recv(m_hSocket,a_pBuf,a_iBufLen,a_nFlag);
    int iReceiveBytes=0;        // 得到的总字节数
    int iReceive=0;
    // 把剩余的数据收完
    int iNeedRecv=a_iBufLen-iReceive;
    if(m_bOutputLog) OutputDebugString(Format(_T("需要接收%d字节"),iNeedRecv));
    while(iNeedRecv>0){
        int iRecvBytes=iNeedRecv;
        if(iRecvBytes>1024*100){
            iRecvBytes=1024*100;
        }
        iReceive=recv(m_hSocket,a_pBuf+iReceiveBytes,iRecvBytes,a_nFlag);
        if(m_bOutputLog) OutputDebugString(Format(_T("收到%d字节"),iReceive));
        if(iReceive<=0) {
//             int iError=WSAGetLastError();
//             if(m_bOutputLog) OutputDebugString(Format(_T("接收数据出错!代码:%d"),iError));
//             Close();
//             SetSocket(NULL);
            break;
        }
        iReceiveBytes+=iReceive;
        iNeedRecv-=iReceive;
    }
    return iReceiveBytes;
}

// 根据结束字符自由接收数据 2018.3.8
// 使用前请先分配空间并且清空数据
int CSocketRoot::ReceiveFree(char* a_pBuf,int a_iMaxBufLen,const char* a_sEndText){
    size_t iEndLen=strlen(a_sEndText);
    int iAlreadyRecv=0;
    do{
        size_t iExistTextLen=strlen(a_pBuf);            // 得到现有的字符串长度
        char* pDes=strstr(a_pBuf,a_sEndText);
        if(pDes){   // 找到结束字符串
            //__int64 iTextLen=pDes-a_pBuf;
            //a_pBuf[iTextLen]=0;
            //size_t iLen=strlen(a_pBuf);
            break;
        }
        else{       // 没有找到就继续收数据
            int iReceive=recv(m_hSocket,a_pBuf+iExistTextLen,1,0);
            if(iReceive<=0) {
                break;
            }
            iAlreadyRecv+=iReceive;
        }
    }while(true);

    return iAlreadyRecv;
}

int CSocketRoot::Send(const char* a_pBuf,int a_iBufLen,int a_nFlag)
{
    return send(m_hSocket,a_pBuf,a_iBufLen,a_nFlag);
}

CClientSocket::CClientSocket()
{
    m_pBuffer   =NULL;
    m_bConnected=false;
    m_bReceiveAll=false;
    //m_pReceiveThread=NULL;
    m_pClientMessage=NULL;
    m_hReceiveThread=NULL;
    m_bServerClient =false;
    m_dwThreadID=0;
    m_dwRecvBuffSize=0;
    m_bRecvText     =false;
    memset(m_sTextEnd,0,sizeof(m_sTextEnd));
    //CString a(Crlf);
    strcpy_s(m_sTextEnd,10,("\r\n"));
    SetRecvBuffSize(m_dwRecvBuffSize);     

    InitializeCriticalSection(&m_csLock);
}

CClientSocket::~CClientSocket()
{
    Close();
    if(m_hReceiveThread){
        if(WaitForSingleObject(m_hReceiveThread,200)==WAIT_TIMEOUT){
        CloseHandle(m_hReceiveThread);
        m_hReceiveThread=NULL;
    }
    }
    if(m_pBuffer){
        delete[] m_pBuffer;
        m_pBuffer=NULL;
    }
    DeleteCriticalSection(&m_csLock);
}

void CClientSocket::Lock(void) //多线程下使用
{
    EnterCriticalSection(&m_csLock);
}

void CClientSocket::UnLock(void)
{ 
    LeaveCriticalSection(&m_csLock);
}
void CClientSocket::SetConnectInfo(LPCTSTR a_sAddr,int a_iPort)
{
    StrCopySafe(m_sHost,a_sAddr);
    m_iPort=a_iPort;
}

int CClientSocket::Connect(LPCTSTR a_sAddr,int a_iPort)
{
    StrCopySafe(m_sHost,a_sAddr);
    m_iPort=a_iPort;
    int iResult=CSocketRoot::Connect(a_sAddr,a_iPort);
    if(iResult!=SOCKET_ERROR){
        m_bConnected=true;
        iResult=TRUE;
    }
    else{
        m_bConnected=false;
        iResult=FALSE;
    }
    return iResult;
}

int CClientSocket::Close()
{
    if(!m_hSocket) return 0;
    if(m_pClientMessage){
        m_pClientMessage->OnClose(this);
        if(m_bServerClient){
            delete m_pClientMessage;
            m_pClientMessage=NULL;
        }
    }                                      
    m_bConnected=false;
    int iResult=CSocketRoot::Close();
    return iResult;
}

// 发送命令(自动追加回车换行)
// 2007.7.30
int CClientSocket::SendCommand(LPCTSTR a_sCommand)
{
    CString sCommand(a_sCommand);
    sCommand+=m_sTextEnd;
    return Send((char*)sCommand.GetBuffer(0),sCommand.GetLength());
}

int CClientSocket::Send(const char* a_pBuf,int a_iBufLen,int a_nFlag)
{
    // 发送数据需要进行重试
    int iNeedSendByts=a_iBufLen;
    if(m_bOutputLog) OutputDebugString(Format(_T("需要发送%d字节"),iNeedSendByts));
    int iSendCount=0;
    while(iNeedSendByts>0){
        int iSendBytes=iNeedSendByts;
        if(iSendBytes>1024*100){
            iSendBytes=1024*100;
        }
        int iSend=send(m_hSocket,a_pBuf+iSendCount,iSendBytes,a_nFlag);
        if(m_bOutputLog) OutputDebugString(Format(_T("已经发送%d字节"),iSend));
        if(iSend<=0) break;
        iNeedSendByts-=iSend;
        iSendCount+=iSend;
        if(iNeedSendByts<=0){
            break;
        }
    }
//     if(iSendCount<=0){
//         Close();
//         Create(m_pClientMessage,m_bServerClient);
//         if(!Connect(m_sHost,m_iPort)) return 0;
//         return send(m_hSocket,a_pBuf,a_iBufLen,a_nFlag);
//     }

    return iSendCount;
}

// 设置接收缓冲区大小
// 2007.11.9
void CClientSocket::SetRecvBuffSize(DWORD a_dwSize)
{
//     if(a_dwSize==m_dwRecvBuffSize && m_pBuffer){
//         return;
//     }
    if(m_pBuffer){
        delete[] m_pBuffer;
        m_pBuffer=NULL;
    }
    m_dwRecvBuffSize=a_dwSize;
    if(m_dwRecvBuffSize>0){
        m_pBuffer=new char[m_dwRecvBuffSize];
        memset(m_pBuffer,0,m_dwRecvBuffSize);
    }
}

// 设置接收文本
// 2007.11.9
void CClientSocket::SetRecvText(const char* a_sTextEnd)
{
    m_bRecvText=true;
    strcpy_s(m_sTextEnd,MAX_PATH,a_sTextEnd);
}

// 设置接收二进制数据
// 2007.11.9
void CClientSocket::SetRecvBin()
{
    m_bRecvText=false;
}

// 开始线程
// 2007.8.2
UINT _stdcall CClientSocket::ReceiveThread(LPVOID AParam)
{
    CClientSocket* me=(CClientSocket*)AParam;
    try{
        while (me->GetSocket()) {
            if(!me->m_bRecvText){       // 二进制数据
                int iReceive=0;
                int iReceiveBytes=0;     // 得到的总字节数
                if(me->m_bReceiveAll){
                    iReceive=me->Receive(me->m_pBuffer+iReceiveBytes,me->m_dwRecvBuffSize);
                    iReceiveBytes+=iReceive;
                    if(iReceive<=0) {
                        if(errno!=EINTR){
                            me->Close();
                            goto End;
                        }
                        continue;                                           
                    }
                    CString sTemp=Format(_T("(线程:%4.4d)"),GetCurrentThreadId());
                    if(me->m_bOutputLog) OutputDebugString(Format(_T("%s收到数据:%d字节"),sTemp,iReceive));
                    if(me->m_pClientMessage && me->GetSocket() && me->m_pBuffer && iReceiveBytes>0){
                        me->m_pClientMessage->OnReceive(me,me->m_pBuffer,iReceiveBytes);
                    }
                }
                else{
                // 把剩余的数据收完
                int iNeedRecv=me->m_dwRecvBuffSize;
                while(iNeedRecv>0){
                    CString sTemp=Format(_T("(线程:%4.4d)"),GetCurrentThreadId());
                    if(me->m_bOutputLog) OutputDebugString(sTemp+_T("需要接收 ")+IntToStr(iNeedRecv)+_T(" 个字节的数据"));
                    iReceive=me->Receive(me->m_pBuffer+iReceiveBytes,iNeedRecv);
                    if(me->m_bOutputLog) OutputDebugString(Format(_T("%s收到数据:%d字节"),sTemp,iReceive));
                    if(iReceive<=0) {
                        me->Close();
                        goto End;                                           
                    }
                    iReceiveBytes+=iReceive;
                    if(me->m_pClientMessage && me->GetSocket()){
                        me->m_pClientMessage->OnExecute(me,etReceive,iReceiveBytes);
                    }
                    iNeedRecv-=iReceive;
                }
                if(me->m_pClientMessage && me->GetSocket() && me->m_pBuffer && iReceiveBytes>0){
                    me->m_pClientMessage->OnReceive(me,me->m_pBuffer,iReceiveBytes);
                }
            }
            }
            else{       // 文本数据
                size_t iEndLen=strlen(me->m_sTextEnd);
                int iAlreadyRecv=0;
                do{
                    size_t iExistTextLen=strlen(me->m_pBuffer);        // 得到现有的字符串长度
                    char* pDes=strstr(me->m_pBuffer,me->m_sTextEnd);
                    if(pDes){   // 找到结束字符串
                        __int64 iTextLen=pDes-me->m_pBuffer;
                        if(me->m_pClientMessage){
                            me->m_pClientMessage->OnReceive(me,me->m_pBuffer,(int)iTextLen);
                            me->m_pBuffer[iTextLen]=0;
                            size_t iLen=strlen(me->m_pBuffer);
                            // 把已经收到的缓冲区删除
                            memmove(me->m_pBuffer,me->m_pBuffer+iTextLen+iEndLen,iExistTextLen-iTextLen-iEndLen);
                            me->m_pBuffer[iExistTextLen-iTextLen-iEndLen]=0;
                        }
                        break;
                    }
                    else{       // 没有找到就继续收数据
                        //int iReceive=me->Receive(me->m_pBuffer+iExistTextLen,SOCKET_RECEIVE_DATA_BUFSIZE);
                        int iReceive=recv(me->m_hSocket,me->m_pBuffer+iExistTextLen,SOCKET_RECEIVE_DATA_BUFSIZE,0);
                        if(iReceive<=0) {
                            me->Close();
                            me->SetSocket(NULL);
                            goto End;
                        }
                        me->m_pBuffer[iReceive+iExistTextLen]=0;
                        iAlreadyRecv+=iReceive;
                    }
                }while(true);
            }
        }
    }
    catch (...) {
        if(me->m_bOutputLog) OutputDebugString(_T("接收线程出错了！"));
    }
End:
    me->Stop();
    //ExitThread(0);
    if(me->m_hReceiveThread){
        CloseHandle(me->m_hReceiveThread);
        me->m_hReceiveThread=NULL;
    }
    //if(me->m_bServerClient) delete me;
    return 0;
}

CServerSocket::CServerSocket()
{
    //m_pAcceptThread     =NULL;
    m_hAcceptThread     =0;
    m_pServerMessage    =NULL;
    m_dwPort            =0;
}

CServerSocket::~CServerSocket()
{
    Close();
    if(m_hAcceptThread){
        CloseHandle(m_hAcceptThread);
        m_hAcceptThread=0;
    }
}

// 获得正常连接的数量
// 2007.11.8
int CServerSocket::GetConnectCount()
{
    int iResult=0;
    int iCount=(int)m_aClientSocket.GetItemCount();
    for(int i=0;i<iCount;i++){
        CClientSocket* pSocket=(CClientSocket*)m_aClientSocket.GetValue(i);
        if(pSocket && pSocket->GetSocket()){
            iResult++;
        }
    }
    return iResult;
}

// 获得一个空的连接，如果没有空的则创建一个新的
// 2007.11.8
CClientSocket* CServerSocket::GetNeverUseSocket()
{
    CClientSocket* pSocket=NULL;
    int iCount=(int)m_aClientSocket.GetItemCount();
    for(int i=0;i<iCount;i++){
        CClientSocket* pTempSocket=(CClientSocket*)m_aClientSocket.GetValue(i);
        if(!pTempSocket->GetSocket()){
            pSocket=pTempSocket;
            break;
        }
    }
    if(!pSocket){
        pSocket=new CClientSocket;
        int one=1;
        setsockopt(pSocket->GetSocket(), IPPROTO_TCP, TCP_NODELAY, (char *)&one, sizeof(one));
        m_aClientSocket.AddItem(pSocket);
    }
    pSocket->Create(m_pServerMessage->ClientMessageFactory(pSocket),true);
    return pSocket;
}

// 释放所有未用的连接
// 2007.11.8
bool CServerSocket::FreeAllClient()
{
    CClientSocket* pSocket=NULL;
    int iCount=(int)m_aClientSocket.GetItemCount();
    for(int i=iCount-1;i>=0;i--){
        pSocket=(CClientSocket*)m_aClientSocket.GetValue(i);
        if(!pSocket->GetSocket()){
            m_aClientSocket.DelItem(i);
        }
    }
    return true;
}

bool CServerSocket::Create(CServerMessage* AMessage,int a_iPort)
{
    m_pServerMessage=AMessage;
    m_dwPort    =a_iPort;
    CSocketRoot::Initialize(stTCP);
    SOCKADDR sa;
    InitAddr(&sa, _T("0.0.0.0"), a_iPort);
    bind(m_hSocket,&sa,sizeof(sa));
    bool bResult=(listen(this->m_hSocket,SOMAXCONN)!=SOCKET_ERROR);
    //m_pAcceptThread=AfxBeginThread(AcceptThread,this,THREAD_PRIORITY_NORMAL);
    unsigned int hThreadID=0;
    //m_hAcceptThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)AcceptThread,this,0,&hThreadID);
    m_hAcceptThread=(HANDLE)_beginthreadex(NULL,0,AcceptThread,this,CREATE_SUSPENDED,&hThreadID);
    ResumeThread(m_hAcceptThread);
    return bResult;
};

int CServerSocket::Close()
{
    CClientSocket* pSocket=NULL;
    int iCount=(int)m_aClientSocket.GetItemCount();
    for(int i=iCount-1;i>=0;i--){
        pSocket=(CClientSocket*)m_aClientSocket.GetValue(i);
        if(pSocket && pSocket->GetSocket()){
            pSocket->Close();
        }
        m_aClientSocket.DelItem(i);
    }
    int iResult=CSocketRoot::Close();
    if(m_hAcceptThread){
        WaitForSingleObject(m_hAcceptThread,2000);
    }
    return iResult;
}

// 开始线程
// 2007.8.2
UINT _stdcall CServerSocket::AcceptThread(LPVOID AParam)
{
    CServerSocket* me=(CServerSocket*)AParam;
    try{
        sockaddr_in siRemote;
        int iLen=sizeof(siRemote);
        while (me->m_hSocket) {
            SOCKET hSocket;
            hSocket=accept(me->m_hSocket,(sockaddr*)&siRemote,&iLen);
            if(hSocket==INVALID_SOCKET){
                if(me->m_bOutputLog) OutputDebugString(_T("Accept 句柄指针为 INVALID_SOCKET"));
                break;
                //continue;
            }
            if(me->m_pServerMessage && me->m_hSocket){
                // 查找一个空的连接，如果没有则创建一个新的连接
                CClientSocket* pSocket=me->GetNeverUseSocket();
                //CClientSocket* pSocket=new CClientSocket;
                //pSocket->Create(me->m_pServerMessage->ClientMessageFactory(pSocket),true);

                pSocket->SetSocket(hSocket);
                pSocket->SetRemoteAddr(siRemote);
                pSocket->GetMessageObject()->OnConnect(pSocket);
                //AfxBeginThread(CClientSocket::ReceiveThread,pSocket,THREAD_PRIORITY_NORMAL);

                //DWORD hThreadID=0;
                //CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CClientSocket::ReceiveThread,pSocket,0,&hThreadID);
                pSocket->BeginReceiveData();
                if(me->m_bOutputLog) OutputDebugString(Format(_T("创建新的接收数据线程 id=%4.4d"),pSocket->GetReceiveThreadID()));
            }
        }
    }
    catch (...) {
        OutputDebugString(_T("Accept 出错"));
    }
    CloseHandle(me->m_hAcceptThread);
    me->m_hAcceptThread=NULL;
    //ExitThread(0);
    return 0;
}

CUDPSocket::CUDPSocket()
{
    //m_pReceiveThread=NULL;
    m_hReceiveThread=NULL;
    m_dwThreadID    =NULL;
    m_dwRecvBuffSize=SOCKET_RECEIVE_DATA_BUFSIZE;
    
    m_pBuffer       =new char[m_dwRecvBuffSize+1];
}

CUDPSocket::~CUDPSocket()
{
    if(m_pBuffer){
        delete[] m_pBuffer;
        m_pBuffer=NULL;
    }
}

bool CUDPSocket::Create(CUDPMessage* AMessage,int a_iPort)
{
    m_pMessage=AMessage;
    CSocketRoot::Initialize(stUDP);
    SOCKADDR sa;
    InitAddr(&sa, _T("0.0.0.0"), a_iPort);
    bool bResult=Bool(::bind(m_hSocket,&sa,sizeof(sa))!=SOCKET_ERROR);
    return bResult;
}

int CUDPSocket::Close()
{
    if(m_pMessage){
        m_pMessage->OnClose(this);
    }                                      
    return CSocketRoot::Close();
}

int CUDPSocket::ReceiveFrom(char FAR * buf, int len, int flags, struct sockaddr FAR *from, int FAR * fromlen)
{
    return recvfrom(m_hSocket,buf,len,flags,from,fromlen);
}

int CUDPSocket::SendTo(byte* a_pSend,int a_iLen,LPCTSTR a_sAddr,int a_iPort)
{
    if( _tcslen( a_sAddr ) == 0 || a_iPort == 0 )
        return SOCKET_ERROR;
    struct sockaddr_in daddr;
    memset((void *)&daddr,0,sizeof(daddr));
    daddr.sin_family=AF_INET;
    daddr.sin_port=htons(a_iPort);
#ifdef _UNICODE
    string s=WideToMutilByte(a_sAddr);
    daddr.sin_addr.s_addr=inet_addr(s.c_str());
#else
    daddr.sin_addr.s_addr=inet_addr(a_sAddr);
#endif

    return sendto(m_hSocket,(char*)a_pSend,a_iLen,0,(struct sockaddr *)&daddr,sizeof(daddr));
}


// 开始线程
// 2007.8.2
UINT _stdcall CUDPSocket::ReceiveThread(LPVOID AParam)
{
    CUDPSocket* me=(CUDPSocket*)AParam;
    try{
        while (true) {
            sockaddr_in siRemote;
            int iFromlen=sizeof(siRemote);
            int iReceive=recvfrom(me->m_hSocket,me->m_pBuffer,me->m_dwRecvBuffSize,0,(sockaddr*)&siRemote,&iFromlen);
            if(iReceive<0) {
                me->Close();
                break;
            }
            if(me->m_pMessage && me->m_hSocket){
                me->m_pBuffer[iReceive]=0;
                me->m_pMessage->OnReceiveFrom(me,me->m_pBuffer,iReceive,(sockaddr*)&siRemote);
            }
        }
    }
    catch (...) {
    }
    me->Stop();
    //ExitThread(0);
    CloseHandle(me->m_hReceiveThread);
    me->m_hReceiveThread=NULL;
    return 0;
}

// 设置接收缓冲区大小
// 2007.11.9
void CUDPSocket::SetRecvBuffSize(DWORD a_dwSize)
{
    if(m_pBuffer){
        delete[] m_pBuffer;
        m_pBuffer=NULL;
    }
    m_dwRecvBuffSize=a_dwSize;
    m_pBuffer=new char[m_dwRecvBuffSize+1];
}

// 设置接收文本
// 2007.11.9
void CUDPSocket::SetRecvText()
{
    m_bReveText=true;
}

// 设置接收二进制数据
// 2007.11.9
void CUDPSocket::SetRecvBin()
{
    m_bReveText=false;
}

// 获得一个以回车换行结束的文本
// 2007.8.8
string CUDPSocket::ReceiveText()
{
    string sResult("");
    char aBuff[MAX_PATH+1]={0};
    sockaddr_in siRemote;
    int iFromlen=sizeof(siRemote);
    int iReceive=ReceiveFrom(aBuff,MAX_PATH,0,(sockaddr*)&siRemote,&iFromlen);
    if(iReceive<0){
        return ("");
    }
    aBuff[iReceive]=0;
    sResult+=aBuff;
    while(true){
        if(sResult.find("\r\n")>=0){
            break;
        }
        iReceive=ReceiveFrom(aBuff,MAX_PATH,0,(sockaddr*)&siRemote,&iFromlen);
        if(iReceive<0){
            return ("");
        }
        aBuff[iReceive]=0;
        sResult+=aBuff;
    }
    return sResult;
}


}
