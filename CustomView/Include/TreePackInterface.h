//////////////////////////////////////////////////////////////////////////
// 名称：通用树型数据结构
// 功能：可以存放各种不同的数据到指定结构中，该结构可以支持自我解析
//       主要用来替代 xml 保存数据，相比较 xml 来讲，本结构极大的提
//       高了解析时的运行速度和写入速度，对运行时效率有很大程度的提
//       升
// 说明：
// 写入数据示例：
// CPackTreeBase* dp=CreateTreePackObject("Boot");
// dp->SetInt("BootInt",2);
// dp->SetByte("BootByte",1);
// dp->SetByte("BootByte2",3);
// dp->SetString("BootStr","str1");
// CPackTreeBase* p1=CreateTreePackObject("p1");
// p1->SetInt("p1-Int",34343);
// p1->SetByte("p1-byte",3);
// p1->SetString("p1-string","str1");
// CPackTreeBase* p3=CreateTreePackObject("p3");
// p3->SetInt("p3-int",2);
// p3->SetByte("p3-byte",1);
// p3->SetByte("p3-byte",3);
// p3->SetString("p3-string","str1");
// p1->AddChild(p3);
// dp->AddChild(p1);
// CPackTreeBase* p2=CreateTreePackObject("p2");
// p2->SetInt("p2-int",2);
// p2->SetByte("p2-byte",1);
// p2->SetByte("p2-byte",3);
// p2->SetString("p2-string","str1");
// CBMP bmp;
// bmp.ReadDIBFile(GetAppExePath()+"image.bmp");
// p2->SetImage("bmp",bmp.GetWidth(),bmp.GetHeight(),bmp.GetBitCount(),bmp.GetImageAddress());
// RECT r;
// r.left=50;
// r.top=50;
// r.right=150;
// r.bottom=150;
// p2->SetImageOfRect("bmpOfRect",bmp.GetWidth(),bmp.GetHeight(),bmp.GetBitCount(),bmp.GetImageAddress(),r,IT_DIB);
// dp->AddChild(p2);
// CPackTreeBase* p21=CreateTreePackObject("p21");
// p21->SetName("p21");
// p2->AddChild(p21);
//
// 该数据的结构为：
// Boot
//   ┃
//   ┣ p1
//   ┃ ┃
//   ┃ ┗ p3
//   ┃
//   ┗ p2
//      ┃
//      ┗ p21
// 
// 读取数据示例：
// enum SUN_DATA_PACK_TYPE{
//     DP_BYTE=0,          // 字节
//     DP_WORD,            // 字
//     DP_DWORD,           // 双字
//     DP_INT,             // 整形
//     DP_FLOAT,           // 单精度浮点
//     DP_DOUBLE,          // 双精度浮点
//     DP_INT64,           // 四字整数
//     DP_LONGDOUBLE,      // 长浮点(10字节)
//     DP_STRING,          // 字符串
//     DP_TEXT,            // 长文本字符串
//     DP_BINARY,          // 二进制
//     DP_IMAGE,           // 图像
//     DP_POINT            // 指针
// };
// BOOL EnumTreePack(IN CPackTreeBase* a_pPack,OUT CStringArray& a_aOut,OUT int& a_iLevel)
// {
//     // 先枚举自己的属性
//     CDataItem* pData=a_pPack->Begin();
//     _TCHAR aSpace[MAX_PATH]={0};
//     if(a_iLevel>0) memset(aSpace,' ',a_iLevel*4);
//     while(pData){
//         CString sValue;
//         switch (pData->m_dpType)
//         {
//         case DP_BYTE      :
//             sValue.Format("%d",*(byte*)pData->m_pData);
//             break;
//         case DP_WORD      :
//             sValue.Format("%d",*(WORD*)pData->m_pData);
//             break;
//         case DP_DWORD     :
//             sValue.Format("%d",*(DWORD*)pData->m_pData);
//             break;
//         case DP_INT       :
//             sValue.Format("%d",*(int*)pData->m_pData);
//             break;
//         case DP_FLOAT     :
//             sValue.Format("%f",*(float*)pData->m_pData);
//             break;
//         case DP_DOUBLE    :
//             sValue.Format("%f",*(double*)pData->m_pData);
//             break;
//         case DP_INT64     :
//             sValue.Format("%d",*(INT64*)pData->m_pData);
//             break;
//         case DP_LONGDOUBLE:
//             sValue.Format("%d",*(long double*)pData->m_pData);
//             break;
//         case DP_STRING    :
//             sValue.Format("%s",(_TCHAR*)pData->m_pData);
//             break;
//         case DP_TEXT    :
//             sValue.Format("%s",(_TCHAR*)pData->m_pData);
//             break;
//         case DP_BINARY    :
//             sValue.Format("二进制数据");
//             break;
//         case DP_IMAGE     :
//             sValue.Format("图像数据");
//             break;
//         case DP_POINT     :
//             sValue.Format("%8.8X",*(__int64*)pData->m_pData);
//             break;
//         }
//         CString sTemp;
//         sTemp.Format("%s%s=\"%s\"",aSpace,pData->m_sName,sValue);
//         a_aOut.Add(sTemp);
//         pData=a_pPack->Next();
//     }
//     // 再枚举子节点
//     CPackTreeBase* pTree=a_pPack->ChildBegin();
//     while(pTree){
//         CString sTemp;
//         sTemp.Format("%s%s(节点)",aSpace,pTree->GetName());
//         a_aOut.Add(sTemp);
//         a_iLevel++;
//         EnumTreePack(pTree,a_aOut,a_iLevel);
//         pTree=a_pPack->ChildNext();
//     }
//     a_iLevel--;
//     return TRUE;
// }
// 
// 实际代码如下：
// int iLevel=0;
// CStringArray aInfo;
// EnumTreePack(dp,aInfo,iLevel);
// CString sInfo;
// for(int i=0;i<aInfo.GetSize();i++){
//     sInfo+=(aInfo.GetAt(i)+"\r\n");
// }
// AfxMessageBox(sInfo);
//
// 取指定数据用 FindChild 进行查找，找到后，用 GetXXX 指令直接取值即可
// CPackTreeBase* p1=dp->FindChild("p1");
// if(p1) p1->GetInt("age2");
// 
// 注意：
// 1.所有使用 AddChild 方法加入到其他节点中的指针不能手工销毁(根节点需要手工销毁)
// 2.所有节点或数据都必须有名称，不允许重名，否则数据会自动叠加并且导致内存泄漏
// 3.子节点和数据名称中不能包含“空格”、“尖括号”、“问号”、“双引号”、“回车”
//   、“换行”、“跳格”等字符
// 4.字符串类型的数据中不能包含“尖括号”
// 5.使用 Text 类型不需要考虑其中的符号问题，除非里面出现了“<![CDATA[”或“]]>”
// 6.Lock 和 Unlock 指令用来进行数据同步，如果有多层数据，请尽量使用根节点的锁定和解锁指令
// 
// 
// 作者：梅文海
// 日期：2009.3.4 - 2011.1.17
// 历史记录：
// v1.0
// 2011.4.13
// 1.解决了删除属性时会先删除指针，后删除数据，因此删除数据的时候无法得到名称
// 2.加载数据和保存数据失败时不能正确返回假
// 3.新增了查询属性的方法
// 
// v1.1
// 2011.4.14
// 1.新增了设置属性说明的方法
// 
// v1.2
// 2011.4.15
// 1.新增了获取父节点的功能
// 
// v1.3
// 2011.11.3
// 1.新增了 TranTreePackToXML 的新版本，注意每次调用该函数一定要设置 a_iLen 和 a_iLevel 为 0
// 2.新增了 所有方法的 API 调用，使得其他语言也能够使用通用数据包
// 
// v1.4
// 2012.5.25
// 1.新增获取参数默认值
// 如：byte GetByte(LPCTSTR a_sName,const byte a_iDefValue);
// 
// v1.5
// 2013.5.30
// 解决整个数据包不能大于 2G 的限制
// 
// v1.6
// 2014.1.23
// 1.新增了 SetImageOfRect 函数，可以直接根据大图设置图像
// 
// v1.7
// 2014.3.11
// 1.修复了改变节点名称导致的存储后文件变大问题
// 
// v1.8
// 2014.3.13
// 1.修复了 SetImageOfRect 函数会导致 mtp 文件变大的问题
// 
// v1.9
// 2014.3.20
// 1.修复了节点的 Lock 和 Unlock 时内存泄露的问题
// 
// v1.91
// 2016.6.28
// 1.确保数据包在存储的时候如果出现了重命名失败时会自动重试
//
// v1.92
// 2016.7.16
// 1.增加了加载和保存时的进度功能
// 
// int iLevel=0;
// _TCHAR* pResult=NULL;
// if(true){
//     dp->TranTreePackToXML(&pResult,iLevel);
//     if(pResult){
//         m_sInfo+=pResult;
//         delete[] pResult;
//     }
// }
// else{
//     int iLen=0;
//     dp->TranTreePackToXML(NULL,iLevel,true,iLen,"gb2312");
//     if(iLen>0){
//         pResult=new _TCHAR[iLen+1];
//         pResult[iLen]=0;
//         iLen=0;
//         iLevel=0;
//         dp->TranTreePackToXML(pResult,iLevel,false,iLen,"gb2312");
//         //AfxMessageBox(Format("%d",iLen));
//         if(pResult){
//             m_sInfo+=pResult;
//             delete[] pResult;
//         }
//     }
// }
// 
//////////////////////////////////////////////////////////////////////////

