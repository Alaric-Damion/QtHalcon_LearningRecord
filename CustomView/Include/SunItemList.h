
/********************************************************************
  ���ܣ�ͨ�ýڵ������
  ���ߣ�÷�ĺ�
  ���ڣ�2006
  �汾��1.0
  ˵����
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
// ��֤ CLASS_NAME һ����һ������

class CIDItemBase
{
public:
    CIDItemBase(){};
    virtual ~CIDItemBase() {};
    virtual void SetID(int AID)=NULL;                   // ���� ID
    virtual int GetID()=NULL;                           // ��� ID
    virtual LPCTSTR GetName()=NULL;                     // �������

    virtual bool Assign(const CIDItemBase& AOther)=NULL;// ��������(������Ϊ���鷽�������� ID ���ܱ���ֵ)
    virtual CIDItemBase* CreateInstance()=NULL;         // ���������ʵ��
    virtual LPCTSTR GetClassName()=NULL;                // �������
};

// �� ID �Ľڵ����
// 2005.11.30
class SUN_DLL_DEFINE CIDItem:public CIDItemBase
{
protected:
    int         m_iID;                              // ���
public:
    CIDItem();
    virtual ~CIDItem(){};
    virtual void SetID(int AID){m_iID=AID;};        // ���� ID
    virtual int GetID(){return m_iID;};             // ��� ID
    virtual LPCTSTR GetName(){return _T("");};      // �������

    virtual bool Assign(const CIDItemBase& AOther);     // ��������(������Ϊ���鷽�������� ID ���ܱ���ֵ)
    virtual CIDItemBase* CreateInstance(){return NULL;};// ���������ʵ��
    virtual LPCTSTR GetClassName(){GET_ITEM_CLASS_NAME(CIDItem);};   // �������
};

typedef CArray<CIDItemBase*,CIDItemBase*> CArrayIDItem;


// �� ID ���б���
// 2005.11.30
class SUN_DLL_DEFINE CItemList
{
protected:
    CArrayIDItem    m_aItem;
public:
    CItemList();
    virtual ~CItemList();

    virtual bool Assign(CItemList& AOther);         // ��������

    // ������ı��
    int GetMaxID();
    // ����һ���ڵ�
    void AddItem(CIDItemBase* AItem);
    // ����һ���ڵ�
    void InsertItem(const int a_iIndex,CIDItemBase* AItem);
    // ɾ��һ���ڵ�
    bool DelItem(int AIndex);
    // ɾ��һ���ڵ�
    bool DelItemOfID(int AID);
    // ����б�
    void Clear();
    // ������нڵ�
    int GetItemCount();
    // ���һ���ڵ�
    CIDItemBase* GetValue(int AIndex);
    // ���һ��������
    int GetIndexOfID(int AID);
    // ����һ�� ID ���һ���ڵ�
    CIDItemBase* GetValueOfID(int AID);
    // �������ƻ��һ���ڵ�
    CIDItemBase* GetValueOfName(CString AName);

    // ͬ������
    void SyncItemList(CItemList* ATargetList);
    // �¼Ӻ��� --------------------------------------------------------------
    // ������ֵ 2016.2.29
    void SetValue(int a_iID,CIDItemBase* a_pItem){
        m_aItem.SetAt(a_iID,a_pItem);
    }
    // ��������������С 2018.7.4
    void ReSetSize(CIDItemBase* a_pItem,const int a_iSize,const int a_iGrowBy=100);
};

class SUN_DLL_DEFINE CItemListVector
{
protected:
    vector<CIDItemBase*>    m_aItem;
public:
    CItemListVector(void);
    virtual ~CItemListVector();

    virtual bool Assign(CItemListVector& AOther);         // ��������

    // ������ı��
    int GetMaxID();
    // ����һ���ڵ�
    void AddItem(CIDItemBase* AItem);
    // ����һ���ڵ�
    void InsertItem(const int a_iIndex,CIDItemBase* AItem);
    // ɾ��һ���ڵ�
    bool DelItem(int AIndex);
    // ɾ��һ���ڵ�
    bool DelItemOfID(int AID);
    // ����б�
    void Clear();
    // ������нڵ�
    int GetItemCount();
    // ���һ���ڵ�
    CIDItemBase* GetValue(int AIndex);
    // ���һ��������
    int GetIndexOfID(int AID);
    // ����һ�� ID ���һ���ڵ�
    CIDItemBase* GetValueOfID(int AID);
    // �������ƻ��һ���ڵ�
    CIDItemBase* GetValueOfName(CString AName);

    // �¼Ӻ��� --------------------------------------------------------------
    // ������ֵ 2016.2.29
    void SetValue(int a_iID,CIDItemBase* a_pItem){
        m_aItem[a_iID]=a_pItem;
    }
};


}
#endif // !defined(AFX_ITEMLIST_H__DF818024_934D_493C_99B5_8357C08B313A__INCLUDED_)
