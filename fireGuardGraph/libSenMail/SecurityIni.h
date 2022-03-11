/************************************************************************************/
/*! @file SecurityIni.h
	@brief 보안을 위하여 ini의 각 값을 보안처리하여 read/write 하는 클래스 선언파일
	@remarks
	▶ 작성자: 정운형\n
	▶ 작성일: 2011/06/14\n

************************************************************************************
  - @b 추가 @b 및 @b 변경사항
************************************************************************************

	@b 작성)
	-# <2011/06/14:정운형:최초작성>.

************************************************************************************
Copyright (c) 2008 by SQI Soft
*/
/***********************************************************************************/

#pragma once

#include <afx.h>
#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxtempl.h>


//! 보안을 위하여 ini의 각 값을 보안처리하여 read/write 하는 클래스
/*!
	[Root] 섹션의 IsEncrypt 값을 보고 '1' 값이 아니라면 ini 파일의\n
	모든 value 값을 읽어서 Base64로 암호화 하여 저장한다. \n
*/
class CSecurityIni
{
public:
	CSecurityIni(void);									///<생성장
	virtual ~CSecurityIni(void);									///<소멸자


	CString		ReadValue(LPCSTR lpstrSection,
							LPCSTR lpstrKey,
							LPCSTR lpstrFilePath);		///<값을 읽는다.

	bool		WriteValue(LPCSTR lpstrSection,
							LPCSTR lpstrKey,
							LPCSTR lpstrValue,
							LPCSTR lpstrFilePath);		///<값을 쓴다.

	bool		EncryptFile(LPCSTR lpstrFilePath);		///<파일의 값을 암호화 한다.
	bool		MakeDecryptFile(LPCSTR lpstrSrcPath,
								LPCSTR lpstrTargetPath);///<	
	CString		AsciiToBase64(CString strAscii);		///<아스키 문자열을 Base64로 인코딩한다.
	CString		Base64ToAscii(CString strBase64);		///<Base64 문자열을 아스키문자열로 디코딩한다.
};


//! <ini 파일의 각 섹션의 키와 값을 갖는 클래스>
class CIniValue
{
public:
	CIniValue();										///<생성자
	virtual ~CIniValue();								///<소멸자

	CString		m_strKey;								///<키값
	CString		m_strOrgValue;							///<원래 값
	CString		m_strEncValue;							///<암호화된 값
};

typedef CArray <CIniValue*, CIniValue*>		PARY_Value;

//! <ini 파일의 섹션들을 갖는 클래스>
class CIniSection
{
public:
	CIniSection();										///<생성자
	virtual ~CIniSection();								///<소멸자

	CString		m_strSection;							///<섹션이름
	PARY_Value	m_paryValue;							///<섹션의 값들 배열
};
typedef CArray <CIniSection*, CIniSection*>	PARY_Section;
