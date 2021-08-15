
/********************************************************************
  功能：控件扩展库
  作者：梅文海
  日期：2005
  版本：1.0
  说明：
*********************************************************************/

#ifndef _sun_control_ext_h
#define _sun_control_ext_h

#include "SunDefine.h"

namespace sunLib{

// 获得文本框的行数
SUN_DLL_DEFINE int GetEditLineCount(HWND AHWND);
// 在文本框插入字符(可以自动把滚动条拉到最下)
SUN_DLL_DEFINE void InsertStringToEdit(HWND AHWND,int Index,const CString AText);
// 获得列表控件的当前选择序号
SUN_DLL_DEFINE int GetListCtrlSelect(CListCtrl& AList);
// 选择一个组合框的指定内容
SUN_DLL_DEFINE int SelComboBoxID(CComboBox& ABox,int AID);
SUN_DLL_DEFINE int SelComboBoxID(CComboBox& ABox,CString AID);

/********************************************************************
  功能：自排序列表控件
  作者：梅文海
  日期：2007.6.5-2007.6.6
  版本：v1.0
  说明：如果采用字符串排序可以直接使用 CSortListCtrl ，如果采用自定义的排序方法
        需要派生一个新类，覆盖 CustomCompare() 虚拟方法即可

  使用说明：用向导生成 CSortListCtrl() 控件的 OnColumnclick 事件，调用 
  NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
  SortItems(pNMListView->iSubItem); 即可进行排序 pNMListView->iSubItem 表示用户
  点击的列序号

  CustomCompare() 方法的参数有三个，如 int AColumnIndex,CString AParam1,CString AParam2
  第一个参数是用户点击的列序号，第二个和第三个参数是需要进行排序的节点的字符串
  如果第一列需要进行数字排序，在派生类中改成如下代码即可：
  if(AColumnIndex==0) return atoi(AParam1)>atoi(AParam2);
  else                return CSortListCtrl::CustomCompare(AColumnIndex,AParam1,AParam2);
*********************************************************************/
class CCustemItemDataBase{
public:
    CCustemItemDataBase(){};
    virtual ~CCustemItemDataBase(){};
};

class SUN_DLL_DEFINE tagCustomItemData {
public:
    int     m_iIndex;       // 当前节点在列表中的索引
    CCustemItemDataBase*   m_dwData;       // 自定义数据

    tagCustomItemData(){
        m_iIndex=-1;
        m_dwData=NULL;
    }
    virtual ~tagCustomItemData(){
        if(m_dwData) {
            delete m_dwData;
            m_dwData=NULL;
        }    
    }
};

class SUN_DLL_DEFINE CSortListCtrl : public CListCtrl
{
protected:
    int     m_iSortColumn;      // 排序的列
    bool    m_bDesc;            // 是否采用降序

    static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
    BOOL SortItems(PFNLVCOMPARE pfnCompare,DWORD_PTR dwData);       // 屏蔽父类的 SortItems 函数

    // 屏蔽这些函数
    virtual void* GetItemData(int nItem){return (void*)CListCtrl::GetItemData(nItem);};
    virtual BOOL SetItemData( int nItem, void* AData){return CListCtrl::SetItemData(nItem,(DWORD_PTR)AData);};
    virtual int InsertItem(const LVITEM* pItem ){return CListCtrl::InsertItem(pItem);};
    virtual int InsertItem(int nItem, LPCTSTR lpszItem ){return CListCtrl::InsertItem(nItem,lpszItem);};
    virtual int InsertItem(int nItem, LPCTSTR lpszItem, int nImage ){return CListCtrl::InsertItem(nItem,lpszItem,nImage);};
    virtual int InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam )
        {return CListCtrl::InsertItem(nMask,nItem,lpszItem,nState,nStateMask,nImage,lParam);};
    virtual BOOL DeleteItem( int nItem ){return CListCtrl::DeleteItem(nItem);};
    virtual BOOL DeleteAllItems(){return CListCtrl::DeleteAllItems();};
public:
    CSortListCtrl();

    virtual int CustomCompare(int AColumnIndex,CString AParam1,CString AParam2);
    virtual CCustemItemDataBase* GetItemDataEx(int nItem);
    virtual BOOL SetItemDataEx( int nItem, CCustemItemDataBase* AData);
    virtual BOOL SortItems(int AColumnIndex);

    virtual int InsertItemEx(const LVITEM* pItem );
    virtual int InsertItemEx(int nItem, LPCTSTR lpszItem );
    virtual int InsertItemEx(int nItem, LPCTSTR lpszItem, int nImage );
    virtual int InsertItemEx(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam );

    virtual BOOL DeleteItemEx( int nItem );
    virtual BOOL DeleteAllItemsEx();

    afx_msg void OnDestroy( ){
        DeleteAllItemsEx();
    };
    DECLARE_MESSAGE_MAP()
};


}
#endif