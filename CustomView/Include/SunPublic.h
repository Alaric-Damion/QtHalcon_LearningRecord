
/*****************************************************************************
// ���ܣ����ú�����
// ���ߣ�÷�ĺ�
// ���ڣ�2003
// �汾��1.0
// ˵������ģ��Ϊ���ɰ汾��������ʹ�ã�ת���븽�ϱ�˵����Ϣ
*****************************************************************************/

#ifndef _INC_PUBLIC_H_32564987132178947
#define _INC_PUBLIC_H_32564987132178947

#include <Tlhelp32.h>
#include <afxmt.h>
#include "SunDefine.h"
#include "SunItemList.h"
#include <list>

using namespace std;


namespace sunLib{

class CPackTreeBase;    

// *********************************** ���� ***********************************
// ���Ӧ�ó����ȫ���ƣ���·����
SUN_DLL_DEFINE CString GetAppExeName();
// ���Ӧ�ó������ڵ�Ŀ¼
SUN_DLL_DEFINE CString GetAppExePath();
// ��õ�ǰ������Ϣ
SUN_DLL_DEFINE PROCESSENTRY32 SunGetCurrentProcessInfo();
// �ж�һ���ļ��Ƿ����
SUN_DLL_DEFINE BOOL FileExists(CString AFileName);
// �ж�һ���ļ����Ƿ����
SUN_DLL_DEFINE BOOL DirectoryExists(CString AFileName);
// ɾ��һ��Ŀ¼��
SUN_DLL_DEFINE bool DelDirTree(CString APath);
// ����Ŀ¼(�����Զ����������ڵ�Ŀ¼)
SUN_DLL_DEFINE bool MakeSureDirectoryFileExist(CString APath);
// �����̼��˵���ϼ�(�� Alt ��)
SUN_DLL_DEFINE void MenuKeyboardClick(byte AKey);
// �����Ļ�ķֱ���
SUN_DLL_DEFINE long GetScreenWidth();
SUN_DLL_DEFINE long GetScreenHeight();
// �ȴ�ϵͳ
SUN_DLL_DEFINE BOOL DoEvents();
// ����һ����ִ�г���
SUN_DLL_DEFINE BOOL LaunchApplication(LPCTSTR pCmdLine,PROCESS_INFORMATION* pProcessInfo);
// ��ʾһ����Ϣ��
SUN_DLL_DEFINE UINT MyMessageBox(CString ACaption,int AType=0);
// ��ʾ���ֵ� DC
SUN_DLL_DEFINE void ShowTextToDC(CDC& a_dc,CString a_sText,CRect a_rRect,COLORREF a_dwFontColor,
    int a_iFontSize=12,UINT nFormat=0,int nWeight=FW_NORMAL,CString a_sFontName=_T("����"),UINT a_iChartSet=GB2312_CHARSET);
// ��������ɾ���ӽڵ�(֧��ģ��ƥ��)����������Ӱ����ļ���#include <list>
SUN_DLL_DEFINE void DeleteTreePackNodeOfName(CPackTreeBase* a_pPack,LPCTSTR a_sName,bool a_bUseRule);




// ******************************* ����ת����� *******************************
// ��������ת��Ϊһ������
SUN_DLL_DEFINE CRect GetPointToRect(CPoint APoint1,CPoint APoint2,int AExpand=0);
// ����һ�����εķ�Χ
SUN_DLL_DEFINE CRect SpeciRectLimit(CRect ARect,CRect ALimit);
// ����һ����ķ�Χ
SUN_DLL_DEFINE CPoint SpeciPointLimit(CPoint APoint,CRect ALimit);
// �ƶ�һ����������
SUN_DLL_DEFINE CRect MoveRect(CRect ARect,CPoint APoint,BOOL AMove=FALSE);
// �ƶ�һ����
SUN_DLL_DEFINE CPoint MovePoint(CPoint ASource,CPoint APoint);
// ����һ�������Ƿ�ȫ��������һ����������
SUN_DLL_DEFINE bool IfContainRect(CRect ASource,CRect ATarget);
// ����     : ���һ��ͼƬ��һ����������ʾ����ʽ
SUN_DLL_DEFINE CRect GetPictionRect(CPoint APanelRect,CPoint APicRect,bool AAutoZoomIn,bool AAutoZoomOut,int AWidthSpace=0,int AHeightSpace=0);


// ***************************** ��ѧ����������� *****************************
// ���һ�������
SUN_DLL_DEFINE long GetRandom(long AMaxValue);
// �������ת��Ϊ����(���֧�� 36 ����)
SUN_DLL_DEFINE __int64 DigitToInt(CString mDigit,byte mScale);
// �������뺯��(��������ȡ����С������ȡ��)
SUN_DLL_DEFINE double Round(double AValue,int count);
// ��������
template <class T>
SUN_DLL_DEFINE void QuickSort(T n[], int left, int right);


// �����߳� 2016.11.10
// ���÷�Χ���ʺ϶�ʱ����ܴ�����ɵ��̣߳�������һֱ���ڵ��߳�
// ʹ��˵��1��
//   ����Ҫ�������� CSunThread ���������磺class CDlgMain:public CDialog,public CSunThread
//   �̳� SunExecute �������� SunExecute ��������Ӵ������
//   ���� SunCreateThread ����һ�����߳�
// ʹ��˵��2��
//   ����һ���ࣺ���磺class CMyThread:public CSunThread
//   ���������ʵ����CMyThread thread1;
//   ��ʼ�̣߳�thread1.SunCreateThread(NULL);
//   ���� SunExecute �������� SunExecute �б�д���̴߳������
class CSunThread;
struct tagThreadParamInfo{
    void*           m_pOwner;
    CSunThread*     m_pThis;
};

class SUN_DLL_DEFINE CSunThread{
public:
    CSunThread();
    virtual ~CSunThread(){};
    virtual HANDLE SunCreateThread(void* a_pOwner=NULL);
    virtual bool SunStop(){return true;}
    virtual void SunEnd();
    virtual bool SunExecute(void* a_pOwner){return true;};
    virtual int SunGetThreadCount(){return m_iSunThreadCount;}
protected:
    UINT    m_dwSunThreadID;        // �߳� ID
    HANDLE  m_hSunThread;           // �߳̾��
    int     m_iSunThreadCount;      // �߳���
    CSemaphore  m_vntEvent;             // ͬ����

