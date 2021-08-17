// ItemList.cpp: implementation of the CItemList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunItemList.h"

#ifdef _DEBUG
#undef THIS_FILE
static _TCHAR THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace sunLib{


CIDItem::CIDItem()
{
    m_iID   =-1;
}

// 拷贝函数
// 2006.4.20
bool CIDItem::Assign(const CIDItemBase& AOther)
{
    m_iID   =((CIDItem*)&AOther)->m_iID;
    return true;
}

CItemList::CItemList()
{

}

CItemList::~CItemList()
{
    Clear();
}

// 拷贝函数
// 2006.4.20
bool CItemList::Assign(CItemList& AOther)
{
    Clear();
    ReSetSize(NULL,0,AOther.GetItemCount());
    for(int i=0;i<AOther.GetItemCount();i++){
        CIDItemBase* pItem=(CIDItemBase*)AOther.GetValue(i)->CreateInstance();
        pItem->Assign(*AOther.GetValue(i));
        AddItem(pItem);
    }
    return true;
}

// 获得最大的编号
// 2006.2.7
int CItemList::GetMaxID()
{
    int i,iMax,iCount;
    CIDItemBase* pItem;
    iMax=0;
    iCount=(int)GetItemCount();
    for(i=0;i<iCount;i++){
        pItem=GetValue(i);
        if(pItem->GetID()>iMax){
            iMax=pItem->GetID();
        }
    }
    return iMax;
}
       
// 增加一个节点
void CItemList::AddItem(CIDItemBase* AItem){
    if(AItem->GetID()<0){
        AItem->SetID(GetMaxID()+1);
    }
    else{
        // 判断 ID 是否重复
        if(GetIndexOfID(AItem->GetID())>=0){
            AItem->SetID(GetMaxID()+1);
        }
    }
    m_aItem.Add(AItem);
}

// 插入一个节点 2010.3.20
void CItemList::InsertItem(const int a_iIndex,CIDItemBase* AItem)
{
    if(AItem->GetID()<0){
        AItem->SetID(GetMaxID()+1);
    }
    else{
        // 判断 ID 是否重复
        if(GetIndexOfID(AItem->GetID())>=0){
            AItem->SetID(GetMaxID()+1);
        }
    }
    m_aItem.InsertAt(a_iIndex,AItem);
}

// 删除一个节点
bool CItemList::DelItem(int AIndex){
    if(AIndex<GetItemCount() && AIndex>=0){
        CIDItemBase* pItem=m_aItem.GetAt(AIndex);
        if(pItem) delete pItem;
        m_aItem.RemoveAt(AIndex);
        return true;
    }
    else{
        return false;
    }
}

bool CItemList::DelItemOfID(int AID){
    int iIndex=GetIndexOfID(AID);
    if(iIndex<0){
        return false;
    }
    else{
        return DelItem(iIndex);
    }
}

// 清空列表
void CItemList::Clear(){
    int iCount=(int)GetItemCount();
    for(int i=iCount-1;i>=0;i--){
        DelItem(i);
    }
    m_aItem.RemoveAll();
}

// 获得所有节点
int CItemList::GetItemCount(){
    return (int)m_aItem.GetSize();
}

// 获得一个节点
CIDItemBase* CItemList::GetValue(int AIndex){
    if(AIndex<0) return NULL;
    if(AIndex>=GetItemCount()) return NULL;
    else                       return m_aItem.GetAt(AIndex);
}

// 获得一个索引号
int CItemList::GetIndexOfID(int AID){
    int iResult=-1;
    for(int i=0;i<GetItemCount();i++){
        CIDItemBase* pItem=GetValue(i);
        if(pItem){
            if(pItem->GetID()==AID){
                iResult=i;
                break;
            }
        }
    }
    return iResult;
}

// 根据一个 ID 获得一个节点
CIDItemBase* CItemList::GetValueOfID(int AID){
    int iIndex=GetIndexOfID(AID);
    if(iIndex>=0) return m_aItem.GetAt(iIndex);
    else          return NULL;
}

// 根据名称获得一个节点
// 2006.5.25
CIDItemBase* CItemList::GetValueOfName(CString AName)
{
    int iCount=(int)GetItemCount();
    for(int i=0;i<iCount;i++){
        CIDItemBase* pItem=GetValue(i);
        if(pItem){
            if(pItem->GetName()==AName){
                return pItem;
            }
        }
    }
    return NULL;
}

// 同步数据
// 把 ATargetList 的数据同步到本身
//    如果本身中有 ATargetList 中没有的数据，则删除本身中的该项
//    如果 ATargetList 中有而本身没有则在本身中添加该项
// 2006.7.26
void CItemList::SyncItemList(CItemList* ATargetList)
{
    // 删除 ASourceList 中没有的数据项
    int iCount=(int)GetItemCount();
    int i;
    for(i=iCount-1;i>=0;i--){
        CIDItemBase* pItem=GetValue(i);
        if(!ATargetList->GetValueOfName(pItem->GetName())){
            DelItem(i);
        }
    }
    // 添加 ATargetList 中没有的数据项
    for(i=0;i<ATargetList->GetItemCount();i++){
        CIDItemBase* pItem=ATargetList->GetValue(i);
        if(!GetValueOfName(pItem->GetName())){
            CIDItemBase* pNewItem=pItem->CreateInstance();
            pNewItem->Assign(*pItem);
            pItem->SetID(GetMaxID()+1);
            AddItem(pNewItem);
        }
    }
}

// 重新设置容器大小 2018.7.4
void CItemList::ReSetSize(CIDItemBase* a_pItem,const int a_iSize,const int a_iGrowBy){
    Clear();
    m_aItem.SetSize(a_iSize,a_iGrowBy);   
    for(int i=0;i<a_iSize;i++){
        CIDItemBase* pItem=a_pItem->CreateInstance();
        pItem->SetID(i+1);
        m_aItem.SetAt(i,pItem);
    }
}




//////////////////////////////////////////////////////////////////////////
// CItemListVector

CItemListVector::CItemListVector(void)
{

}

CItemListVector::~CItemListVector()
{
    Clear();
}

// 拷贝函数
// 2006.4.20
bool CItemListVector::Assign(CItemListVector& AOther)
{
    Clear();
    for(int i=0;i<AOther.GetItemCount();i++){
        CIDItemBase* pItem=(CIDItemBase*)AOther.GetValue(i)->CreateInstance();
        pItem->Assign(*AOther.GetValue(i));
        AddItem(pItem);
    }
    return true;
}

// 获得最大的编号
// 2006.2.7
int CItemListVector::GetMaxID()
{
    int i,iMax,iCount;
    CIDItemBase* pItem;
    iMax=0;
    iCount=(int)GetItemCount();
    for(i=0;i<iCount;i++){
        pItem=GetValue(i);
        if(pItem->GetID()>iMax){
            iMax=pItem->GetID();
        }
    }
    return iMax;
}

// 增加一个节点
void CItemListVector::AddItem(CIDItemBase* AItem){
    if(AItem->GetID()<0){
        AItem->SetID(GetMaxID()+1);
    }
    else{
        // 判断 ID 是否重复
        if(GetIndexOfID(AItem->GetID())>=0){
            AItem->SetID(GetMaxID()+1);
        }
    }
    m_aItem.push_back(AItem);
}

// 插入一个节点 2010.3.20
void CItemListVector::InsertItem(const int a_iIndex,CIDItemBase* AItem)
{
    if(AItem->GetID()<0){
        AItem->SetID(GetMaxID()+1);
    }
    else{
        // 判断 ID 是否重复
        if(GetIndexOfID(AItem->GetID())>=0){
            AItem->SetID(GetMaxID()+1);
        }
    }
    m_aItem.insert(m_aItem.begin()+a_iIndex,AItem);
}

// 删除一个节点
bool CItemListVector::DelItem(int AIndex){
    if(AIndex<GetItemCount() && AIndex>=0){
        delete m_aItem[AIndex];
        m_aItem.erase(m_aItem.begin()+AIndex);
        return true;
    }
    else{
        return false;
    }
}

bool CItemListVector::DelItemOfID(int AID){
    int iIndex=GetIndexOfID(AID);
    if(iIndex<0){
        return false;
    }
    else{
        return DelItem(iIndex);
    }
}

// 清空列表
void CItemListVector::Clear(){
    int iCount=(int)GetItemCount();
    for(int i=iCount-1;i>=0;i--){
        DelItem(i);
    }
    m_aItem.clear();
}

// 获得所有节点
int CItemListVector::GetItemCount(){
    return (int)m_aItem.size();
}

// 获得一个节点
CIDItemBase* CItemListVector::GetValue(int AIndex){
    if(AIndex<0) return NULL;
    if(AIndex>=GetItemCount()) return NULL;
    else                       return m_aItem[AIndex];
}

// 获得一个索引号
int CItemListVector::GetIndexOfID(int AID){
    int iResult=-1;
    for(int i=0;i<GetItemCount();i++){
        CIDItemBase* pItem=GetValue(i);
        if(pItem){
            if(pItem->GetID()==AID){
                iResult=i;
                break;
            }
        }
    }
    return iResult;
}

// 根据一个 ID 获得一个节点
CIDItemBase* CItemListVector::GetValueOfID(int AID){
    int iIndex=GetIndexOfID(AID);
    if(iIndex>=0) return m_aItem[iIndex];
    else          return NULL;
}

// 根据名称获得一个节点
// 2006.5.25
CIDItemBase* CItemListVector::GetValueOfName(CString AName)
{
    int iCount=(int)GetItemCount();
    for(int i=0;i<iCount;i++){
        CIDItemBase* pItem=GetValue(i);
        if(pItem){
            if(pItem->GetName()==AName){
                return pItem;
            }
        }
    }
    return NULL;
}





}//namespace