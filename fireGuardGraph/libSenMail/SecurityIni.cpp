#include "stdafx.h"
#include "SecurityIni.h"
#include "atlenc.h"
//#include "ci/libDebug/ciDebug.h"


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// 생성자 \n
/////////////////////////////////////////////////////////////////////////////////

//ciSET_DEBUG(10,"SecurityIni");

CIniValue::CIniValue()
: m_strKey("")
, m_strOrgValue("")
, m_strEncValue("")
{
}

/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// 소멸자 \n
/////////////////////////////////////////////////////////////////////////////////
CIniValue::~CIniValue()
{
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// 생성자 \n
/////////////////////////////////////////////////////////////////////////////////
CIniSection::CIniSection()
: m_strSection("")
{
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// 소멸자 \n
/////////////////////////////////////////////////////////////////////////////////
CIniSection::~CIniSection()
{
}



/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// 생성자 \n
/////////////////////////////////////////////////////////////////////////////////
CSecurityIni::CSecurityIni()
{
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// 소멸자 \n
/////////////////////////////////////////////////////////////////////////////////
CSecurityIni::~CSecurityIni()
{
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// 값을 읽는다. \n
/// @param (LPCSTR) lpstrSection : (in) 섹션값
/// @param (LPCSTR) lpstrKey : (in) 키값
/// @param (LPCSTR) lpstrFilePath : (in) 파일의 이름
/// @return <형: CString> \n
///			<값: 읽은 값> \n
/////////////////////////////////////////////////////////////////////////////////
CString CSecurityIni::ReadValue(LPCSTR lpstrSection, LPCSTR lpstrKey, LPCSTR lpstrFilePath)
{
	//ciDEBUG(1,("ReadValue(%s,%s,%s)", lpstrSection, lpstrKey, lpstrFilePath));
	TCHAR cBuffer[1024] = { 0x00 };
	//먼저 ini 파일의 값이 암호와 되었는지 확인한다.
	GetPrivateProfileString("root", "IsEncrypt", _T(""), cBuffer, 1024, lpstrFilePath);
	if(!atoi(cBuffer))
	{
		//암호와 되지 않았다면 암호화 한다.
		if(!EncryptFile(lpstrFilePath))
		{
			//ciERROR(("EncrytFile failed"));
			return "";
		}//if
	}//if

	GetPrivateProfileString(lpstrSection, lpstrKey, _T(""), cBuffer, 1024, lpstrFilePath);
	CString strRet = Base64ToAscii(cBuffer);
	//ciDEBUG(1,("ReadValue(%s,%s,%s)=%s", lpstrSection, lpstrKey, lpstrFilePath, strRet));

	return strRet;
}

/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// 값을 쓴다. \n
/// @param (LPCSTR) lpstrSection : (in) 섹션값
/// @param (LPCSTR) lpstrKey : (in) 키값
/// @param (LPCSTR) lpstrValue : (in) 설정하려는 값
/// @param (LPCSTR) lpstrFilePath : (in) 파일의 이름
/// @return <형: bool> \n
///			<true: 성공> \n
///			<false: 실패> \n
/////////////////////////////////////////////////////////////////////////////////
bool CSecurityIni::WriteValue(LPCSTR lpstrSection, LPCSTR lpstrKey, LPCSTR lpstrValue, LPCSTR lpstrFilePath)
{
	TCHAR cBuffer[1024] = { 0x00 };
	//먼저 ini 파일의 값이 암호와 되었는지 확인한다.
	GetPrivateProfileString("root", "IsEncrypt", _T(""), cBuffer, 1024, lpstrFilePath);

	if(!atoi(cBuffer))
	{
		//암호와 되지 않았다면 암호화 한다.
		if(!EncryptFile(lpstrFilePath))
		{
			return false;
		}//if
	}//if

	CString strEncryptVal = AsciiToBase64(lpstrValue);
	WritePrivateProfileString(lpstrSection, lpstrKey, strEncryptVal, lpstrFilePath);
	return true;
}



/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// 파일의 값을 암호화 한다. \n
/// @param (LPCSTR) lpstrFilePath : (in) 암호화할 파일경로
/// @return <형: bool> \n
///			<true: 성공> \n
///			<false: 실패> \n
/////////////////////////////////////////////////////////////////////////////////
bool CSecurityIni::EncryptFile(LPCSTR lpstrFilePath)
{
	CStdioFile fileIni;
	if(!fileIni.Open(lpstrFilePath, CFile::modeRead | CFile::shareDenyWrite | CFile::typeText))
	{
		TRACE("File access fail : %s\r\n", lpstrFilePath);
		return false;
	}//if

	CString strLine, strSection, strKey, strVal, strEncryptVal;
	PARY_Section arySection;
	CIniSection* pstSection = NULL;
	CIniValue* pstValue = NULL;
	int nIdx;
	while(fileIni.ReadString(strLine))
	{
		nIdx = strLine.Find("=", 0);
		if(nIdx == -1)
		{
			//"=" 값이 없는 경우는 섹션인 경우...[]
			//이전의 섹션값은 배열에 넣어준다.
			if(pstSection)
			{
				arySection.Add(pstSection);
			}//if

			strSection = strLine;
			strSection.TrimLeft("[");
			strSection.TrimRight("]");
			pstSection = new CIniSection;
			pstSection->m_strSection = strSection;
			
			continue;
		}//if

		strKey = strLine.Left(nIdx);
		strVal = strLine.Right(strLine.GetLength()-(nIdx+1));
		strEncryptVal = AsciiToBase64(strVal);
		pstValue = new CIniValue;
		pstValue->m_strKey = strKey;
		pstValue->m_strOrgValue = strVal;
		pstValue->m_strEncValue = strEncryptVal;
		if(pstSection)
		{
			pstSection->m_paryValue.Add(pstValue);
		}//if		
	}//while

	if(pstSection)
	{
		arySection.Add(pstSection);
	}//if

	fileIni.Close();

	//파일에 변환된 값을 쓴다.
	for(int i=0; i<arySection.GetCount(); i++)
	{
		pstSection = arySection.GetAt(i);
		for(int j=0; j<pstSection->m_paryValue.GetCount(); j++)
		{
			pstValue = pstSection->m_paryValue.GetAt(j);
			WritePrivateProfileString(pstSection->m_strSection, pstValue->m_strKey, pstValue->m_strEncValue, lpstrFilePath);
			//CString strTmp = pstValue->m_strKey;
			//strTmp.Append("_Org");
			//WritePrivateProfileString(pstSection->m_strSection, strTmp, pstValue->m_strOrgValue, lpstrFilePath);
			delete pstValue;
		}//for
		delete pstSection;
	}//for

	//파일이 암호와 되었다고 root에 기록한다.
	WritePrivateProfileString("root", "IsEncrypt", "1", lpstrFilePath);

	return true;
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// 암호화된 파일을 읽어 Ansi 문자열로된 파일을 만든다. \n
/// @param (LPCSTR) lpstrSrcPath : (in) 암호화된 파일의 경로
/// @param (LPCSTR) lpstrTargetPath : (in) 생성하려는 암호화가 해제된 파일의 경로
/// @return <형: bool> \n
///			<true: 성공> \n
///			<false: 실패> \n
/////////////////////////////////////////////////////////////////////////////////
bool CSecurityIni::MakeDecryptFile(LPCSTR lpstrSrcPath, LPCSTR lpstrTargetPath)
{
	TCHAR cBuffer[1024] = { 0x00 };
	//먼저 ini 파일의 값이 암호와 되었는지 확인한다.
	GetPrivateProfileString("root", "IsEncrypt", _T(""), cBuffer, 1024, lpstrSrcPath);
	if(!atoi(cBuffer))
	{
		//암호화되지 않았다면...
		return false;
	}//if

	CStdioFile fileIni;
	if(!fileIni.Open(lpstrSrcPath, CFile::modeRead | CFile::typeText))
	{
		TRACE("File access fail : %s\r\n", lpstrSrcPath);
		return false;
	}//if

	CString strLine, strSection, strKey, strVal, strAnsiVal;
	PARY_Section arySection;
	CIniSection* pstSection = NULL;
	CIniValue* pstValue = NULL;
	int nIdx;
	while(fileIni.ReadString(strLine))
	{
		nIdx = strLine.Find("=", 0);
		if(nIdx == -1)
		{
			//"=" 값이 없는 경우는 섹션인 경우...[]
			//이전의 섹션값은 배열에 넣어준다.
			if(pstSection)
			{
				arySection.Add(pstSection);
			}//if

			strSection = strLine;
			strSection.TrimLeft("[");
			strSection.TrimRight("]");
			pstSection = new CIniSection;
			pstSection->m_strSection = strSection;
			
			continue;
		}//if

		strKey = strLine.Left(nIdx);
		strVal = strLine.Right(strLine.GetLength()-(nIdx+1));
		strAnsiVal = Base64ToAscii(strVal);
		pstValue = new CIniValue;
		pstValue->m_strKey = strKey;
		pstValue->m_strOrgValue = strAnsiVal;
		pstValue->m_strEncValue = strVal;
		if(pstSection)
		{
			pstSection->m_paryValue.Add(pstValue);
		}//if		
	}//while

	if(pstSection)
	{
		arySection.Add(pstSection);
	}//if

	fileIni.Close();

	//파일에 변환된 값을 쓴다.
	for(int i=0; i<arySection.GetCount(); i++)
	{
		pstSection = arySection.GetAt(i);
		for(int j=0; j<pstSection->m_paryValue.GetCount(); j++)
		{
			pstValue = pstSection->m_paryValue.GetAt(j);
			WritePrivateProfileString(pstSection->m_strSection, pstValue->m_strKey, pstValue->m_strOrgValue, lpstrTargetPath);
			//CString strTmp = pstValue->m_strKey;
			//strTmp.Append("_Org");
			//WritePrivateProfileString(pstSection->m_strSection, strTmp, pstValue->m_strOrgValue, lpstrFilePath);
			delete pstValue;
		}//for
		delete pstSection;
	}//for

	//파일이 암호와 안되었음을 기록
	WritePrivateProfileString("root", "IsEncrypt", "0", lpstrTargetPath);

	return true;
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// 아스키 문자열을 Base64로 인코딩한다. \n
/// @param (CString) strAscii : (in) 아스키문자열
/// @return <형: CString> \n
///			<값: Base64문자열> \n
/////////////////////////////////////////////////////////////////////////////////
CString CSecurityIni::AsciiToBase64(CString strAscii)
{
	int nDestLen = Base64EncodeGetRequiredLength(strAscii.GetLength());

	CString strBase64;
	Base64Encode((const BYTE*)(LPCSTR)strAscii, strAscii.GetLength(),strBase64.GetBuffer(nDestLen), &nDestLen);
	strBase64.ReleaseBuffer(nDestLen);

	return strBase64;
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// Base64 문자열을 아스키문자열로 디코딩한다. \n
/// @param (CString) strBase64 : (in) Base64문자열
/// @return <형: CString> \n
///			<값: 아스키문자열> \n
/////////////////////////////////////////////////////////////////////////////////
CString CSecurityIni::Base64ToAscii(CString strBase64)
{
	int nDecLen = Base64DecodeGetRequiredLength(strBase64.GetLength());

	CString strAscii;
	Base64Decode(strBase64, strBase64.GetLength(), (BYTE*)strAscii.GetBuffer(nDecLen), &nDecLen);
	strAscii.ReleaseBuffer(nDecLen);
	return strAscii;
}
