/************************************************************************************/
/*! @file SecurityIni.h
	@brief ������ ���Ͽ� ini�� �� ���� ����ó���Ͽ� read/write �ϴ� Ŭ���� ��������
	@remarks
	�� �ۼ���: ������\n
	�� �ۼ���: 2011/06/14\n

************************************************************************************
  - @b �߰� @b �� @b �������
************************************************************************************

	@b �ۼ�)
	-# <2011/06/14:������:�����ۼ�>.

************************************************************************************
Copyright (c) 2008 by SQI Soft
*/
/***********************************************************************************/

#pragma once

#include <afx.h>
#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxtempl.h>


//! ������ ���Ͽ� ini�� �� ���� ����ó���Ͽ� read/write �ϴ� Ŭ����
/*!
	[Root] ������ IsEncrypt ���� ���� '1' ���� �ƴ϶�� ini ������\n
	��� value ���� �о Base64�� ��ȣȭ �Ͽ� �����Ѵ�. \n
*/
class CSecurityIni
{
public:
	CSecurityIni(void);									///<������
	virtual ~CSecurityIni(void);									///<�Ҹ���


	CString		ReadValue(LPCSTR lpstrSection,
							LPCSTR lpstrKey,
							LPCSTR lpstrFilePath);		///<���� �д´�.

	bool		WriteValue(LPCSTR lpstrSection,
							LPCSTR lpstrKey,
							LPCSTR lpstrValue,
							LPCSTR lpstrFilePath);		///<���� ����.

	bool		EncryptFile(LPCSTR lpstrFilePath);		///<������ ���� ��ȣȭ �Ѵ�.
	bool		MakeDecryptFile(LPCSTR lpstrSrcPath,
								LPCSTR lpstrTargetPath);///<	
	CString		AsciiToBase64(CString strAscii);		///<�ƽ�Ű ���ڿ��� Base64�� ���ڵ��Ѵ�.
	CString		Base64ToAscii(CString strBase64);		///<Base64 ���ڿ��� �ƽ�Ű���ڿ��� ���ڵ��Ѵ�.
};


//! <ini ������ �� ������ Ű�� ���� ���� Ŭ����>
class CIniValue
{
public:
	CIniValue();										///<������
	virtual ~CIniValue();								///<�Ҹ���

	CString		m_strKey;								///<Ű��
	CString		m_strOrgValue;							///<���� ��
	CString		m_strEncValue;							///<��ȣȭ�� ��
};

typedef CArray <CIniValue*, CIniValue*>		PARY_Value;

//! <ini ������ ���ǵ��� ���� Ŭ����>
class CIniSection
{
public:
	CIniSection();										///<������
	virtual ~CIniSection();								///<�Ҹ���

	CString		m_strSection;							///<�����̸�
	PARY_Value	m_paryValue;							///<������ ���� �迭
};
typedef CArray <CIniSection*, CIniSection*>	PARY_Section;
