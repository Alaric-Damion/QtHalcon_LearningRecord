
/*****************************************************************************
// 功能：常用函数库
// 作者：梅文海
// 日期：2003
// 版本：1.0
// 说明：该模块为自由版本，可任意使用，转载请附上本说明信息
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

// *********************************** 杂类 ***********************************
// 获得应用程序的全名称（含路径）
SUN_DLL_DEFINE CString GetAppExeName();
// 获得应用程序所在的目录
SUN_DLL_DEFINE CString GetAppExePath();
// 获得当前进程信息
SUN_DLL_DEFINE PROCESSENTRY32 SunGetCurrentProcessInfo();
// 判断一个文件是否存在
SUN_DLL_DEFINE BOOL FileExists(CString AFileName);
// 判断一个文件夹是否存在
SUN_DLL_DEFINE BOOL DirectoryExists(CString AFileName);
// 删除一个目录树
SUN_DLL_DEFINE bool DelDirTree(CString APath);
// 创建目录(可以自动创建不存在的目录)
SUN_DLL_DEFINE bool MakeSureDirectoryFileExist(CString APath);
// 按键盘键菜单组合键(即 Alt 键)
SUN_DLL_DEFINE void MenuKeyboardClick(byte AKey);
// 获得屏幕的分辨率
SUN_DLL_DEFINE long GetScreenWidth();
SUN_DLL_DEFINE long GetScreenHeight();
// 等待系统
SUN_DLL_DEFINE BOOL DoEvents();
// 运行一个可执行程序
SUN_DLL_DEFINE BOOL LaunchApplication(LPCTSTR pCmdLine,PROCESS_INFORMATION* pProcessInfo);
// 显示一个消息框
SUN_DLL_DEFINE UINT MyMessageBox(CString ACaption,int AType=0);
// 显示文字到 DC
SUN_DLL_DEFINE void ShowTextToDC(CDC& a_dc,CString a_sText,CRect a_rRect,COLORREF a_dwFontColor,
    int a_iFontSize=12,UINT nFormat=0,int nWeight=FW_NORMAL,CString a_sFontName=_T("宋体"),UINT a_iChartSet=GB2312_CHARSET);
// 根据名称删除子节点(支持模糊匹配)，请自行添加包含文件：#include <list>
SUN_DLL_DEFINE void DeleteTreePackNodeOfName(CPackTreeBase* a_pPack,LPCTSTR a_sName,bool a_bUseRule);




// ******************************* 坐标转换相关 *******************************
// 将两个点转换为一个矩形
SUN_DLL_DEFINE CRect GetPointToRect(CPoint APoint1,CPoint APoint2,int AExpand=0);
// 限制一个矩形的范围
SUN_DLL_DEFINE CRect SpeciRectLimit(CRect ARect,CRect ALimit);
// 限制一个点的范围
SUN_DLL_DEFINE CPoint SpeciPointLimit(CPoint APoint,CRect ALimit);
// 移动一个矩形区域
SUN_DLL_DEFINE CRect MoveRect(CRect ARect,CPoint APoint,BOOL AMove=FALSE);
// 移动一个点
SUN_DLL_DEFINE CPoint MovePoint(CPoint ASource,CPoint APoint);
// 返回一个矩形是否全部包含另一个矩形区域
SUN_DLL_DEFINE bool IfContainRect(CRect ASource,CRect ATarget);
// 功能     : 获得一个图片在一个容器中显示的形式
SUN_DLL_DEFINE CRect GetPictionRect(CPoint APanelRect,CPoint APicRect,bool AAutoZoomIn,bool AAutoZoomOut,int AWidthSpace=0,int AHeightSpace=0);


// ***************************** 数学函数处理相关 *****************************
// 获得一个随机数
SUN_DLL_DEFINE long GetRandom(long AMaxValue);
// 任意进制转换为整数(最多支持 36 进制)
SUN_DLL_DEFINE __int64 DigitToInt(CString mDigit,byte mScale);
// 四舍五入函数(正数向上取整，小数向下取整)
SUN_DLL_DEFINE double Round(double AValue,int count);
// 快速排序
template <class T>
SUN_DLL_DEFINE void QuickSort(T n[], int left, int right);


// 创建线程 2016.11.10
// 适用范围：适合短时间就能处理完成的线程，而不是一直存在的线程
// 使用说明1：
//   在需要处理的类从 CSunThread 派生，例如：class CDlgMain:public CDialog,public CSunThread
//   继承 SunExecute 函数，在 SunExecute 函数中添加处理代码
//   调用 SunCreateThread 创建一个新线程
// 使用说明2：
//   派生一个类：例如：class CMyThread:public CSunThread
//   声明该类的实例：CMyThread thread1;
//   开始线程：thread1.SunCreateThread(NULL);
//   重载 SunExecute 方法，在 SunExecute 中编写多线程处理代码
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
    UINT    m_dwSunThreadID;        // 线程 ID
    HANDLE  m_hSunThread;           // 线程句柄
    int     m_iSunThreadCount;      // 线程数
    CSemaphore  m_vntEvent;             // 同步用

    static UINT _stdcall SunThreadExecute(void* a_pParam);    // 开始线程
    void Lock(){m_vntEvent.Lock();}
    void Unlock(){m_vntEvent.Unlock();}
};

/*===================================================================
功能：对 Ini 文件进行操作
编写：梅文海

  历史记录：
  
    1.0     2006.08.01:
    1.增加 GetInt(); GetStr(); GetDouble() 等函数
    2005.02.02
    1.GetVarInt() 函数如果发现没有数据时返回 0
    1.01    2007.1.4
    1.修改了加载和保存中存在的隐患
    2.修改了成员变量的命名规则
    
      用户接口说明:在成员函数SetVarStr和SetVarInt函数中,当iType等于零,则如果用户制定的参数在ini文件中不存在,
      则就写入新的变量.当iType不等于零,则如果用户制定的参数在ini文件中不存在,就不写入新的变量，而是直接返回false;
===================================================================*/

