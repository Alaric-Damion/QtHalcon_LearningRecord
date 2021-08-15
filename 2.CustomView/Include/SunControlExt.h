
/********************************************************************
  ���ܣ��ؼ���չ��
  ���ߣ�÷�ĺ�
  ���ڣ�2005
  �汾��1.0
  ˵����
*********************************************************************/

#ifndef _sun_control_ext_h
#define _sun_control_ext_h

#include "SunDefine.h"

namespace sunLib{

// ����ı��������
SUN_DLL_DEFINE int GetEditLineCount(HWND AHWND);
// ���ı�������ַ�(�����Զ��ѹ�������������)
SUN_DLL_DEFINE void InsertStringToEdit(HWND AHWND,int Index,const CString AText);
// ����б�ؼ��ĵ�ǰѡ�����
SUN_DLL_DEFINE int GetListCtrlSelect(CListCtrl& AList);
// ѡ��һ����Ͽ��ָ������
SUN_DLL_DEFINE int SelComboBoxID(CComboBox& ABox,int AID);
SUN_DLL_DEFINE int SelComboBoxID(CComboBox& ABox,CString AID);

/********************************************************************
  ���ܣ��������б�ؼ�
  ���ߣ�÷�ĺ�
  ���ڣ�2007.6.5-2007.6.6
  �汾��v1.0
  ˵������������ַ����������ֱ��ʹ�� CSortListCtrl ����������Զ�������򷽷�
        ��Ҫ����һ�����࣬���� CustomCompare() ���ⷽ������

  ʹ��˵������������ CSortListCtrl() �ؼ��� OnColumnclick �¼������� 
  NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
  SortItems(pNMListView->iSubItem); ���ɽ������� pNMListView->iSubItem ��ʾ�û�
  ����������

  CustomCompare() �����Ĳ������������� int AColumnIndex,CString AParam1,CString AParam2
  ��һ���������û����������ţ��ڶ����͵�������������Ҫ��������Ľڵ���ַ���
  �����һ����Ҫ���������������������иĳ����´��뼴�ɣ�
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
    int     m_iIndex;       // ��ǰ�ڵ����б��е�����
    CCustemItemDataBase*   m_dwData;       // �Զ�������

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
    int     m_iSortColumn;      // �������
    bool    m_bDesc;            // �Ƿ���ý���

    static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
    BOOL SortItems(PFNLVCOMPARE pfnCompare,DWORD_PTR dwData);       // ���θ���� SortItems ����

    // ������Щ����
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