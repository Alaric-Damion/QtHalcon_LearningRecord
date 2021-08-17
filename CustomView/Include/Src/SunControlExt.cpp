

#include "stdafx.h"
#include "SunControlExt.h"
#include "SunPublic.h"
#include "SunStr.h"

namespace sunLib{


// 获得文本框的行数
// 2006.4.12
int GetEditLineCount(HWND AHWND)
{
    int iResult=0;
    iResult = (int)SendMessage(AHWND, EM_GETLINECOUNT, 0, 0);
    if(SendMessage(AHWND, EM_LINELENGTH, SendMessage(AHWND,
        EM_LINEINDEX, iResult - 1, 0), 0) == 0) iResult--;
    return iResult;
}

// 在文本框插入字符
// 特别说明：默认的文本框是有字符数限制的，使用前先用 SetLimitText(-1) 方法设置为不受限制
// 2006.3.18
void InsertStringToEdit(HWND AHWND,int Index,const CString AText)
{
    int SelStart, LineLen;
    CString Line;

    if(Index >=0){
        SelStart=(int)SendMessage(AHWND, EM_LINEINDEX, Index, 0);
        if(SelStart >= 0) Line = AText + Crlf;
        else{
            SelStart = (int)SendMessage(AHWND, EM_LINEINDEX, Index - 1, 0);
            if(SelStart < 0) return;
            LineLen = (int)SendMessage(AHWND, EM_LINELENGTH, SelStart, 0);
            if(LineLen == 0) return;
            SelStart+=LineLen;
            Line = Crlf + AText;
        }
        SendMessage(AHWND, EM_SETSEL, SelStart, SelStart);
        SendMessage(AHWND, EM_REPLACESEL, 0, (LPARAM)(LPCTSTR)Line);
    }
}

// 获得列表控件的当前选择序号
// 2004.12.16
int GetListCtrlSelect(CListCtrl& AList)
{
    int iResult=-1;
    for(int i=0;i<AList.GetItemCount();i++)
    {
        if(AList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
        {
            iResult=i;
            break;
        }
    }
    return iResult;
}

// 选择一个组合框的指定内容
// 该组合框的组织方式为，数字＋空格＋字符串
// 2006.3.10
int SelComboBoxID(CComboBox& ABox,int AID)
{
    for(int i=0;i<ABox.GetCount();i++){
        CString s;
        ABox.GetLBText(i,s);
        int iID=_ttoi(GetStringSign(s,_T(" "),1));
        if(AID==iID){
            ABox.SetCurSel(i);
            return i;
        }
    }
    return -1;
}

int SelComboBoxID(CComboBox& ABox,CString AID)
{
    for(int i=0;i<ABox.GetCount();i++){
        CString s;
        ABox.GetLBText(i,s);
        CString sID=(GetStringSign(s,_T(" "),1));
        if(AID==sID){
            ABox.SetCurSel(i);
            return i;
        }
    }
    return -1;
}


//////////////////////////////////////////////////////////////////////////
// CSortListCtrl 类开始
//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSortListCtrl, CListCtrl)
ON_WM_DESTROY()
END_MESSAGE_MAP()

CSortListCtrl::CSortListCtrl()
{
    m_iSortColumn   =-1;
    m_bDesc         =false;
}

BOOL CSortListCtrl::DeleteItemEx(int nItem)
{
    tagCustomItemData* pData=(tagCustomItemData*)CListCtrl::GetItemData(nItem);
    if(pData) delete pData;
    return CListCtrl::DeleteItem(nItem);
}

BOOL CSortListCtrl::DeleteAllItemsEx()
{
    int iCount=GetItemCount();
    for(int i=0;i<iCount;i++){
        tagCustomItemData* pData=(tagCustomItemData*)CListCtrl::GetItemData(i);
        if(pData) delete pData;
    }
    return CListCtrl::DeleteAllItems();
}

CCustemItemDataBase* CSortListCtrl::GetItemDataEx(int nItem)
{
    if(nItem<0 || nItem>=GetItemCount()) return NULL;
    tagCustomItemData* pData=(tagCustomItemData*)CListCtrl::GetItemData(nItem);
    return pData->m_dwData;
}

BOOL CSortListCtrl::SetItemDataEx( int nItem, CCustemItemDataBase* AData)
{
    tagCustomItemData* pData=(tagCustomItemData*)CListCtrl::GetItemData(nItem);
    pData->m_dwData=AData;      // 这里就是传说当中的“狸猫换太子”，保证原有的 Data 属性仍然可以使用
    return true;
}

int CSortListCtrl::InsertItemEx(const LVITEM* pItem )
{
    int iIndex=CListCtrl::InsertItem(pItem);
    if(iIndex>=0){
        tagCustomItemData* pData=new tagCustomItemData;
        pData->m_iIndex=0;
        CListCtrl::SetItemData(iIndex,(DWORD_PTR)pData);
    }
    
    return iIndex;
}

int CSortListCtrl::InsertItemEx(int nItem, LPCTSTR lpszItem )
{
    int iIndex=CListCtrl::InsertItem(nItem,lpszItem);
    if(iIndex>=0){
        tagCustomItemData* pData=new tagCustomItemData;
        pData->m_iIndex=0;
        CListCtrl::SetItemData(iIndex,(DWORD_PTR)pData);
    }
    
    return iIndex;
}

int CSortListCtrl::InsertItemEx(int nItem, LPCTSTR lpszItem, int nImage )
{
    int iIndex=CListCtrl::InsertItem(nItem,lpszItem,nImage);
    if(iIndex>=0){
        tagCustomItemData* pData=new tagCustomItemData;
        pData->m_iIndex=0;
        CListCtrl::SetItemData(iIndex,(DWORD_PTR)pData);
    }
    
    return iIndex;
}

int CSortListCtrl::InsertItemEx(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam )
{
    int iIndex=CListCtrl::InsertItem(nMask,nItem,lpszItem,nState,nStateMask,nImage,lParam);
    if(iIndex>=0){
        tagCustomItemData* pData=new tagCustomItemData;
        pData->m_iIndex=0;
        CListCtrl::SetItemData(iIndex,(DWORD_PTR)pData);
    }
    
    return iIndex;
}

BOOL CSortListCtrl::SortItems(int AColumnIndex)
{
    if(m_iSortColumn!=AColumnIndex){
        m_iSortColumn=AColumnIndex;
        m_bDesc     =false;
    }
    else{
        m_bDesc=!m_bDesc;
    }
    // 先更新序号
    int iCount=GetItemCount();
    for(int i=0;i<iCount;i++){
        DWORD_PTR dwData=CListCtrl::GetItemData(i);
        tagCustomItemData* pData=(tagCustomItemData*)dwData;
        if(pData){
            pData->m_iIndex=i;
        }
    }
    return CListCtrl::SortItems(CompareFunc,(DWORD_PTR)this);
}

// 排序回调函数
// 2007.6.5
int CSortListCtrl::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    CSortListCtrl* pList=(CSortListCtrl*)lParamSort;
    if(!pList) return 0;
    tagCustomItemData* pData1=(pList->m_bDesc?(tagCustomItemData*)lParam2:(tagCustomItemData*)lParam1);
    tagCustomItemData* pData2=(pList->m_bDesc?(tagCustomItemData*)lParam1:(tagCustomItemData*)lParam2);
    CString    strItem1 = pList->GetItemText(pData1->m_iIndex, pList->m_iSortColumn);
    CString    strItem2 = pList->GetItemText(pData2->m_iIndex, pList->m_iSortColumn);
    
    return pList->CustomCompare(pList->m_iSortColumn,strItem1,strItem2);
}

int CSortListCtrl::CustomCompare(int AColumnIndex,CString AParam1,CString AParam2)
{
    return _tcscmp(AParam1,AParam2);
}

}//namespace