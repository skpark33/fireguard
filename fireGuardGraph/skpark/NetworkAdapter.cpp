/*
	Copyright(C) Nishant Sivakumar, 2005. All rights reserved.
	http://blog.voidnish.com
*/

#include "stdAfx.h"
#include "NetworkAdapter.h"

CNetworkAdapterList::CNetworkAdapterList(void)
	: m_nCount(0)
	, m_nType(E_ADT_TYPE_UNKNOWN)
{
	
}

CNetworkAdapterList::~CNetworkAdapterList(void)
{
	ClearAdtInfolist();
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// Clear Adapter info list \n
/////////////////////////////////////////////////////////////////////////////////
void CNetworkAdapterList::ClearAdtInfolist(void)
{
	listAdtInfo::iterator itr;
	for(itr=m_listAdtInfo.begin(); itr!=m_listAdtInfo.end(); itr++)
	{
		CAdapterInfo* pclsAdtInfo =  (*itr);
		delete pclsAdtInfo;
	}//for
	m_listAdtInfo.clear();
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// Load system Adapters \n
/// @param (int) nType : (in) Adapters type 
/// @return <��: int> \n
///			Adapters count \n
/////////////////////////////////////////////////////////////////////////////////
int CNetworkAdapterList::LoadAdapter(int nType)
{
	ClearAdtInfolist();
	m_nCount = 0;
	m_nType = E_ADT_TYPE_UNKNOWN;

	if(nType == E_ADT_TYPE_UNKNOWN)
	{
		return -1;
	}//if

	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	BYTE* btAdtInBuff = NULL;
	ULONG OutBufLen = 0;
	int nCnt = 0;

	//Get the required size of the buffer
	if(GetAdaptersInfo(NULL, &OutBufLen) != ERROR_BUFFER_OVERFLOW)
	{
		return -1;
	}//if
/*
	//IP �ּҰ� �� ��ũ��ũ ī�忡 1���̻� �����Ǿ� �������
	//free ���� �޸𸮰� ������.
	int divisor = sizeof IP_ADAPTER_INFO;
	if(sizeof time_t == 8)
		divisor -= 8;

//	int divisor = sizeof IP_ADAPTER_INFO;

	nCnt = OutBufLen/divisor;
	//pAdapterInfo = new IP_ADAPTER_INFO[nCnt];
	pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO)*nCnt);
	memset(pAdapterInfo, 0x00, sizeof(IP_ADAPTER_INFO)*nCnt);
*/
	
	btAdtInBuff = (BYTE*)malloc(sizeof(BYTE)*OutBufLen);
	memset(btAdtInBuff, 0x00, sizeof(BYTE)*OutBufLen);
	pAdapterInfo = (PIP_ADAPTER_INFO)btAdtInBuff;

	//Get info for the adapters
	if(GetAdaptersInfo(pAdapterInfo, &OutBufLen) != ERROR_SUCCESS)
	{
		//Call failed
		//delete[] pAdapterInfo;
		free(pAdapterInfo);
		pAdapterInfo = NULL;
		return -1;
	}//if

	m_nType = nType;

	int index = 0;
	char szBuf[8] = { 0x00 };
	CAdapterInfo* pclsAdtInfo = NULL;
	PIP_ADAPTER_INFO pAdapInfo = pAdapterInfo;
	if(nType == E_ADT_TYPE_ALL)
	{
		while(pAdapInfo)
		{
			m_nCount++;
			pclsAdtInfo						= new CAdapterInfo;
			pclsAdtInfo->m_strAdtName		= pAdapInfo->AdapterName;
			pclsAdtInfo->m_strDescription	= pAdapInfo->Description;

			memset(&szBuf, 0x00, 8);
			std::string strMac;
			for(int i=0; i < (int)pAdapInfo->AddressLength; i++)
			{
				sprintf(szBuf, "%02X-", (int)pAdapInfo->Address[i]);
				strMac += szBuf;
			}//for

			int nLength = strMac.length();
			for(int i=0; i<nLength-1; i++)
			{
				pclsAdtInfo->m_strMAC += strMac.at(i);
			}//for

			m_listAdtInfo.push_back(pclsAdtInfo);
			pAdapInfo = pAdapInfo->Next;
		}//while

		//delete[] pAdapterInfo;
		free(pAdapterInfo);
		return m_nCount;
	}//if

	//������Ʈ���� MediaSubType Ű�� �������� �ʴ� ��찡 �־
	//Name Ű������ "���� ���� ����", "���� ��Ʈ��ũ ����" ���� ���ڿ��� �о �Ǵ��ϵ��� ����
	//nType �� E_ADT_TYPE_LOCAL �̸� "���� ���� ���� (��ȣ)",
	//E_ADT_TYPE_WIRELESS �̸� "���� ��Ʈ��ũ ���� (��ȣ)" �̴�.
	std::string strOpenKey;
	std::string strRootKey = "SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\";
	//DWORD dwType = REG_DWORD;
	//DWORD dwSize = 8;
	DWORD dwMediaSubType = 0;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 128;
	char szName[1024] = { 0x00 };
	wchar_t szUnicodeName[1024] = { 0x00 };
	HKEY hKey;
	listAdtInfo listFront, listEnd;

	// ������xp,7
	// unicode string                    1     2     3     4     5     6     7     8     9     10    11    12    13    14    15    16    17    18    19    20    21    22    23    24    25    26    27    28    29    30    31    32    33    34    35    36    37    38    39    40    41    42    43    44    45    46    47    48    49    50    51    52    53    54
	// Local Area Connection
	BYTE szNetworkTypeEn[42]         = { 0x4C, 0x00, 0x6F, 0x00, 0x63, 0x00, 0x61, 0x00, 0x6C, 0x00, 0x20, 0x00, 0x41, 0x00, 0x72, 0x00, 0x65, 0x00, 0x61, 0x00, 0x20, 0x00, 0x43, 0x00, 0x6F, 0x00, 0x6E, 0x00, 0x6E, 0x00, 0x65, 0x00, 0x63, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6F, 0x00, 0x6E, 0x00 };
	// ���� ���� ����
	BYTE szNetworkTypeKr[16]         = { 0x5C, 0xB8, 0xEC, 0xCE, 0x20, 0x00, 0x01, 0xC6, 0xED, 0xC5, 0x20, 0x00, 0xF0, 0xC5, 0xB0, 0xAC };
	// ��-���� ���ꫢ����
	BYTE szNetworkTypeJp[20]         = { 0xED, 0x30, 0xFC, 0x30, 0xAB, 0x30, 0xEB, 0x30, 0x20, 0x00, 0xA8, 0x30, 0xEA, 0x30, 0xA2, 0x30, 0xA5, 0x63, 0x9A, 0x7D };

	// Wireless Network Connection
	BYTE szNetworkTypeEnWireless[54] = { 0x57, 0x00, 0x69, 0x00, 0x72, 0x00, 0x65, 0x00, 0x6C, 0x00, 0x65, 0x00, 0x73, 0x00, 0x73, 0x00, 0x20, 0x00, 0x4E, 0x00, 0x65, 0x00, 0x74, 0x00, 0x77, 0x00, 0x6F, 0x00, 0x72, 0x00, 0x6B, 0x00, 0x20, 0x00, 0x43, 0x00, 0x6F, 0x00, 0x6E, 0x00, 0x6E, 0x00, 0x65, 0x00, 0x63, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6F, 0x00, 0x6E, 0x00 };
	// ���� ��Ʈ��ũ ����
	BYTE szNetworkTypeKrWireless[20] = { 0x34, 0xBB, 0x20, 0xC1, 0x20, 0x00, 0x24, 0xB1, 0xB8, 0xD2, 0xCC, 0xC6, 0x6C, 0xD0, 0x20, 0x00, 0xF0, 0xC5, 0xB0, 0xAC };
	// �﫤��쫹 �ͫëȫ�-������
	BYTE szNetworkTypeJpWireless[28] = { 0xEF, 0x30, 0xA4, 0x30, 0xE4, 0x30, 0xEC, 0x30, 0xB9, 0x30, 0x20, 0x00, 0xCD, 0x30, 0xC3, 0x30, 0xC8, 0x30, 0xEF, 0x30, 0xFC, 0x30, 0xAF, 0x30, 0xA5, 0x63, 0x9A, 0x7D };

	// ������8�迭
	// Ethernet
	BYTE szNetworkTypeEn_win8[16]    = { 0x45, 0x00, 0x74, 0x00, 0x68, 0x00, 0x65, 0x00, 0x72, 0x00, 0x6E, 0x00, 0x65, 0x00, 0x74, 0x00};
	// �̴���
	BYTE szNetworkTypeKr_win8[ 6]    = { 0x74, 0xC7, 0x54, 0xB3, 0x37, 0xB1};
	// ��-���ͫë�
	BYTE szNetworkTypeJp_win8[12]    = { 0xA4, 0x30, 0xFC, 0x30, 0xB5, 0x30, 0xCD, 0x30, 0xC3, 0x30, 0xC8, 0x30};
	// Wi-Fi
	BYTE szNetworkType_Wifi_win8[10] = { 0x57, 0x00, 0x69, 0x00, 0x2D, 0x00, 0x46, 0x00, 0x69, 0x00};

	while(pAdapInfo)
	{
		::ZeroMemory(szName, sizeof(szName));
		::ZeroMemory(szUnicodeName, sizeof(szUnicodeName));

		strOpenKey = strRootKey;
		strOpenKey += pAdapInfo->AdapterName;
		strOpenKey += "\\Connection";
		LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE , strOpenKey.c_str(), 0, KEY_READ, &hKey);
		printf("KEY=%s\n",strOpenKey.c_str());
		if(ERROR_SUCCESS != lResult)
		{
			pAdapInfo = pAdapInfo->Next;
			continue;
			//return -1;
		}//if

		dwType = REG_SZ;
		dwSize = 1024;
		//lResult = RegQueryValueEx(hKey, "NameEng", NULL, &dwType, (LPBYTE)szName, &dwSize);
		//if(ERROR_SUCCESS != lResult)
		//{
			RegQueryValueExW(hKey, L"Name", NULL, &dwType, (LPBYTE)szUnicodeName, &dwSize);
			lResult = RegQueryValueEx(hKey, "Name", NULL, &dwType, (LPBYTE)szName, &dwSize);
			if(ERROR_SUCCESS != lResult)
			{
				RegCloseKey(hKey);
				pAdapInfo = pAdapInfo->Next;
				continue;
			}
			//return -1;
		//}//if

		printf("Name=%s\n", szName);

		if( nType == E_ADT_TYPE_LOCAL )
		{
			//local
			if( memcmp(szUnicodeName,szNetworkTypeEn,sizeof(szNetworkTypeEn))!=0 &&
				memcmp(szUnicodeName,szNetworkTypeKr,sizeof(szNetworkTypeKr))!=0 &&
				memcmp(szUnicodeName,szNetworkTypeJp,sizeof(szNetworkTypeJp))!=0 &&
				memcmp(szUnicodeName,szNetworkTypeEn_win8,sizeof(szNetworkTypeEn_win8))!=0 &&
				memcmp(szUnicodeName,szNetworkTypeKr_win8,sizeof(szNetworkTypeKr_win8))!=0 &&
				memcmp(szUnicodeName,szNetworkTypeJp_win8,sizeof(szNetworkTypeJp_win8))!=0
				)
			{
				RegCloseKey(hKey);
				pAdapInfo = pAdapInfo->Next;
				continue;
			}
		}
		else //if( nType != E_ADT_TYPE_LOCAL )
		{
			//wireless
			if( memcmp(szUnicodeName,szNetworkTypeEnWireless,sizeof(szNetworkTypeEnWireless))!=0 &&
				memcmp(szUnicodeName,szNetworkTypeKrWireless,sizeof(szNetworkTypeKrWireless))!=0 &&
				memcmp(szUnicodeName,szNetworkTypeJpWireless,sizeof(szNetworkTypeJpWireless))!=0 &&
				memcmp(szUnicodeName,szNetworkType_Wifi_win8,sizeof(szNetworkType_Wifi_win8))!=0
				)
			{
				RegCloseKey(hKey);
				pAdapInfo = pAdapInfo->Next;
				continue;
			}
		}

		//
		m_nCount++;
		pclsAdtInfo						= new CAdapterInfo;
		pclsAdtInfo->m_strAdtName		= pAdapInfo->AdapterName;
		pclsAdtInfo->m_strDescription	= pAdapInfo->Description;

		memset(&szBuf, 0x00, 8);
		std::string strMac;
		for(int i=0; i < (int)pAdapInfo->AddressLength; i++)
		{
			sprintf(szBuf, "%02X-", (int)pAdapInfo->Address[i]);
			strMac += szBuf;
		}//for

		int nLength = strMac.length();
		for(int i=0; i<nLength-1; i++)
		{
			pclsAdtInfo->m_strMAC += strMac.at(i);
		}//for

		//MediaSubType Ű ���� �����ϴ� ��쿡�� ���ϴ� Ÿ�԰� ������ �˻��ϰ�
		//������쿡�� �迭�� �տ�, Ʋ����쿡�� �ڿ� �߰��Ѵ�.
		//��, MediaSubType Ű���� �����ϰ� ���ϴ� Ÿ���� ���� ��������� �迭��
		//�տ� �߰��ϰ� MediaSubType Ű���� ���� ��� ������ �迭�� �߰���
		//���������� MediaSubType Ű���� ���������� Ÿ���� ��ġ���� �ʴ� 
		//��� ������ �迭�� �������� �߰��Ѵ�.
		dwType = REG_DWORD;
		dwSize = 8;
		lResult = RegQueryValueEx(hKey, "MediaSubType", NULL, &dwType, (LPBYTE)&dwMediaSubType, &dwSize);
		if(ERROR_SUCCESS == lResult)
		{
			if(dwMediaSubType == nType)
			{
				listFront.push_back(pclsAdtInfo);
			}
			else
			{
				listEnd.push_back(pclsAdtInfo);
			}//if
		}
		else
		{
			m_listAdtInfo.push_back(pclsAdtInfo);
		}//if

		pAdapInfo = pAdapInfo->Next;
	}//while

	//��� �迭�� �տ� �߰�
	if(listFront.size() != 0)
	{
		listAdtInfo::iterator itr;
		for(itr = listFront.begin(); itr != listFront.end(); itr++)
		{
			CAdapterInfo* pclsAdtInfo =  (*itr);
			m_listAdtInfo.push_front(pclsAdtInfo);
		}//for
	}//if

	//��� �迭�� �ڿ� �߰�
	if(listEnd.size() != 0)
	{
		listAdtInfo::iterator itr;
		for(itr = listEnd.begin(); itr != listEnd.end(); itr++)
		{
			CAdapterInfo* pclsAdtInfo =  (*itr);
			m_listAdtInfo.push_back(pclsAdtInfo);
		}//for
	}//if

	//delete[] pAdapterInfo;
	free(pAdapterInfo);
	return m_nCount;
}


//IsValid returns true if the adapter info has been successfully obtained.
//This function should be called before using the class.
bool CNetworkAdapterList::IsValid(void)
{
	if(m_nCount != 0 && m_nType != E_ADT_TYPE_UNKNOWN)
	{
		return true;
	}//if

	return false;
}

int CNetworkAdapterList::GetCount(void)
{
	if(!IsValid())
		return -1;

	return m_nCount;
}


/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// Get adapter info \n
/// @param (int) nIndex : (in/out) ����
/// @return <��: CAdapterInfo*> \n
///			<CAdapterInfo*: Adapter info class pointer at given index> \n
///			<NULL: On fail> \n
/////////////////////////////////////////////////////////////////////////////////
CAdapterInfo* CNetworkAdapterList::GetAdtInfo(int nIndex)
{
	if(!IsValid())
	{
		return NULL;
	}//if

	if(nIndex >= m_listAdtInfo.size())
	{
		return NULL;
	}//if

	int nIdx = 0;
	listAdtInfo::iterator itr;
	for(itr=m_listAdtInfo.begin(); itr!=m_listAdtInfo.end(); itr++)
	{
		CAdapterInfo* pclsAdtInfo =  (*itr);
		if(nIdx == nIndex)
		{
			return pclsAdtInfo;
		}//if

		nIdx++;
	}//for

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////
/// @remarks
/// Get adapter mac address \n
/// @param (int) nIndex : (in/out) ����
/// @return <��: CAdapterInfo*> \n
///			<string: Adapter mac address string> \n
///			<"": On fail> \n
/////////////////////////////////////////////////////////////////////////////////
std::string CNetworkAdapterList::GetAdtMacAddr(int nIndex)
{
	if(!IsValid())
	{
		return "";
	}//if

	if(nIndex >= m_listAdtInfo.size())
	{
		return "";
	}//if

	int nIdx = 0;
	listAdtInfo::iterator itr;
	for(itr=m_listAdtInfo.begin(); itr!=m_listAdtInfo.end(); itr++)
	{
		CAdapterInfo* pclsAdtInfo =  (*itr);
		if(nIdx == nIndex)
		{
			return pclsAdtInfo->m_strMAC;
		}//if

		nIdx++;
	}//for

	return "";
}

