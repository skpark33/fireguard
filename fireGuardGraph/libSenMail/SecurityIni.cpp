#include "stdafx.h"
#include "SecurityIni.h"
#include "atlenc.h"
//#include "ci/libDebug/ciDebug.h"


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// ������ \n
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
/// �Ҹ��� \n
/////////////////////////////////////////////////////////////////////////////////
CIniValue::~CIniValue()
{
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// ������ \n
/////////////////////////////////////////////////////////////////////////////////
CIniSection::CIniSection()
: m_strSection("")
{
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// �Ҹ��� \n
/////////////////////////////////////////////////////////////////////////////////
CIniSection::~CIniSection()
{
}



/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// ������ \n
/////////////////////////////////////////////////////////////////////////////////
CSecurityIni::CSecurityIni()
{
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// �Ҹ��� \n
/////////////////////////////////////////////////////////////////////////////////
CSecurityIni::~CSecurityIni()
{
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// ���� �д´�. \n
/// @param (LPCSTR) lpstrSection : (in) ���ǰ�
/// @param (LPCSTR) lpstrKey : (in) Ű��
/// @param (LPCSTR) lpstrFilePath : (in) ������ �̸�
/// @return <��: CString> \n
///			<��: ���� ��> \n
/////////////////////////////////////////////////////////////////////////////////
CString CSecurityIni::ReadValue(LPCSTR lpstrSection, LPCSTR lpstrKey, LPCSTR lpstrFilePath)
{
	//ciDEBUG(1,("ReadValue(%s,%s,%s)", lpstrSection, lpstrKey, lpstrFilePath));
	TCHAR cBuffer[1024] = { 0x00 };
	//���� ini ������ ���� ��ȣ�� �Ǿ����� Ȯ���Ѵ�.
	GetPrivateProfileString("root", "IsEncrypt", _T(""), cBuffer, 1024, lpstrFilePath);
	if(!atoi(cBuffer))
	{
		//��ȣ�� ���� �ʾҴٸ� ��ȣȭ �Ѵ�.
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
/// ���� ����. \n
/// @param (LPCSTR) lpstrSection : (in) ���ǰ�
/// @param (LPCSTR) lpstrKey : (in) Ű��
/// @param (LPCSTR) lpstrValue : (in) �����Ϸ��� ��
/// @param (LPCSTR) lpstrFilePath : (in) ������ �̸�
/// @return <��: bool> \n
///			<true: ����> \n
///			<false: ����> \n
/////////////////////////////////////////////////////////////////////////////////
bool CSecurityIni::WriteValue(LPCSTR lpstrSection, LPCSTR lpstrKey, LPCSTR lpstrValue, LPCSTR lpstrFilePath)
{
	TCHAR cBuffer[1024] = { 0x00 };
	//���� ini ������ ���� ��ȣ�� �Ǿ����� Ȯ���Ѵ�.
	GetPrivateProfileString("root", "IsEncrypt", _T(""), cBuffer, 1024, lpstrFilePath);

	if(!atoi(cBuffer))
	{
		//��ȣ�� ���� �ʾҴٸ� ��ȣȭ �Ѵ�.
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
/// ������ ���� ��ȣȭ �Ѵ�. \n
/// @param (LPCSTR) lpstrFilePath : (in) ��ȣȭ�� ���ϰ��
/// @return <��: bool> \n
///			<true: ����> \n
///			<false: ����> \n
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
			//"=" ���� ���� ���� ������ ���...[]
			//������ ���ǰ��� �迭�� �־��ش�.
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

	//���Ͽ� ��ȯ�� ���� ����.
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

	//������ ��ȣ�� �Ǿ��ٰ� root�� ����Ѵ�.
	WritePrivateProfileString("root", "IsEncrypt", "1", lpstrFilePath);

	return true;
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// ��ȣȭ�� ������ �о� Ansi ���ڿ��ε� ������ �����. \n
/// @param (LPCSTR) lpstrSrcPath : (in) ��ȣȭ�� ������ ���
/// @param (LPCSTR) lpstrTargetPath : (in) �����Ϸ��� ��ȣȭ�� ������ ������ ���
/// @return <��: bool> \n
///			<true: ����> \n
///			<false: ����> \n
/////////////////////////////////////////////////////////////////////////////////
bool CSecurityIni::MakeDecryptFile(LPCSTR lpstrSrcPath, LPCSTR lpstrTargetPath)
{
	TCHAR cBuffer[1024] = { 0x00 };
	//���� ini ������ ���� ��ȣ�� �Ǿ����� Ȯ���Ѵ�.
	GetPrivateProfileString("root", "IsEncrypt", _T(""), cBuffer, 1024, lpstrSrcPath);
	if(!atoi(cBuffer))
	{
		//��ȣȭ���� �ʾҴٸ�...
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
			//"=" ���� ���� ���� ������ ���...[]
			//������ ���ǰ��� �迭�� �־��ش�.
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

	//���Ͽ� ��ȯ�� ���� ����.
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

	//������ ��ȣ�� �ȵǾ����� ���
	WritePrivateProfileString("root", "IsEncrypt", "0", lpstrTargetPath);

	return true;
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// �ƽ�Ű ���ڿ��� Base64�� ���ڵ��Ѵ�. \n
/// @param (CString) strAscii : (in) �ƽ�Ű���ڿ�
/// @return <��: CString> \n
///			<��: Base64���ڿ�> \n
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
/// Base64 ���ڿ��� �ƽ�Ű���ڿ��� ���ڵ��Ѵ�. \n
/// @param (CString) strBase64 : (in) Base64���ڿ�
/// @return <��: CString> \n
///			<��: �ƽ�Ű���ڿ�> \n
/////////////////////////////////////////////////////////////////////////////////
CString CSecurityIni::Base64ToAscii(CString strBase64)
{
	int nDecLen = Base64DecodeGetRequiredLength(strBase64.GetLength());

	CString strAscii;
	Base64Decode(strBase64, strBase64.GetLength(), (BYTE*)strAscii.GetBuffer(nDecLen), &nDecLen);
	strAscii.ReleaseBuffer(nDecLen);
	return strAscii;
}
