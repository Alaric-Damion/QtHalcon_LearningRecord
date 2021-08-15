
/********************************************************************
  功能：字符串处理库
  作者：梅文海
  日期：2003
  版本：1.0
  说明：
*********************************************************************/

#ifndef _sun_str_h
#define _sun_str_h

#include "SunDefine.h"
#include <string>
using namespace std;

#define StrCopySafe(s,t) _tcscpy_s(s,1024*1024,t)

namespace sunLib{

// ****************************** 字符串处理相关 ******************************
// 获得文件路径（含\）
SUN_DLL_DEFINE CString GetFilePath(CString AFileName);
// 获得文件名
SUN_DLL_DEFINE CString GetFileName(CString AFilePath);
// 获得文件的扩展名(a_bExtend:True 返回扩展名 否则返回不但扩展名的文件名)
SUN_DLL_DEFINE CString GetFileNameExtend(CString a_sFileName,bool a_bExtend);
// 整形转换为字符串
SUN_DLL_DEFINE CString IntToStr(int AValue);
SUN_DLL_DEFINE CString IntToStr(INT64 AValue);
// 浮点数转换为字符串
SUN_DLL_DEFINE CString FloatToStr(double AValue);
// 字符串合并函数
SUN_DLL_DEFINE CString Format(const _TCHAR *strLine, ...);
#ifdef _UNICODE
SUN_DLL_DEFINE wstring FormatString(const _TCHAR *strLine, ...);
#else
SUN_DLL_DEFINE string FormatString(const _TCHAR *strLine, ...);
#endif
// 获得由一个分隔符隔开的一个字串
SUN_DLL_DEFINE CString GetStringSign(CString ASource,CString AFind,int AIndex);
// 功能：分解有分隔符（随意）的字符串
SUN_DLL_DEFINE int ExpandStrToArray(CString a_sSource,CString a_sSign,CStringArray& a_aExp);
// 判断一个字符串是否能转换为整数
SUN_DLL_DEFINE BOOL IsNumeric(CString AValue);
// 字符串表示一个比例(进度)
SUN_DLL_DEFINE CString ConvPercentToString(double AValue,double AMax,CString ABack,CString AFront,int ACount);
// 转换二进制数据为字符串
SUN_DLL_DEFINE void TranBinaryToStr(IN const BYTE* a_pBin,IN const UINT a_iSize,OUT _TCHAR** a_pStr);
// 翻译字节数(用自然语言描述字节数) 2016.7.28
SUN_DLL_DEFINE CString TranslateBytesToNormal(__int64 a_iBytes);
// 保存一个字符串到文件
SUN_DLL_DEFINE bool SaveStringToFile(CString AString,const CString AFileName);
// 保存数据到文件 2009.8.25
SUN_DLL_DEFINE bool SaveLogToFile(CString a_sLog,CString a_sFileTitle);
SUN_DLL_DEFINE bool SaveTextToFile(CString a_sLog,CString a_sFileName);
// 字符串规则判定
SUN_DLL_DEFINE bool TextRuleCheckEx(LPCTSTR a_sTextRule,LPCTSTR a_sText);
// 获得不含扩展名的文件名
SUN_DLL_DEFINE CString GetSimpleFileName(CString a_sFileName);
// 转换宽字符串为多字节字符串
SUN_DLL_DEFINE string WideToMutilByte(const wstring& _src);
// 转换多字节字符串为宽字符串
SUN_DLL_DEFINE wstring MutilByteToWide(const string& _src);



}

#endif