#ifndef _treepackinterface_h
#define _treepackinterface_h

#pragma warning(disable:4786)
#pragma warning(disable:4788)

using namespace std;
namespace sunLib{

    // 数据包类型
    enum SUN_DATA_PACK_TYPE{
        DP_BYTE=0,          // 字节
        DP_WORD,            // 字
        DP_DWORD,           // 双字
        DP_INT,             // 整形
        DP_FLOAT,           // 单精度浮点
        DP_DOUBLE,          // 双精度浮点
        DP_INT64,           // 四字整数
        DP_LONGDOUBLE,      // 长浮点(10字节)
        DP_STRING,          // 字符串
        DP_TEXT,            // 长文本字符串(在生成 xml 时会自动放入 CData 字段)
        DP_BINARY,          // 二进制
        DP_IMAGE,           // 图像
        DP_POINT            // 指针
    };

    // 状态类型
    enum TREE_PACK_STATUS_TYPE{
        TPST_LOAD=0,        // 加载
        TPST_SAVE           // 保存
    };

    // 状态回调函数 2016.7.14
    typedef void (_stdcall* PTreePackStatusCallback)(void* a_pOwner,int a_iType,int a_iMin,int a_iMax,int a_iPos);

    // 图像类型
    enum SUN_IMAGE_TYPE{
        IT_LINE=0,          // 直接存储格式(线性存储，从上到下，不按4字节对齐)
        IT_DIB              // Windows 的标准位图格式，存储时每行按4字节对齐，数据从下到上
    };

