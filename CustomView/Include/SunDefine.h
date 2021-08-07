
/*****************************************************************************
// 功能：常用函数库
// 作者：梅文海
// 日期：2003-2007
// 版本：1.0
// 说明：该模块为自由版本，可任意使用，转载请附上本说明信息
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

#define cassert(expn) typedef _TCHAR __C_ASSERT__[(expn)?1:-1]        // 编译期断言(传入假会导致编译失败)

// 析构一个对象(2005.6.15)
#define MyDestroy(AValue) \
    if(AValue) AValue->Destroy(); \
    if(AValue) delete AValue; \
AValue=NULL;

#define MyFreeNULL(AValue) \
    if(AValue) {delete AValue; \
AValue=NULL;}

//////////////////////////////////////////////////////////////////////////
// 定义常量
const CString Crlf                      =_T("\r\n");                                // 回车换行

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