    static UINT _stdcall SunThreadExecute(void* a_pParam);    // ��ʼ�߳�
    void Lock(){m_vntEvent.Lock();}
    void Unlock(){m_vntEvent.Unlock();}
};

/*===================================================================
���ܣ��� Ini �ļ����в���
��д��÷�ĺ�

  ��ʷ��¼��
  
    1.0     2006.08.01:
    1.���� GetInt(); GetStr(); GetDouble() �Ⱥ���
    2005.02.02
    1.GetVarInt() �����������û������ʱ���� 0
    1.01    2007.1.4
    1.�޸��˼��غͱ����д��ڵ�����
    2.�޸��˳�Ա��������������
    
      �û��ӿ�˵��:�ڳ�Ա����SetVarStr��SetVarInt������,��iType������,������û��ƶ��Ĳ�����ini�ļ��в�����,
      ���д���µı���.��iType��������,������û��ƶ��Ĳ�����ini�ļ��в�����,�Ͳ�д���µı���������ֱ�ӷ���false;
===================================================================*/

// Ini �ļ��ڵ�
class SUN_DLL_DEFINE CIniFileItem:public CIDItem
{
public:
    CIniFileItem(){};
    virtual ~CIniFileItem(){};
    virtual bool Assign(const CIDItemBase& AOther){                         // ��������(������Ϊ���鷽�������� ID ���ܱ���ֵ)
        CIniFileItem* pOther=(CIniFileItem*)&AOther;
        m_sName =pOther->GetName();
        m_sValue=pOther->GetValue();
        return true;
    }
    virtual CIDItemBase* CreateInstance(){return new CIniFileItem;};                // ���������ʵ��
    virtual LPCTSTR GetClassName(){GET_ITEM_CLASS_NAME(CIniFileItem);};  // �������
    virtual LPCTSTR GetName(){return m_sName;};                                 // �������
    virtual CString GetValue(){return m_sValue;};
    virtual void SetName(LPCTSTR a_sName){m_sName=a_sName;};
    virtual void SetValue(LPCTSTR a_sValue){m_sValue=a_sValue;};
protected:
    CString     m_sName;
    CString     m_sValue;
};

class SUN_DLL_DEFINE CIniFile
{
public:
    CIniFile();
    virtual ~CIniFile();
private:
    CIniFile(const CIniFile &);
    CIniFile & operator = (const CIniFile &);
public:
    //��������
    virtual bool Load(const CString strFileName);
    virtual bool Save();
    virtual bool Save(const CString strFileName);
    