    // 图像数据结构头
    struct CImageDataHeader
    {
        int             m_iWidth;       // 宽
        int             m_iHeight;      // 高
        int             m_iBit;         // 图像位数
        SUN_IMAGE_TYPE  m_itType;       // 图像格式
    };

    // 数据节点
    class CDataItem
    {
    public:
        int                 m_dpType;       // 数据类型
        _TCHAR              m_sName[260];   // 名称
        _TCHAR              m_sDesc[260];   // 说明
        byte*               m_pData;        // 数据
        __int64             m_iSize;        // 数据长度
        byte*               m_pCustom;      // 自定义数据
        __int64             m_iCustomSize;  // 自定义数据长度
        CDataItem(){
            m_pData=NULL;
            m_pCustom=NULL;
        }
        virtual ~CDataItem(){
            if(m_pData){
                delete[] m_pData;
                m_pData=NULL;
            }
            if(m_pCustom){
                delete[] m_pCustom;
                m_pCustom=NULL;
            }
        };
        __int64 CalcSize(){
            __int64 iSize=0;
            iSize += ((_tcslen(m_sName) + 1)*sizeof(TCHAR));      // Key 即 Name
            iSize+=sizeof(m_dpType);
            iSize += ((_tcslen(m_sDesc) + 1)*sizeof(TCHAR));
            iSize+=sizeof(m_iSize);
            iSize+=sizeof(m_iCustomSize);
            iSize+=m_iSize;
            iSize+=m_iCustomSize;
            return iSize;
        }
        void Assign(CDataItem* a_pOther){
            m_dpType    =a_pOther->m_dpType;
            _tcscpy_s(m_sName,260,a_pOther->m_sName);
            _tcscpy_s(m_sDesc,260,a_pOther->m_sDesc);
            m_iSize         =a_pOther->m_iSize;
            m_iCustomSize   =a_pOther->m_iCustomSize;
            m_pData=new byte[m_iSize];
            memcpy(m_pData,a_pOther->m_pData,m_iSize);
            m_pCustom=new byte[m_iCustomSize];
            memcpy(m_pCustom,a_pOther->m_pCustom,m_iCustomSize);
        }
    };

    class CPackTreeBase  
    {
    protected:
        CPackTreeBase(){};
        CPackTreeBase(LPCTSTR a_sName){};
    public:
        virtual ~CPackTreeBase(){};

        virtual void Lock()=NULL;           // 锁定
        virtual void Unlock()=NULL;         // 解锁