// Ini 文件节点
class SUN_DLL_DEFINE CIniFileItem:public CIDItem
{
public:
    CIniFileItem(){};
    virtual ~CIniFileItem(){};
    virtual bool Assign(const CIDItemBase& AOther){                         // 拷贝函数(不能设为纯虚方法，否则 ID 不能被设值)
        CIniFileItem* pOther=(CIniFileItem*)&AOther;
        m_sName =pOther->GetName();
        m_sValue=pOther->GetValue();
        return true;
    }
    virtual CIDItemBase* CreateInstance(){return new CIniFileItem;};                // 创建该类的实例
    virtual LPCTSTR GetClassName(){GET_ITEM_CLASS_NAME(CIniFileItem);};  // 获得类名
    virtual LPCTSTR GetName(){return m_sName;};                                 // 获得名称
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
    //创建函数
    virtual bool Load(const CString strFileName);
    virtual bool Save();
    virtual bool Save(const CString strFileName);
    
    // 获得文本数据
    virtual CStringArray* GetData();
    virtual CString GetTextData();
    // 获得所有分组
    virtual void GetAllGroup(OUT CStringArray* a_aOutGroup);
    // 获得一个分组的节点数据(CIniFileItem*)
    virtual void GetGroupItem(IN LPCTSTR a_sGroupName,OUT CItemList* a_aOutItem);

    //得到变量整数型数值
    virtual bool GetVarInt(const CString,const CString ,int*);
    virtual bool GetVarDouble(const CString,const CString ,double*);
    virtual bool GetVarFloat(const CString,const CString ,float*);
    //得到变量字符串型数值
    virtual bool GetVarStr(const CString,const CString ,CString*);
    