    // ����ı�����
    virtual CStringArray* GetData();
    virtual CString GetTextData();
    // ������з���
    virtual void GetAllGroup(OUT CStringArray* a_aOutGroup);
    // ���һ������Ľڵ�����(CIniFileItem*)
    virtual void GetGroupItem(IN LPCTSTR a_sGroupName,OUT CItemList* a_aOutItem);

    //�õ�������������ֵ
    virtual bool GetVarInt(const CString,const CString ,int*);
    virtual bool GetVarDouble(const CString,const CString ,double*);
    virtual bool GetVarFloat(const CString,const CString ,float*);
    //�õ������ַ�������ֵ
    virtual bool GetVarStr(const CString,const CString ,CString*);
    
    // �����Ļ��ֵ����
    virtual CString GetStr(CString ASection,CString AItemName,CString ADefValue);
    virtual int GetInt(CString ASection,CString AItemName,int ADefValue);
    virtual double GetDouble(CString ASection,CString AItemName,double ADefValue);
    virtual float GetFloat(CString ASection,CString AItemName,float ADefValue);
    virtual bool GetBool(CString ASection,CString AItemName,bool ADefValue);
    
    
    // �����ı�����
    virtual void SetData(const CStringArray* AData);
    // �����ı�����
    virtual void SetTextData(const CString& AValue);
    //�������ñ�����������ֵ
    virtual bool SetVarInt(const CString,const CString ,int,const int iType = 0);
    virtual bool SetVarDouble(const CString,const CString ,double,const int iType = 0);
    //�������ñ����ַ�������ֵ
    virtual bool SetVarStr(const CString,const CString, CString,const int iType = 0);
    // ���ò�����������
    virtual bool SetBool(const CString,const CString,const bool,const int iType =0);

    
protected:
    virtual bool GetVar(const CString,const CString,CString*);
    virtual bool SetVar(const CString,const CString,CString,const int iType = 0);
    virtual int  SearchLine(const CString,const CString &);
    
protected:
    CStringArray    m_aFileContainer;
    CString         m_sFileName;
};

//////////////////////////////////////////////////////////////////////
// 
// ���ܣ����ڵ������ļ�����
// ���ߣ�÷�ĺ�
// ���ڣ�2006.9.16
// �÷���
// 1.�� Load() �ļ�
// 2.ʹ�� GetValue() ��������ֱ�Ӷ�������
// �ļ��ĸ�ʽ������ʾ��
// 101=
// 1001=��ʼ
// 1002=����
// ......
//
//////////////////////////////////////////////////////////////////////
class SUN_DLL_DEFINE CConfigFile
{
protected:
    CStringArray        m_aContainer;       // �ļ�������
public:
    CConfigFile();
    virtual ~CConfigFile();
    
    //��������
    bool Load(const CString AFileName);
    bool Save(const CString AFileName);
    void Clear(){m_aContainer.RemoveAll();}
    bool Assign(CConfigFile& a_cfFile){
        Clear();
        for(int i=0;i<a_cfFile.GetCount();i++){
            m_aContainer.Add(a_cfFile.GetValue(i));
        }
        return true;
    }
    
