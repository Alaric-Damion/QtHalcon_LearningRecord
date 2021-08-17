
/********************************************************************
  ���ܣ��ַ��������
  ���ߣ�÷�ĺ�
  ���ڣ�2003
  �汾��1.0
  ˵����
*********************************************************************/

#ifndef _sun_str_h
#define _sun_str_h

#include "SunDefine.h"
#include <string>
using namespace std;

#define StrCopySafe(s,t) _tcscpy_s(s,1024*1024,t)

namespace sunLib{

// ****************************** �ַ���������� ******************************
// ����ļ�·������\��
SUN_DLL_DEFINE CString GetFilePath(CString AFileName);
// ����ļ���
SUN_DLL_DEFINE CString GetFileName(CString AFilePath);
// ����ļ�����չ��(a_bExtend:True ������չ�� ���򷵻ز�����չ�����ļ���)
SUN_DLL_DEFINE CString GetFileNameExtend(CString a_sFileName,bool a_bExtend);
// ����ת��Ϊ�ַ���
SUN_DLL_DEFINE CString IntToStr(int AValue);
SUN_DLL_DEFINE CString IntToStr(INT64 AValue);
// ������ת��Ϊ�ַ���
SUN_DLL_DEFINE CString FloatToStr(double AValue);
// �ַ����ϲ�����
SUN_DLL_DEFINE CString Format(const _TCHAR *strLine, ...);
#ifdef _UNICODE
SUN_DLL_DEFINE wstring FormatString(const _TCHAR *strLine, ...);
#else
SUN_DLL_DEFINE string FormatString(const _TCHAR *strLine, ...);
#endif
// �����һ���ָ���������һ���ִ�
SUN_DLL_DEFINE CString GetStringSign(CString ASource,CString AFind,int AIndex);
// ���ܣ��ֽ��зָ��������⣩���ַ���
SUN_DLL_DEFINE int ExpandStrToArray(CString a_sSource,CString a_sSign,CStringArray& a_aExp);
// �ж�һ���ַ����Ƿ���ת��Ϊ����
SUN_DLL_DEFINE BOOL IsNumeric(CString AValue);
// �ַ�����ʾһ������(����)
SUN_DLL_DEFINE CString ConvPercentToString(double AValue,double AMax,CString ABack,CString AFront,int ACount);
// ת������������Ϊ�ַ���
SUN_DLL_DEFINE void TranBinaryToStr(IN const BYTE* a_pBin,IN const UINT a_iSize,OUT _TCHAR** a_pStr);
// �����ֽ���(����Ȼ���������ֽ���) 2016.7.28
SUN_DLL_DEFINE CString TranslateBytesToNormal(__int64 a_iBytes);
// ����һ���ַ������ļ�
SUN_DLL_DEFINE bool SaveStringToFile(CString AString,const CString AFileName);
// �������ݵ��ļ� 2009.8.25
SUN_DLL_DEFINE bool SaveLogToFile(CString a_sLog,CString a_sFileTitle);
SUN_DLL_DEFINE bool SaveTextToFile(CString a_sLog,CString a_sFileName);
// �ַ��������ж�
SUN_DLL_DEFINE bool TextRuleCheckEx(LPCTSTR a_sTextRule,LPCTSTR a_sText);
// ��ò�����չ�����ļ���
SUN_DLL_DEFINE CString GetSimpleFileName(CString a_sFileName);
// ת�����ַ���Ϊ���ֽ��ַ���
SUN_DLL_DEFINE string WideToMutilByte(const wstring& _src);
// ת�����ֽ��ַ���Ϊ���ַ���
SUN_DLL_DEFINE wstring MutilByteToWide(const string& _src);



}

#endif