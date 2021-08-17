
/*****************************************************************************
// ���ܣ����ú�����
// ���ߣ�÷�ĺ�
// ���ڣ�2003-2007
// �汾��1.0
// ˵������ģ��Ϊ���ɰ汾��������ʹ�ã�ת���븽�ϱ�˵����Ϣ
*****************************************************************************/

#ifndef _INC_DEFINE_H_32564987132178947
#define _INC_DEFINE_H_32564987132178947

#include <afxtempl.h>

namespace sunLib{

#define Bool(Value) (Value?true:false)

typedef CArray<int,int>         CArrayInt;
typedef CArray<byte,byte>       CArrayByte;
typedef CArray<long,long>       CArrayLong;
typedef CArray<CPoint,CPoint>   CArrayPoint;
typedef CArray<CRect,CRect>     CArrayRect;
typedef CArray<float,float>     CArrayFloat;

#define cassert(expn) typedef _TCHAR __C_ASSERT__[(expn)?1:-1]        // �����ڶ���(����ٻᵼ�±���ʧ��)

// ����һ������(2005.6.15)
#define MyDestroy(AValue) \
    if(AValue) AValue->Destroy(); \
    if(AValue) delete AValue; \
AValue=NULL;

#define MyFreeNULL(AValue) \
    if(AValue) {delete AValue; \
AValue=NULL;}

//////////////////////////////////////////////////////////////////////////
// ���峣��
const CString Crlf                      =_T("\r\n");                                // �س�����

// #ifdef SUN_DLL_EXPORT
//     #define SUN_DLL_DEFINE extern "C" _declspec(dllexport)
// #else
//     #define SUN_DLL_DEFINE 
// #endif

#ifdef SUN_DLL_EXPORT
    #define SUN_DLL_DEFINE _declspec(dllexport)
#else
    #define SUN_DLL_DEFINE 
#endif


}// namespace

using namespace sunLib;

#endif
