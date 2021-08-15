
/********************************************************************
  功能：通用节点管理类
  作者：梅文海
  日期：2006
  版本：1.0
  说明：
*********************************************************************/

#if !defined(AFX_ITEMLIST_H__DF818024_934D_493C_99B5_8357C08B313A__INCLUDED_)
#define AFX_ITEMLIST_H__DF818024_934D_493C_99B5_8357C08B313A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SunDefine.h"
#include <vector>
using namespace std;

namespace sunLib{


#define TRAN_CLASS_NAME_TO_STR(CLASS_NAME) #CLASS_NAME
#define GET_ITEM_CLASS_NAME(CLASS_NAME) \
    sizeof(CLASS_NAME::m_iID); return _T(#CLASS_NAME)
// 保证 CLASS_NAME 一定是一个类型

class CIDItemBase
{
public:
    CIDItemBase(){};
    virtual ~CIDItemBase() {};
    virtual void SetID(int AID)=NULL;                   // 设置 ID
    virtual int GetID()=NULL;                           // 获得 ID
    virtual LPCTSTR GetName()=NULL;                     // 获得名称

    virtual bool Assign(const CIDItemBase& AOther)=NULL;// 拷贝函数(不能设为纯虚方法，否则 ID 不能被设值)
    virtual CIDItemBase* CreateInstance()=NULL;         // 创建该类的实例
    virtual LPCTSTR GetClassName()=NULL;                // 获得类名
};

// 带 ID 的节点基类
// 2005.11.30
class SUN_DLL_DEFINE CIDItem:public CIDItemBase
{
protected:
    int         m_iID;                              // 编号
public:
    CIDItem();
    virtual ~CIDItem(){};
    virtual void SetID(int AID){m_iID=AID;};        // 设置 ID
    virtual int GetID(){return m_iID;};             // 获得 ID
    virtual LPCTSTR GetName(){return _T("");};      // 获得名称

    virtual bool Assign(const CIDItemBase& AOther);     // 拷贝函数(不能设为纯虚方法，否则 ID 不能被设值)
    virtual CIDItemBase* CreateInstance(){return NULL;};// 创建该类的实例
    virtual LPCTSTR GetClassName(){GET_ITEM_CLASS_NAME(CIDItem);};   // 获得类名
};

typedef CArray<CIDItemBase*,CIDItemBase*> CArrayIDItem;


// 带 ID 的列表类
// 2005.11.30
class SUN_DLL_DEFINE CItemList
{
protected:
    CArrayIDItem    m_aItem;
public:
    CItemList();
    virtual ~CItemList();

    virtual bool Assign(CItemList& AOther);         // 拷贝函数

    // 获得最大的编号
    int GetMaxID();
    // 增加一个节点
    void AddItem(CIDItemBase* AItem);
    // 插入一个节点
    void InsertItem(const int a_iIndex,CIDItemBase* AItem);
    // 删除一个节点
    bool DelItem(int AIndex);
    // 删除一个节点
    bool DelItemOfID(int AID);
    // 清空列表
    void Clear();
    // 获得所有节点
    int GetItemCount();
    // 获得一个节点
    CIDItemBase* GetValue(int AIndex);
    // 获得一个索引号
    int GetIndexOfID(int AID);
    // 根据一个 ID 获得一个节点
    CIDItemBase* GetValueOfID(int AID);
    // 根据名称获得一个节点
    CIDItemBase* GetValueOfName(CString AName);

    // 同步数据
    void SyncItemList(CItemList* ATargetList);
    // 新加函数 --------------------------------------------------------------
    // 设置新值 2016.2.29
    void SetValue(int a_iID,CIDItemBase* a_pItem){
        m_aItem.SetAt(a_iID,a_pItem);
    }
    // 重新设置容器大小 2018.7.4
    void ReSetSize(CIDItemBase* a_pItem,const int a_iSize,const int a_iGrowBy=100);
};

class SUN_DLL_DEFINE CItemListVector
{
protected:
    vector<CIDItemBase*>    m_aItem;
public:
    CItemListVector(void);
    virtual ~CItemListVector();

    virtual bool Assign(CItemListVector& AOther);         // 拷贝函数

    // 获得最大的编号
    int GetMaxID();
    // 增加一个节点
    void AddItem(CIDItemBase* AItem);
    // 插入一个节点
    void InsertItem(const int a_iIndex,CIDItemBase* AItem);
    // 删除一个节点
    bool DelItem(int AIndex);
    // 删除一个节点
    bool DelItemOfID(int AID);
    // 清空列表
    void Clear();
    // 获得所有节点
    int GetItemCount();
    // 获得一个节点
    CIDItemBase* GetValue(int AIndex);
    // 获得一个索引号
    int GetIndexOfID(int AID);
    // 根据一个 ID 获得一个节点
    CIDItemBase* GetValueOfID(int AID);
    // 根据名称获得一个节点
    CIDItemBase* GetValueOfName(CString AName);

    // 新加函数 --------------------------------------------------------------
    // 设置新值 2016.2.29
    void SetValue(int a_iID,CIDItemBase* a_pItem){
        m_aItem[a_iID]=a_pItem;
    }
};


}
#endif // !defined(AFX_ITEMLIST_H__DF818024_934D_493C_99B5_8357C08B313A__INCLUDED_)