        virtual BOOL DeclareByte      (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // 定义字节变量
        virtual BOOL DeclareWord      (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // 定义字变量
        virtual BOOL DeclareDword     (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // 定义双字变量
        virtual BOOL DeclareInt       (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // 定义整数变量
        virtual BOOL DeclareFloat     (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // 定义单精度浮点数
        virtual BOOL DeclareDouble    (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // 定义双精度浮点数
        virtual BOOL DeclareInt64     (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // 定义 64 位整数
        virtual BOOL DeclareLongDouble(LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // 定义长双精度浮点数
        virtual BOOL DeclareString    (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // 定义字符串
        virtual BOOL DeclareText      (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // 定义字符串（在转换为 xml 时可以打包进 DATA 区域，即允许特殊字符）
        virtual BOOL DeclareBinary    (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // 定义二进制数据
        virtual BOOL DeclareImage     (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // 定义图像
        virtual BOOL DeclarePoint     (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // 定义指针

        virtual BOOL SetByte      (LPCTSTR a_sName,byte       value)=NULL;               // 设置字节变量
        virtual BOOL SetWord      (LPCTSTR a_sName,WORD       value)=NULL;               // 设置字变量
        virtual BOOL SetDword     (LPCTSTR a_sName,DWORD      value)=NULL;               // 设置双字变量
        virtual BOOL SetInt       (LPCTSTR a_sName,int        value)=NULL;               // 设置整数变量
        virtual BOOL SetFloat     (LPCTSTR a_sName,float      value)=NULL;               // 设置单精度浮点数
        virtual BOOL SetDouble    (LPCTSTR a_sName,double     value)=NULL;               // 设置双精度浮点数
        virtual BOOL SetInt64     (LPCTSTR a_sName,__int64    value)=NULL;               // 设置 64 位整数
        virtual BOOL SetLongDouble(LPCTSTR a_sName,long double value)=NULL;              // 设置长双精度浮点数
        virtual BOOL SetString    (LPCTSTR a_sName,LPCTSTR     value)=NULL;              // 设置字符串
        virtual BOOL SetText      (LPCTSTR a_sName,LPCTSTR     value)=NULL;              // 设置字符串
        virtual BOOL SetBinary    (LPCTSTR a_sName,byte*      value,__int64 a_iSize,byte* custom=NULL,__int64 a_iCustomSize=0)=NULL;                        // 设置二进制数据
        virtual BOOL SetImage     (LPCTSTR a_sName,int a_iWidth,int a_iHeight,int a_iBit,byte* a_pImage,SUN_IMAGE_TYPE a_itType=IT_LINE)=NULL;              // 设置图像
        // 设置一个指定位置的图像 2014.1.23
        virtual BOOL SetImageOfRect(LPCTSTR a_sName,int a_iWidth,int a_iHeight,int a_iBit,byte* a_pImage,RECT a_rRect,SUN_IMAGE_TYPE a_itType=IT_LINE)=NULL;
        virtual BOOL SetPoint     (LPCTSTR a_sName,void*      value)=NULL;                                                                                  // 设置指针

        virtual byte        GetByte      (LPCTSTR a_sName,const byte        a_iDefValue)=NULL;          // 获取数据，下同
        virtual WORD        GetWord      (LPCTSTR a_sName,const WORD        a_iDefValue)=NULL;          //
        virtual DWORD       GetDword     (LPCTSTR a_sName,const DWORD       a_iDefValue)=NULL;          //
        virtual INT         GetInt       (LPCTSTR a_sName,const INT         a_iDefValue)=NULL;          //
        virtual FLOAT       GetFloat     (LPCTSTR a_sName,const FLOAT       a_fDefValue)=NULL;          //
        virtual double      GetDouble    (LPCTSTR a_sName,const double      a_fDefValue)=NULL;          //
        virtual __int64     GetInt64     (LPCTSTR a_sName,const __int64     a_iDefValue)=NULL;          //
        virtual long double GetLongDouble(LPCTSTR a_sName,const long double a_fDefValue)=NULL;          //
        virtual LPCTSTR      GetString    (LPCTSTR a_sName)=NULL;                                       //
        virtual LPCTSTR      GetText      (LPCTSTR a_sName)=NULL;                                       //
        virtual byte*       GetBinary    (LPCTSTR a_sName,OUT __int64& a_iSize)=NULL;                   //
        virtual byte*       GetImage     (LPCTSTR a_sName,OUT CImageDataHeader& a_ithImageInfo)=NULL;   //
        virtual void*       GetPoint     (LPCTSTR a_sName,void* a_pDefValue)=NULL;                      //

        virtual BOOL Clear()=NULL;                                                  // 清除所有数据
        virtual BOOL Delete(LPCTSTR a_sName)=NULL;                                  // 删除一条记录
        virtual INT GetCount()=NULL;                                                // 获得所有记录数
        virtual CPackTreeBase* GetParent()=NULL;                                    // 获得父节点
        virtual CDataItem* Find(LPCTSTR a_sName)=NULL;                              // 获得指定名称的数据指针
        virtual CDataItem* Begin()=NULL;                                            // 获得数据开始指针
        virtual CDataItem* Next()=NULL;                                             // 获得数据结束指针
        virtual void SetName(LPCTSTR a_sName)=NULL;                                 // 设置名称
        virtual LPCTSTR GetName()=NULL;                                             // 获取名称
        virtual void SetParamDesc(LPCTSTR a_sName,LPCTSTR a_sDesc)=NULL;            // 设置属性的说明
        virtual __int64 CalcDataSize()=NULL;                                        // 计算全部数据的长度(即存储所需的空间)
        virtual BOOL SaveToMemory(byte** a_pBuf,OUT __int64* a_pSize)=NULL;         // 保存全部数据到内存(先设置内存大小)
        virtual BOOL LoadFromMemory(byte* a_pBuf,__int64 a_iSize)=NULL;             // 从内存加载数据
        virtual BOOL SaveToFile(LPCTSTR a_sFileName)=NULL;                          // 保存全部数据到文件
        virtual BOOL LoadFromFile(LPCTSTR a_sFileName)=NULL;                        // 从文件中加载数据
        virtual BOOL SaveToFile(LPCTSTR a_sFileName,void* a_pOwner,PTreePackStatusCallback a_pCallback)=NULL;                       // 保存全部数据到文件
        virtual BOOL LoadFromFile(LPCTSTR a_sFileName,void* a_pOwner,PTreePackStatusCallback a_pCallback)=NULL;                     // 从文件中加载数据

        // 子节点
        virtual void AddChild(CPackTreeBase* a_pChild)=NULL;                        // 添加一个子节点
        virtual BOOL ClearChild()=NULL;                                             // 清空子节点
        virtual BOOL DeleteChild(LPCTSTR a_sName)=NULL;                             // 删除一条子节点
        virtual INT GetChildCount()=NULL;                                           // 获得子节点数目
        virtual CPackTreeBase* ChildBegin()=NULL;                                   // 获得子节点开始指针
        virtual CPackTreeBase* ChildNext()=NULL;                                    // 获得子节点结束指针
        virtual CPackTreeBase* FindChild(LPCTSTR a_sName)=NULL;                     // 找一个子节点
        virtual BOOL TranTreePackToXML(OUT _TCHAR** a_sOut,OUT int& a_iLevel,LPCTSTR a_sEncoding=_T("GB2312"))=NULL;  // 转换为 xml 格式数据,请自行释放申请的空间
        // 转换为 xml 格式，第一次调用先设置 a_bGetSize 为真获取所需的内存大小，第二次调用设置 a_bGetSize，并且设置 a_iLen和 a_iLevel 为 0
        virtual BOOL TranTreePackToXML(OUT _TCHAR* a_sOut,OUT int& a_iLevel,bool a_bGetSize,int& a_iLen,LPCTSTR a_sEncoding)=NULL;
        // 新增功能
        virtual CPackTreeBase* CreateInstance()=NULL;                               // 创建实例
        virtual void Assign(CPackTreeBase* a_pSource)=NULL;                         // 复制所有数据

        virtual INT CalcDataSize32()=NULL;                                          // 计算全部数据的长度(32位数据长度即存储所需的空间)
        virtual BOOL LoadFromMemory32(byte* a_pBuf,int a_iSize)=NULL;               // 从内存加载数据(32位数据长度)
        virtual BOOL LoadFromFile32(LPCTSTR a_sFileName)=NULL;                      // 从文件中加载数据(32位数据长度)
        virtual void Destroy()=NULL;                                                // 销毁 2018.8.12
        virtual BOOL DeclareData(LPCTSTR a_sName,SUN_DATA_PACK_TYPE a_dpType,LPCTSTR a_sDesc)=NULL;                                                         // 定义数据 2018.11.16
        virtual BOOL SetData(LPCTSTR a_sName,SUN_DATA_PACK_TYPE a_dpType,byte* a_pData,__int64 a_iSize,byte* custom=NULL,__int64 a_iCustomSize=0)=NULL;     // 设置数据值 2018.11.16
        virtual CDataItem* GetData(LPCTSTR a_sName,SUN_DATA_PACK_TYPE a_dpType)=NULL;                                                                       // 获取属性数据 2018.11.16
        virtual CPackTreeBase* DivideChild(LPCTSTR a_sName)=NULL;                                                                                           // 解离一个子节点（和删除的区别是解离不会销毁节点数据），同时返回该节点指针 2018.11.16
    protected:

    };

    // 创建通用数据结构对象
    // 2011.1.12
    class CTreePackManage{
    public:
        typedef CPackTreeBase* (__stdcall* PCreateTreePackObject)(LPCTSTR);
        typedef void (_stdcall* PDestroyTreePackObject)(HANDLE a_hHnd);
        CTreePackManage(){
            m_pCreateTreePack=NULL;
            m_pDestroyTreePack=NULL;
            m_bLoad     =false;
            m_hDLL=GetModuleHandle(_T("TreePackModule.dll"));
            if(!m_hDLL){
                CString sDLLFile=GetAppExePath()+_T("TreePackModule.dll");
                //string sDLLFile="d:\\TreePackModule.dl_T(";
                m_hDLL=LoadLibrary(sDLLFile);
                if(!m_hDLL){
                    return;
                }
                m_bLoad=true;
            }
            m_pCreateTreePack=(PCreateTreePackObject)GetProcAddress(m_hDLL,"CreateTreePack");
            m_pDestroyTreePack=(PDestroyTreePackObject)GetProcAddress(m_hDLL,"ExpDestroyTreePack");
            ASSERT(m_pCreateTreePack && m_pDestroyTreePack);
        };
        virtual ~CTreePackManage(){
            Destroy();
        };
        CPackTreeBase* CreateTreePackObject(LPCTSTR a_sName){
            if(m_pCreateTreePack){
                return m_pCreateTreePack(a_sName);
            }
            return NULL;
        }
        BOOL DestroyTreePackObject(CPackTreeBase* a_pPack){
            if(m_pDestroyTreePack && a_pPack) {
                m_pDestroyTreePack((HANDLE)a_pPack);
                return TRUE;
            }
            return FALSE;
        }
        void Destroy(){
            if(m_hDLL && m_bLoad) {
                FreeLibrary(m_hDLL);
                m_hDLL=NULL;
                m_bLoad=false;
            }
        }
    protected:
        HMODULE m_hDLL;
        bool    m_bLoad;
        PCreateTreePackObject   m_pCreateTreePack;
        PDestroyTreePackObject  m_pDestroyTreePack;
    };



    // API 接口
    typedef HANDLE (_stdcall* PExpCreateTreePack)(LPCTSTR);         // 创建数据
    typedef void (_stdcall* PExpDestroyTreePack)(HANDLE a_hHnd);    // 释放数据包
    typedef void (_stdcall* PExpLock)(HANDLE a_hHnd);               // 锁定
    typedef void (_stdcall* PExpUnlock)(HANDLE a_hHnd);             // 解锁
    typedef BOOL (_stdcall* PExpDeclareByte)(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);            // 定义字节变量
    typedef BOOL (_stdcall* PExpDeclareWord      )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // 定义字变量
    typedef BOOL (_stdcall* PExpDeclareDword     )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // 定义双字变量
    typedef BOOL (_stdcall* PExpDeclareInt       )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // 定义整数变量
    typedef BOOL (_stdcall* PExpDeclareFloat     )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // 定义单精度浮点数
    typedef BOOL (_stdcall* PExpDeclareDouble    )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // 定义双精度浮点数
    typedef BOOL (_stdcall* PExpDeclareInt64     )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // 定义 64 位整数
    typedef BOOL (_stdcall* PExpDeclareLongDouble)(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // 定义长双精度浮点数
    typedef BOOL (_stdcall* PExpDeclareString    )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // 定义字符串
    typedef BOOL (_stdcall* PExpDeclareText      )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // 定义字符串（在转换为 xml 时可以打包进 DATA 区域，即允许特殊字符）
    typedef BOOL (_stdcall* PExpDeclareBinary    )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // 定义二进制数据
    typedef BOOL (_stdcall* PExpDeclareImage     )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // 定义图像
    typedef BOOL (_stdcall* PExpDeclarePoint     )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // 定义指针

    typedef BOOL (_stdcall* PExpSetByte      )(HANDLE a_hHnd,LPCTSTR a_sName,byte       value);         // 设置字节变量
    typedef BOOL (_stdcall* PExpSetWord      )(HANDLE a_hHnd,LPCTSTR a_sName,WORD       value);         // 设置字变量
    typedef BOOL (_stdcall* PExpSetDword     )(HANDLE a_hHnd,LPCTSTR a_sName,DWORD      value);         // 设置双字变量
    typedef BOOL (_stdcall* PExpSetInt       )(HANDLE a_hHnd,LPCTSTR a_sName,int        value);         // 设置整数变量
    typedef BOOL (_stdcall* PExpSetFloat     )(HANDLE a_hHnd,LPCTSTR a_sName,float      value);         // 设置单精度浮点数
    typedef BOOL (_stdcall* PExpSetDouble    )(HANDLE a_hHnd,LPCTSTR a_sName,double     value);         // 设置双精度浮点数
    typedef BOOL (_stdcall* PExpSetInt64     )(HANDLE a_hHnd,LPCTSTR a_sName,__int64    value);         // 设置 64 位整数
    typedef BOOL (_stdcall* PExpSetLongDouble)(HANDLE a_hHnd,LPCTSTR a_sName,long double value);        // 设置长双精度浮点数
    typedef BOOL (_stdcall* PExpSetString    )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR     value);        // 设置字符串
    typedef BOOL (_stdcall* PExpSetText      )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR     value);        // 设置字符串
    typedef BOOL (_stdcall* PExpSetBinary    )(HANDLE a_hHnd,LPCTSTR a_sName,byte*      value,__int64 a_iSize,byte* custom,__int64 a_iCustomSize);                          // 设置图像
    typedef BOOL (_stdcall* PExpSetImage     )(HANDLE a_hHnd,LPCTSTR a_sName,int a_iWidth,int a_iHeight,int a_iBit,byte* a_pImage,SUN_IMAGE_TYPE a_itType);                 // 根据区域设置图像
    typedef BOOL (_stdcall* PExpSetImageOfRect)(HANDLE a_hHnd,LPCTSTR a_sName,int a_iWidth,int a_iHeight,int a_iBit,byte* a_pImage,RECT a_rRect,SUN_IMAGE_TYPE a_itType);   // 设置指针
    typedef BOOL (_stdcall* PExpSetPoint     )(HANDLE a_hHnd,LPCTSTR a_sName,void*      value);

    typedef byte        (_stdcall* PExpGetByte      )(HANDLE a_hHnd,LPCTSTR a_sName,const byte        a_iDefValue);     // 获取数据，下同
    typedef WORD        (_stdcall* PExpGetWord      )(HANDLE a_hHnd,LPCTSTR a_sName,const WORD        a_iDefValue);
    typedef DWORD       (_stdcall* PExpGetDword     )(HANDLE a_hHnd,LPCTSTR a_sName,const DWORD       a_iDefValue);
    typedef INT         (_stdcall* PExpGetInt       )(HANDLE a_hHnd,LPCTSTR a_sName,const INT         a_iDefValue);
    typedef FLOAT       (_stdcall* PExpGetFloat     )(HANDLE a_hHnd,LPCTSTR a_sName,const FLOAT       a_fDefValue);
    typedef double      (_stdcall* PExpGetDouble    )(HANDLE a_hHnd,LPCTSTR a_sName,const double      a_fDefValue);
    typedef __int64     (_stdcall* PExpGetInt64     )(HANDLE a_hHnd,LPCTSTR a_sName,const __int64     a_iDefValue);
    typedef long double (_stdcall* PExpGetLongDouble)(HANDLE a_hHnd,LPCTSTR a_sName,const long double a_fDefValue);
    typedef LPCTSTR      (_stdcall* PExpGetString    )(HANDLE a_hHnd,LPCTSTR a_sName);
    typedef LPCTSTR      (_stdcall* PExpGetText      )(HANDLE a_hHnd,LPCTSTR a_sName);
    typedef byte*       (_stdcall* PExpGetBinary    )(HANDLE a_hHnd,LPCTSTR a_sName,OUT __int64* a_iSize);
    typedef byte*       (_stdcall* PExpGetImage )(HANDLE a_hHnd,LPCTSTR a_sName,OUT CImageDataHeader* a_ithImageInfo);
    typedef void*       (_stdcall* PExpGetPoint     )(HANDLE a_hHnd,LPCTSTR a_sName,void* a_pDefValue);

    typedef BOOL    (_stdcall* PExpClear)(HANDLE a_hHnd);                                                   // 清除所有数据
    typedef BOOL    (_stdcall* PExpDelete)(HANDLE a_hHnd,LPCTSTR a_sName);                                  // 删除一条记录
    typedef INT     (_stdcall* PExpGetCount)(HANDLE a_hHnd);                                                // 获得所有记录数
    typedef HANDLE  (_stdcall* PExpGetParent)(HANDLE a_hHnd);                                               // 获得父节点
    typedef CDataItem*  (_stdcall* PExpFind)(HANDLE a_hHnd,LPCTSTR a_sName);                                // 找一个属性
    typedef CDataItem*  (_stdcall* PExpBegin)(HANDLE a_hHnd);                                               // 获得属性开始指针
    typedef CDataItem*  (_stdcall* PExpNext)(HANDLE a_hHnd);                                                // 获得属性结束指针
    typedef void    (_stdcall* PExpSetName)(HANDLE a_hHnd,LPCTSTR a_sName);                                 // 设置名称
    typedef LPCTSTR  (_stdcall* PExpGetName)(HANDLE a_hHnd);                                                // 获取名称
    typedef void    (_stdcall* PExpSetParamDesc)(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);            // 设置属性的说明
    typedef __int64 (_stdcall* PExpCalcDataSize)(HANDLE a_hHnd);                                            // 计算全部数据的长度(即存储所需的空间)
    typedef BOOL    (_stdcall* PExpSaveToMemory)(HANDLE a_hHnd,OUT byte** a_pBuf,OUT __int64* a_iSize);     // 保存全部数据到内存(要求先分配内存)
    typedef BOOL    (_stdcall* PExpLoadFromMemory)(HANDLE a_hHnd,byte* a_pBuf,int a_iSize);                 // 从内存加载数据
    typedef BOOL    (_stdcall* PExpSaveToFile)(HANDLE a_hHnd,LPCTSTR a_sFileName);                          // 保存全部数据到文件
    typedef BOOL    (_stdcall* PExpLoadFromFile)(HANDLE a_hHnd,LPCTSTR a_sFileName);                        // 从文件中加载数据
    typedef BOOL    (_stdcall* PExpSaveToFileEx)(HANDLE a_hHnd,LPCTSTR a_sFileName,void* a_pOwner,PTreePackStatusCallback a_pCallback);                       // 保存全部数据到文件
    typedef BOOL    (_stdcall* PExpLoadFromFileEx)(HANDLE a_hHnd,LPCTSTR a_sFileName,void* a_pOwner,PTreePackStatusCallback a_pCallback);                     // 从文件中加载数据
    // 子节点
    typedef void    (_stdcall* PExpAddChild)(HANDLE a_hHnd,HANDLE a_hChild);                                // 添加一个子节点
    typedef BOOL    (_stdcall* PExpClearChild)(HANDLE a_hHnd);                                              // 清空子节点
    typedef BOOL    (_stdcall* PExpDeleteChild)(HANDLE a_hHnd,LPCTSTR a_sName);                             // 删除一条子节点
    typedef INT     (_stdcall* PExpGetChildCount)(HANDLE a_hHnd);                                           // 获得子节点数目
    typedef HANDLE  (_stdcall* PExpChildBegin)(HANDLE a_hHnd);                                              // 获得子节点开始指针
    typedef HANDLE  (_stdcall* PExpChildNext)(HANDLE a_hHnd);                                               // 获得子节点结束指针
    typedef HANDLE  (_stdcall* PExpFindChild)(HANDLE a_hHnd,LPCTSTR a_sName);                               // 找一个子节点
    typedef BOOL    (_stdcall* PExpTranTreePackToXML)(HANDLE a_hHnd,OUT _TCHAR** a_sOut,OUT int* a_iLevel,LPCTSTR a_sEncoding);     // 转换为 xml 格式数据,请自行释放申请的空间
    typedef BOOL    (_stdcall* PExpTranTreePackToXMLEx)(HANDLE a_hHnd,OUT _TCHAR* a_sOut,OUT int* a_iLevel,BOOL a_bGetSize,int* a_iLen,LPCTSTR a_sEncoding);
    // 新增功能
    typedef HANDLE  (_stdcall* PExpCreateInstance)(HANDLE a_hHnd);                                          // 创建实例
    typedef void    (_stdcall* PExpAssign)(HANDLE a_hHnd,HANDLE a_hFrom);                                   // 复制所有数据
    typedef void    (_stdcall* PExpDestroy)(HANDLE a_hHnd);                                                    // 销毁 2018.8.12
    typedef BOOL    (_stdcall* PExpDeclareData)(HANDLE a_hHnd,LPCTSTR a_sName,SUN_DATA_PACK_TYPE a_dpType,LPCTSTR a_sDesc);                                                     // 定义数据 2018.11.16
    typedef BOOL    (_stdcall* PExpSetData)(HANDLE a_hHnd,LPCTSTR a_sName,SUN_DATA_PACK_TYPE a_dpType,byte* a_pData,__int64 a_iSize,byte* custom,__int64 a_iCustomSize);        // 设置数据值 2018.11.16
    typedef CDataItem* (_stdcall* PExpGetData)(HANDLE a_hHnd,LPCTSTR a_sName,SUN_DATA_PACK_TYPE a_dpType);                                                                      // 获取属性数据 2018.11.16
    typedef CPackTreeBase* (_stdcall* PExpDivideChild)(HANDLE a_hHnd,LPCTSTR a_sName);                                                                                          // 解离一个子节点（和删除的区别是解离不会销毁节点数据），同时返回该节点指针 2018.11.16


}

#endif