    int GetCount(){return (int)m_aContainer.GetSize();};
    CString GetValue(int a_iIndex){return m_aContainer.GetAt(a_iIndex);};
    // ���һ��ָ���ڵ��ֵ
    CString GetValue(const CString AItem);
    CString GetValue(const CString AItem,const CString ASign);
    // ����һ���ڵ��ֵ
    void SetValue(const CString AItem,const CString AValue);
    void SetValue(const CString AItem,const int AValue){
        CString s; s.Format(_T("%d"),AValue);
        SetValue(AItem,s);
    }
    // ���ȫ�����ı�����
    void GetAllText(CString& AValue);
    // �����ı�
    void SetText(LPCTSTR a_sText);
    // ���ָ����������
    bool GetValueOfIndex(const int a_iIndex,OUT CString& a_sName,OUT CString& a_sValue) const;
};


// ���������ᵽ�ȶ�λ��һ������Ȼ������������飬�ѱ������С�ķŵ�������ߣ�
// ��ķ��ұߣ�Ȼ�󷵻����м�ֵ��λ�ã������⺯������������ġ�
template <class T>
int partition(T n[], int left, int right )
{
    T pivot, t;
    int lo,hi;
    
    pivot = n[left];
    lo = left-1;
    hi = right+1;
    while( lo+1!=hi ){
        if( n[lo+1]<=pivot )
            lo++;
        else{
            if( n[hi-1]>pivot )
                hi--;
            else{
                t = n[lo+1];
                n[++lo] = n[hi-1];
                n[--hi] = t;
            }
        }
    }
    n[left] = n[lo];
    n[lo] = pivot;
    
    return lo;
    
}
/*
�ú���Ϊ�����ռ� 2007

һ����˵��ð�ݷ��ǳ���Ա���ȽӴ������򷽷��������ŵ���ԭ��򵥣����ʵ�����ף�
������ȱ�����--����Ĵ��--�ٶ�̫���������ҽ���һ������ϼ򵥵����ʵ���ϲ���̫���׵����򷽷���
�Ҳ�֪�����ǲ����������򷽷������ģ��������Ҽ��������ġ�
����ͬ�������飬�������ʱ��ֻ��ð�ݷ���4%���ҡ�����ʱ����Ϊ���������򷨡���

  ���������򷨡�ʹ�õ��ǵݹ�ԭ�������ҽ��һ��������˵�����������򷨡���ԭ��
  ���ȸ���һ������{53��12��98��63��18��72��80��46��32��21}�����ҵ���һ����: 53��
  ������Ϊ�м�ֵ��Ҳ����˵��Ҫ��53����һ��λ�ã�ʹ������ߵ�ֵ����С���ұߵ�ֵ������
  {21��12��32��46��18��53��80��72��63��98}��
  ����һ�����������ͱ��������С���������: 53��ߵ������53�ұߵ����飬
  �����������������ͬ���ķ�ʽ������ȥ��һֱ��˳����ȫ��ȷ��
*/

// n������Ҫ��������飬left��right�Ǵ���������n[]�����(��ʼ��)���ҽ�(������)��
// ���Ҫ���������Ǹ����飬��ôleft��right�ֱ���0��9
template <class T>
void QuickSort(T n[], int left, int right)
{
    int dp;
    
    if ( left<right ){
        // ���������Ҫ�����ĺ���������������˵�ģ�
        // ���ǰ�����С��53�����ŵ�������ߣ���ķ����ұߣ�
        // Ȼ�󷵻�53��������������е�λ�á�
        dp=partition<T>( n, left, right );
        
        //�ݹ����,�ֱ�����53��ߵ�������ұߵ�����
        QuickSort( n, left, dp-1 );
        QuickSort( n, dp+1, right );
    }
}

}// namespace



#endif