    // 新增的获得值函数
    virtual CString GetStr(CString ASection,CString AItemName,CString ADefValue);
    virtual int GetInt(CString ASection,CString AItemName,int ADefValue);
    virtual double GetDouble(CString ASection,CString AItemName,double ADefValue);
    virtual float GetFloat(CString ASection,CString AItemName,float ADefValue);
    virtual bool GetBool(CString ASection,CString AItemName,bool ADefValue);
    
    
    // 设置文本数据
    virtual void SetData(const CStringArray* AData);
    // 设置文本数据
    virtual void SetTextData(const CString& AValue);
    //重新设置变量整数型数值
    virtual bool SetVarInt(const CString,const CString ,int,const int iType = 0);
    virtual bool SetVarDouble(const CString,const CString ,double,const int iType = 0);
    //重新设置变量字符串型数值
    virtual bool SetVarStr(const CString,const CString, CString,const int iType = 0);
    // 设置布尔变量数据
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
// 功能：单节点配置文件管理
// 作者：梅文海
// 日期：2006.9.16
// 用法：
// 1.先 Load() 文件
// 2.使用 GetValue() 函数即可直接读出数据
// 文件的格式如下所示：
// 101=
// 1001=开始
// 1002=结束
// ......
//
//////////////////////////////////////////////////////////////////////
class SUN_DLL_DEFINE CConfigFile
{
protected:
    CStringArray        m_aContainer;       // 文件的内容
public:
    CConfigFile();
    virtual ~CConfigFile();
    
    //创建函数
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
    // 获得一个指定节点的值
    CString GetValue(const CString AItem);
    CString GetValue(const CString AItem,const CString ASign);
    // 设置一个节点的值
    void SetValue(const CString AItem,const CString AValue);
    void SetValue(const CString AItem,const int AValue){
        CString s; s.Format(_T("%d"),AValue);
        SetValue(AItem,s);
    }
    // 获得全部的文本数据
    void GetAllText(CString& AValue);
    // 设置文本
    void SetText(LPCTSTR a_sText);
    // 获得指定索引数据
    bool GetValueOfIndex(const int a_iIndex,OUT CString& a_sName,OUT CString& a_sValue) const;
};


// 我们上面提到先定位第一个数，然后整理这个数组，把比这个数小的放到它的左边，
// 大的放右边，然后返回这中间值的位置，下面这函数就是做这个的。
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
该函数为凌云收集 2007

一般来说，冒泡法是程序员最先接触的排序方法，它的优点是原理简单，编程实现容易，
但它的缺点就是--程序的大忌--速度太慢。下面我介绍一个理解上简单但编程实现上不是太容易的排序方法，
我不知道它是不是现有排序方法中最快的，但它是我见过的最快的。
排序同样的数组，它所需的时间只有冒泡法的4%左右。我暂时称它为“快速排序法”。

  “快速排序法”使用的是递归原理，下面我结合一个例子来说明“快速排序法”的原理。
  首先给出一个数组{53，12，98，63，18，72，80，46，32，21}，先找到第一个数: 53，
  把它作为中间值，也就是说，要把53放在一个位置，使得它左边的值比它小，右边的值比它大。
  {21，12，32，46，18，53，80，72，63，98}，
  这样一个数组的排序就变成了两个小数组的排序: 53左边的数组和53右边的数组，
  而这两个数组继续用同样的方式继续下去，一直到顺序完全正确。
*/

// n就是需要排序的数组，left和right是待排序数组n[]的左界(起始号)和右界(结束号)，
// 如果要排序上面那个数组，那么left和right分别是0和9
template <class T>
void QuickSort(T n[], int left, int right)
{
    int dp;
    
    if ( left<right ){
        // 这就是下面要讲到的函数，按照上面所说的，
        // 就是把所有小于53的数放到它的左边，大的放在右边，
        // 然后返回53在整理过的数组中的位置。
        dp=partition<T>( n, left, right );
        
        //递归调用,分别整理53左边的数组和右边的数组
        QuickSort( n, left, dp-1 );
        QuickSort( n, dp+1, right );
    }
}

}// namespace



#endif
