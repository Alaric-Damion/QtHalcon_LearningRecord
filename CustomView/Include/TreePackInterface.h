//////////////////////////////////////////////////////////////////////////
// ���ƣ�ͨ���������ݽṹ
// ���ܣ����Դ�Ÿ��ֲ�ͬ�����ݵ�ָ���ṹ�У��ýṹ����֧�����ҽ���
//       ��Ҫ������� xml �������ݣ���Ƚ� xml ���������ṹ�������
//       ���˽���ʱ�������ٶȺ�д���ٶȣ�������ʱЧ���кܴ�̶ȵ���
//       ��
// ˵����
// д������ʾ����
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
// �����ݵĽṹΪ��
// Boot
//   ��
//   �� p1
//   �� ��
//   �� �� p3
//   ��
//   �� p2
//      ��
//      �� p21
// 
// ��ȡ����ʾ����
// enum SUN_DATA_PACK_TYPE{
//     DP_BYTE=0,          // �ֽ�
//     DP_WORD,            // ��
//     DP_DWORD,           // ˫��
//     DP_INT,             // ����
//     DP_FLOAT,           // �����ȸ���
//     DP_DOUBLE,          // ˫���ȸ���
//     DP_INT64,           // ��������
//     DP_LONGDOUBLE,      // ������(10�ֽ�)
//     DP_STRING,          // �ַ���
//     DP_TEXT,            // ���ı��ַ���
//     DP_BINARY,          // ������
//     DP_IMAGE,           // ͼ��
//     DP_POINT            // ָ��
// };
// BOOL EnumTreePack(IN CPackTreeBase* a_pPack,OUT CStringArray& a_aOut,OUT int& a_iLevel)
// {
//     // ��ö���Լ�������
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
//             sValue.Format("����������");
//             break;
//         case DP_IMAGE     :
//             sValue.Format("ͼ������");
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
//     // ��ö���ӽڵ�
//     CPackTreeBase* pTree=a_pPack->ChildBegin();
//     while(pTree){
//         CString sTemp;
//         sTemp.Format("%s%s(�ڵ�)",aSpace,pTree->GetName());
//         a_aOut.Add(sTemp);
//         a_iLevel++;
//         EnumTreePack(pTree,a_aOut,a_iLevel);
//         pTree=a_pPack->ChildNext();
//     }
//     a_iLevel--;
//     return TRUE;
// }
// 
// ʵ�ʴ������£�
// int iLevel=0;
// CStringArray aInfo;
// EnumTreePack(dp,aInfo,iLevel);
// CString sInfo;
// for(int i=0;i<aInfo.GetSize();i++){
//     sInfo+=(aInfo.GetAt(i)+"\r\n");
// }
// AfxMessageBox(sInfo);
//
// ȡָ�������� FindChild ���в��ң��ҵ����� GetXXX ָ��ֱ��ȡֵ����
// CPackTreeBase* p1=dp->FindChild("p1");
// if(p1) p1->GetInt("age2");
// 
// ע�⣺
// 1.����ʹ�� AddChild �������뵽�����ڵ��е�ָ�벻���ֹ�����(���ڵ���Ҫ�ֹ�����)
// 2.���нڵ�����ݶ����������ƣ��������������������ݻ��Զ����Ӳ��ҵ����ڴ�й©
// 3.�ӽڵ�����������в��ܰ������ո񡱡��������š������ʺš�����˫���š������س���
//   �������С��������񡱵��ַ�
// 4.�ַ������͵������в��ܰ����������š�
// 5.ʹ�� Text ���Ͳ���Ҫ�������еķ������⣬������������ˡ�<![CDATA[����]]>��
// 6.Lock �� Unlock ָ��������������ͬ��������ж�����ݣ��뾡��ʹ�ø��ڵ�������ͽ���ָ��
// 
// 
// ���ߣ�÷�ĺ�
// ���ڣ�2009.3.4 - 2011.1.17
// ��ʷ��¼��
// v1.0
// 2011.4.13
// 1.�����ɾ������ʱ����ɾ��ָ�룬��ɾ�����ݣ����ɾ�����ݵ�ʱ���޷��õ�����
// 2.�������ݺͱ�������ʧ��ʱ������ȷ���ؼ�
// 3.�����˲�ѯ���Եķ���
// 
// v1.1
// 2011.4.14
// 1.��������������˵���ķ���
// 
// v1.2
// 2011.4.15
// 1.�����˻�ȡ���ڵ�Ĺ���
// 
// v1.3
// 2011.11.3
// 1.������ TranTreePackToXML ���°汾��ע��ÿ�ε��øú���һ��Ҫ���� a_iLen �� a_iLevel Ϊ 0
// 2.������ ���з����� API ���ã�ʹ����������Ҳ�ܹ�ʹ��ͨ�����ݰ�
// 
// v1.4
// 2012.5.25
// 1.������ȡ����Ĭ��ֵ
// �磺byte GetByte(LPCTSTR a_sName,const byte a_iDefValue);
// 
// v1.5
// 2013.5.30
// ����������ݰ����ܴ��� 2G ������
// 
// v1.6
// 2014.1.23
// 1.������ SetImageOfRect ����������ֱ�Ӹ��ݴ�ͼ����ͼ��
// 
// v1.7
// 2014.3.11
// 1.�޸��˸ı�ڵ����Ƶ��µĴ洢���ļ��������
// 
// v1.8
// 2014.3.13
// 1.�޸��� SetImageOfRect �����ᵼ�� mtp �ļ���������
// 
// v1.9
// 2014.3.20
// 1.�޸��˽ڵ�� Lock �� Unlock ʱ�ڴ�й¶������
// 
// v1.91
// 2016.6.28
// 1.ȷ�����ݰ��ڴ洢��ʱ�����������������ʧ��ʱ���Զ�����
//
// v1.92
// 2016.7.16
// 1.�����˼��غͱ���ʱ�Ľ��ȹ���
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

    // ���ݰ�����
    enum SUN_DATA_PACK_TYPE{
        DP_BYTE=0,          // �ֽ�
        DP_WORD,            // ��
        DP_DWORD,           // ˫��
        DP_INT,             // ����
        DP_FLOAT,           // �����ȸ���
        DP_DOUBLE,          // ˫���ȸ���
        DP_INT64,           // ��������
        DP_LONGDOUBLE,      // ������(10�ֽ�)
        DP_STRING,          // �ַ���
        DP_TEXT,            // ���ı��ַ���(������ xml ʱ���Զ����� CData �ֶ�)
        DP_BINARY,          // ������
        DP_IMAGE,           // ͼ��
        DP_POINT            // ָ��
    };

    // ״̬����
    enum TREE_PACK_STATUS_TYPE{
        TPST_LOAD=0,        // ����
        TPST_SAVE           // ����
    };

    // ״̬�ص����� 2016.7.14
    typedef void (_stdcall* PTreePackStatusCallback)(void* a_pOwner,int a_iType,int a_iMin,int a_iMax,int a_iPos);

    // ͼ������
    enum SUN_IMAGE_TYPE{
        IT_LINE=0,          // ֱ�Ӵ洢��ʽ(���Դ洢�����ϵ��£�����4�ֽڶ���)
        IT_DIB              // Windows �ı�׼λͼ��ʽ���洢ʱÿ�а�4�ֽڶ��룬���ݴ��µ���
    };

    // ͼ�����ݽṹͷ
    struct CImageDataHeader
    {
        int             m_iWidth;       // ��
        int             m_iHeight;      // ��
        int             m_iBit;         // ͼ��λ��
        SUN_IMAGE_TYPE  m_itType;       // ͼ���ʽ
    };

    // ���ݽڵ�
    class CDataItem
    {
    public:
        int                 m_dpType;       // ��������
        _TCHAR              m_sName[260];   // ����
        _TCHAR              m_sDesc[260];   // ˵��
        byte*               m_pData;        // ����
        __int64             m_iSize;        // ���ݳ���
        byte*               m_pCustom;      // �Զ�������
        __int64             m_iCustomSize;  // �Զ������ݳ���
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
            iSize += ((_tcslen(m_sName) + 1)*sizeof(TCHAR));      // Key �� Name
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

        virtual void Lock()=NULL;           // ����
        virtual void Unlock()=NULL;         // ����

        virtual BOOL DeclareByte      (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // �����ֽڱ���
        virtual BOOL DeclareWord      (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // �����ֱ���
        virtual BOOL DeclareDword     (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // ����˫�ֱ���
        virtual BOOL DeclareInt       (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // ������������
        virtual BOOL DeclareFloat     (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // ���嵥���ȸ�����
        virtual BOOL DeclareDouble    (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // ����˫���ȸ�����
        virtual BOOL DeclareInt64     (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // ���� 64 λ����
        virtual BOOL DeclareLongDouble(LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // ���峤˫���ȸ�����
        virtual BOOL DeclareString    (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // �����ַ���
        virtual BOOL DeclareText      (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // �����ַ�������ת��Ϊ xml ʱ���Դ���� DATA ���򣬼����������ַ���
        virtual BOOL DeclareBinary    (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // �������������
        virtual BOOL DeclareImage     (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // ����ͼ��
        virtual BOOL DeclarePoint     (LPCTSTR a_sName,LPCTSTR a_sDesc=_T(""))=NULL;     // ����ָ��

        virtual BOOL SetByte      (LPCTSTR a_sName,byte       value)=NULL;               // �����ֽڱ���
        virtual BOOL SetWord      (LPCTSTR a_sName,WORD       value)=NULL;               // �����ֱ���
        virtual BOOL SetDword     (LPCTSTR a_sName,DWORD      value)=NULL;               // ����˫�ֱ���
        virtual BOOL SetInt       (LPCTSTR a_sName,int        value)=NULL;               // ������������
        virtual BOOL SetFloat     (LPCTSTR a_sName,float      value)=NULL;               // ���õ����ȸ�����
        virtual BOOL SetDouble    (LPCTSTR a_sName,double     value)=NULL;               // ����˫���ȸ�����
        virtual BOOL SetInt64     (LPCTSTR a_sName,__int64    value)=NULL;               // ���� 64 λ����
        virtual BOOL SetLongDouble(LPCTSTR a_sName,long double value)=NULL;              // ���ó�˫���ȸ�����
        virtual BOOL SetString    (LPCTSTR a_sName,LPCTSTR     value)=NULL;              // �����ַ���
        virtual BOOL SetText      (LPCTSTR a_sName,LPCTSTR     value)=NULL;              // �����ַ���
        virtual BOOL SetBinary    (LPCTSTR a_sName,byte*      value,__int64 a_iSize,byte* custom=NULL,__int64 a_iCustomSize=0)=NULL;                        // ���ö���������
        virtual BOOL SetImage     (LPCTSTR a_sName,int a_iWidth,int a_iHeight,int a_iBit,byte* a_pImage,SUN_IMAGE_TYPE a_itType=IT_LINE)=NULL;              // ����ͼ��
        // ����һ��ָ��λ�õ�ͼ�� 2014.1.23
        virtual BOOL SetImageOfRect(LPCTSTR a_sName,int a_iWidth,int a_iHeight,int a_iBit,byte* a_pImage,RECT a_rRect,SUN_IMAGE_TYPE a_itType=IT_LINE)=NULL;
        virtual BOOL SetPoint     (LPCTSTR a_sName,void*      value)=NULL;                                                                                  // ����ָ��

        virtual byte        GetByte      (LPCTSTR a_sName,const byte        a_iDefValue)=NULL;          // ��ȡ���ݣ���ͬ
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

        virtual BOOL Clear()=NULL;                                                  // �����������
        virtual BOOL Delete(LPCTSTR a_sName)=NULL;                                  // ɾ��һ����¼
        virtual INT GetCount()=NULL;                                                // ������м�¼��
        virtual CPackTreeBase* GetParent()=NULL;                                    // ��ø��ڵ�
        virtual CDataItem* Find(LPCTSTR a_sName)=NULL;                              // ���ָ�����Ƶ�����ָ��
        virtual CDataItem* Begin()=NULL;                                            // ������ݿ�ʼָ��
        virtual CDataItem* Next()=NULL;                                             // ������ݽ���ָ��
        virtual void SetName(LPCTSTR a_sName)=NULL;                                 // ��������
        virtual LPCTSTR GetName()=NULL;                                             // ��ȡ����
        virtual void SetParamDesc(LPCTSTR a_sName,LPCTSTR a_sDesc)=NULL;            // �������Ե�˵��
        virtual __int64 CalcDataSize()=NULL;                                        // ����ȫ�����ݵĳ���(���洢����Ŀռ�)
        virtual BOOL SaveToMemory(byte** a_pBuf,OUT __int64* a_pSize)=NULL;         // ����ȫ�����ݵ��ڴ�(�������ڴ��С)
        virtual BOOL LoadFromMemory(byte* a_pBuf,__int64 a_iSize)=NULL;             // ���ڴ��������
        virtual BOOL SaveToFile(LPCTSTR a_sFileName)=NULL;                          // ����ȫ�����ݵ��ļ�
        virtual BOOL LoadFromFile(LPCTSTR a_sFileName)=NULL;                        // ���ļ��м�������
        virtual BOOL SaveToFile(LPCTSTR a_sFileName,void* a_pOwner,PTreePackStatusCallback a_pCallback)=NULL;                       // ����ȫ�����ݵ��ļ�
        virtual BOOL LoadFromFile(LPCTSTR a_sFileName,void* a_pOwner,PTreePackStatusCallback a_pCallback)=NULL;                     // ���ļ��м�������

        // �ӽڵ�
        virtual void AddChild(CPackTreeBase* a_pChild)=NULL;                        // ���һ���ӽڵ�
        virtual BOOL ClearChild()=NULL;                                             // ����ӽڵ�
        virtual BOOL DeleteChild(LPCTSTR a_sName)=NULL;                             // ɾ��һ���ӽڵ�
        virtual INT GetChildCount()=NULL;                                           // ����ӽڵ���Ŀ
        virtual CPackTreeBase* ChildBegin()=NULL;                                   // ����ӽڵ㿪ʼָ��
        virtual CPackTreeBase* ChildNext()=NULL;                                    // ����ӽڵ����ָ��
        virtual CPackTreeBase* FindChild(LPCTSTR a_sName)=NULL;                     // ��һ���ӽڵ�
        virtual BOOL TranTreePackToXML(OUT _TCHAR** a_sOut,OUT int& a_iLevel,LPCTSTR a_sEncoding=_T("GB2312"))=NULL;  // ת��Ϊ xml ��ʽ����,�������ͷ�����Ŀռ�
        // ת��Ϊ xml ��ʽ����һ�ε��������� a_bGetSize Ϊ���ȡ������ڴ��С���ڶ��ε������� a_bGetSize���������� a_iLen�� a_iLevel Ϊ 0
        virtual BOOL TranTreePackToXML(OUT _TCHAR* a_sOut,OUT int& a_iLevel,bool a_bGetSize,int& a_iLen,LPCTSTR a_sEncoding)=NULL;
        // ��������
        virtual CPackTreeBase* CreateInstance()=NULL;                               // ����ʵ��
        virtual void Assign(CPackTreeBase* a_pSource)=NULL;                         // ������������

        virtual INT CalcDataSize32()=NULL;                                          // ����ȫ�����ݵĳ���(32λ���ݳ��ȼ��洢����Ŀռ�)
        virtual BOOL LoadFromMemory32(byte* a_pBuf,int a_iSize)=NULL;               // ���ڴ��������(32λ���ݳ���)
        virtual BOOL LoadFromFile32(LPCTSTR a_sFileName)=NULL;                      // ���ļ��м�������(32λ���ݳ���)
        virtual void Destroy()=NULL;                                                // ���� 2018.8.12
        virtual BOOL DeclareData(LPCTSTR a_sName,SUN_DATA_PACK_TYPE a_dpType,LPCTSTR a_sDesc)=NULL;                                                         // �������� 2018.11.16
        virtual BOOL SetData(LPCTSTR a_sName,SUN_DATA_PACK_TYPE a_dpType,byte* a_pData,__int64 a_iSize,byte* custom=NULL,__int64 a_iCustomSize=0)=NULL;     // ��������ֵ 2018.11.16
        virtual CDataItem* GetData(LPCTSTR a_sName,SUN_DATA_PACK_TYPE a_dpType)=NULL;                                                                       // ��ȡ�������� 2018.11.16
        virtual CPackTreeBase* DivideChild(LPCTSTR a_sName)=NULL;                                                                                           // ����һ���ӽڵ㣨��ɾ���������ǽ��벻�����ٽڵ����ݣ���ͬʱ���ظýڵ�ָ�� 2018.11.16
    protected:

    };

    // ����ͨ�����ݽṹ����
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



    // API �ӿ�
    typedef HANDLE (_stdcall* PExpCreateTreePack)(LPCTSTR);         // ��������
    typedef void (_stdcall* PExpDestroyTreePack)(HANDLE a_hHnd);    // �ͷ����ݰ�
    typedef void (_stdcall* PExpLock)(HANDLE a_hHnd);               // ����
    typedef void (_stdcall* PExpUnlock)(HANDLE a_hHnd);             // ����
    typedef BOOL (_stdcall* PExpDeclareByte)(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);            // �����ֽڱ���
    typedef BOOL (_stdcall* PExpDeclareWord      )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // �����ֱ���
    typedef BOOL (_stdcall* PExpDeclareDword     )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // ����˫�ֱ���
    typedef BOOL (_stdcall* PExpDeclareInt       )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // ������������
    typedef BOOL (_stdcall* PExpDeclareFloat     )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // ���嵥���ȸ�����
    typedef BOOL (_stdcall* PExpDeclareDouble    )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // ����˫���ȸ�����
    typedef BOOL (_stdcall* PExpDeclareInt64     )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // ���� 64 λ����
    typedef BOOL (_stdcall* PExpDeclareLongDouble)(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // ���峤˫���ȸ�����
    typedef BOOL (_stdcall* PExpDeclareString    )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // �����ַ���
    typedef BOOL (_stdcall* PExpDeclareText      )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // �����ַ�������ת��Ϊ xml ʱ���Դ���� DATA ���򣬼����������ַ���
    typedef BOOL (_stdcall* PExpDeclareBinary    )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // �������������
    typedef BOOL (_stdcall* PExpDeclareImage     )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // ����ͼ��
    typedef BOOL (_stdcall* PExpDeclarePoint     )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);      // ����ָ��

    typedef BOOL (_stdcall* PExpSetByte      )(HANDLE a_hHnd,LPCTSTR a_sName,byte       value);         // �����ֽڱ���
    typedef BOOL (_stdcall* PExpSetWord      )(HANDLE a_hHnd,LPCTSTR a_sName,WORD       value);         // �����ֱ���
    typedef BOOL (_stdcall* PExpSetDword     )(HANDLE a_hHnd,LPCTSTR a_sName,DWORD      value);         // ����˫�ֱ���
    typedef BOOL (_stdcall* PExpSetInt       )(HANDLE a_hHnd,LPCTSTR a_sName,int        value);         // ������������
    typedef BOOL (_stdcall* PExpSetFloat     )(HANDLE a_hHnd,LPCTSTR a_sName,float      value);         // ���õ����ȸ�����
    typedef BOOL (_stdcall* PExpSetDouble    )(HANDLE a_hHnd,LPCTSTR a_sName,double     value);         // ����˫���ȸ�����
    typedef BOOL (_stdcall* PExpSetInt64     )(HANDLE a_hHnd,LPCTSTR a_sName,__int64    value);         // ���� 64 λ����
    typedef BOOL (_stdcall* PExpSetLongDouble)(HANDLE a_hHnd,LPCTSTR a_sName,long double value);        // ���ó�˫���ȸ�����
    typedef BOOL (_stdcall* PExpSetString    )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR     value);        // �����ַ���
    typedef BOOL (_stdcall* PExpSetText      )(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR     value);        // �����ַ���
    typedef BOOL (_stdcall* PExpSetBinary    )(HANDLE a_hHnd,LPCTSTR a_sName,byte*      value,__int64 a_iSize,byte* custom,__int64 a_iCustomSize);                          // ����ͼ��
    typedef BOOL (_stdcall* PExpSetImage     )(HANDLE a_hHnd,LPCTSTR a_sName,int a_iWidth,int a_iHeight,int a_iBit,byte* a_pImage,SUN_IMAGE_TYPE a_itType);                 // ������������ͼ��
    typedef BOOL (_stdcall* PExpSetImageOfRect)(HANDLE a_hHnd,LPCTSTR a_sName,int a_iWidth,int a_iHeight,int a_iBit,byte* a_pImage,RECT a_rRect,SUN_IMAGE_TYPE a_itType);   // ����ָ��
    typedef BOOL (_stdcall* PExpSetPoint     )(HANDLE a_hHnd,LPCTSTR a_sName,void*      value);

    typedef byte        (_stdcall* PExpGetByte      )(HANDLE a_hHnd,LPCTSTR a_sName,const byte        a_iDefValue);     // ��ȡ���ݣ���ͬ
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

    typedef BOOL    (_stdcall* PExpClear)(HANDLE a_hHnd);                                                   // �����������
    typedef BOOL    (_stdcall* PExpDelete)(HANDLE a_hHnd,LPCTSTR a_sName);                                  // ɾ��һ����¼
    typedef INT     (_stdcall* PExpGetCount)(HANDLE a_hHnd);                                                // ������м�¼��
    typedef HANDLE  (_stdcall* PExpGetParent)(HANDLE a_hHnd);                                               // ��ø��ڵ�
    typedef CDataItem*  (_stdcall* PExpFind)(HANDLE a_hHnd,LPCTSTR a_sName);                                // ��һ������
    typedef CDataItem*  (_stdcall* PExpBegin)(HANDLE a_hHnd);                                               // ������Կ�ʼָ��
    typedef CDataItem*  (_stdcall* PExpNext)(HANDLE a_hHnd);                                                // ������Խ���ָ��
    typedef void    (_stdcall* PExpSetName)(HANDLE a_hHnd,LPCTSTR a_sName);                                 // ��������
    typedef LPCTSTR  (_stdcall* PExpGetName)(HANDLE a_hHnd);                                                // ��ȡ����
    typedef void    (_stdcall* PExpSetParamDesc)(HANDLE a_hHnd,LPCTSTR a_sName,LPCTSTR a_sDesc);            // �������Ե�˵��
    typedef __int64 (_stdcall* PExpCalcDataSize)(HANDLE a_hHnd);                                            // ����ȫ�����ݵĳ���(���洢����Ŀռ�)
    typedef BOOL    (_stdcall* PExpSaveToMemory)(HANDLE a_hHnd,OUT byte** a_pBuf,OUT __int64* a_iSize);     // ����ȫ�����ݵ��ڴ�(Ҫ���ȷ����ڴ�)
    typedef BOOL    (_stdcall* PExpLoadFromMemory)(HANDLE a_hHnd,byte* a_pBuf,int a_iSize);                 // ���ڴ��������
    typedef BOOL    (_stdcall* PExpSaveToFile)(HANDLE a_hHnd,LPCTSTR a_sFileName);                          // ����ȫ�����ݵ��ļ�
    typedef BOOL    (_stdcall* PExpLoadFromFile)(HANDLE a_hHnd,LPCTSTR a_sFileName);                        // ���ļ��м�������
    typedef BOOL    (_stdcall* PExpSaveToFileEx)(HANDLE a_hHnd,LPCTSTR a_sFileName,void* a_pOwner,PTreePackStatusCallback a_pCallback);                       // ����ȫ�����ݵ��ļ�
    typedef BOOL    (_stdcall* PExpLoadFromFileEx)(HANDLE a_hHnd,LPCTSTR a_sFileName,void* a_pOwner,PTreePackStatusCallback a_pCallback);                     // ���ļ��м�������
    // �ӽڵ�
    typedef void    (_stdcall* PExpAddChild)(HANDLE a_hHnd,HANDLE a_hChild);                                // ���һ���ӽڵ�
    typedef BOOL    (_stdcall* PExpClearChild)(HANDLE a_hHnd);                                              // ����ӽڵ�
    typedef BOOL    (_stdcall* PExpDeleteChild)(HANDLE a_hHnd,LPCTSTR a_sName);                             // ɾ��һ���ӽڵ�
    typedef INT     (_stdcall* PExpGetChildCount)(HANDLE a_hHnd);                                           // ����ӽڵ���Ŀ
    typedef HANDLE  (_stdcall* PExpChildBegin)(HANDLE a_hHnd);                                              // ����ӽڵ㿪ʼָ��
    typedef HANDLE  (_stdcall* PExpChildNext)(HANDLE a_hHnd);                                               // ����ӽڵ����ָ��
    typedef HANDLE  (_stdcall* PExpFindChild)(HANDLE a_hHnd,LPCTSTR a_sName);                               // ��һ���ӽڵ�
    typedef BOOL    (_stdcall* PExpTranTreePackToXML)(HANDLE a_hHnd,OUT _TCHAR** a_sOut,OUT int* a_iLevel,LPCTSTR a_sEncoding);     // ת��Ϊ xml ��ʽ����,�������ͷ�����Ŀռ�
    typedef BOOL    (_stdcall* PExpTranTreePackToXMLEx)(HANDLE a_hHnd,OUT _TCHAR* a_sOut,OUT int* a_iLevel,BOOL a_bGetSize,int* a_iLen,LPCTSTR a_sEncoding);
    // ��������
    typedef HANDLE  (_stdcall* PExpCreateInstance)(HANDLE a_hHnd);                                          // ����ʵ��
    typedef void    (_stdcall* PExpAssign)(HANDLE a_hHnd,HANDLE a_hFrom);                                   // ������������
    typedef void    (_stdcall* PExpDestroy)(HANDLE a_hHnd);                                                    // ���� 2018.8.12
    typedef BOOL    (_stdcall* PExpDeclareData)(HANDLE a_hHnd,LPCTSTR a_sName,SUN_DATA_PACK_TYPE a_dpType,LPCTSTR a_sDesc);                                                     // �������� 2018.11.16
    typedef BOOL    (_stdcall* PExpSetData)(HANDLE a_hHnd,LPCTSTR a_sName,SUN_DATA_PACK_TYPE a_dpType,byte* a_pData,__int64 a_iSize,byte* custom,__int64 a_iCustomSize);        // ��������ֵ 2018.11.16
    typedef CDataItem* (_stdcall* PExpGetData)(HANDLE a_hHnd,LPCTSTR a_sName,SUN_DATA_PACK_TYPE a_dpType);                                                                      // ��ȡ�������� 2018.11.16
    typedef CPackTreeBase* (_stdcall* PExpDivideChild)(HANDLE a_hHnd,LPCTSTR a_sName);                                                                                          // ����һ���ӽڵ㣨��ɾ���������ǽ��벻�����ٽڵ����ݣ���ͬʱ���ظýڵ�ָ�� 2018.11.16


}

#endif