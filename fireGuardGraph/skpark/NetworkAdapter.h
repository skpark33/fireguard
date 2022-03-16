/*
	Copyright(C) Nishant Sivakumar, 2005. All rights reserved.
	http://blog.voidnish.com
*/

#pragma once


#pragma comment(lib,"Iphlpapi.lib")
#include <Iphlpapi.h>

#pragma comment(lib,"Rpcrt4.lib")

#include <netcon.h>
#include <list>

//Network adapter type
enum
{
	E_ADT_TYPE_UNKNOWN = 0,		///<Unknown type
	E_ADT_TYPE_LOCAL,			///<Lacal area connected type
	E_ADT_TYPE_WIRELESS,		///<Wireless area connected type
	E_ADT_TYPE_ALL = 99			///<All adapter type
};



//! Network adapter info class
/*!
	 \n
*/
class CAdapterInfo
{
public:
	CAdapterInfo()
	{
		m_strAdtName = m_strDescription = m_strMAC = "";
	}

	virtual~CAdapterInfo() { }

	CAdapterInfo& operator= (const CAdapterInfo& info)
	{
		m_strAdtName		=	info.m_strAdtName;
		m_strDescription	=	info.m_strDescription;
		m_strMAC			=	info.m_strMAC;

		return *this;
	}

	std::string	 m_strAdtName;			///<Adapter name(instance id)
	std::string	 m_strDescription;		///<Adapter description
	std::string m_strMAC;				///<Adapter mac address	
};

typedef  std::list<CAdapterInfo*>  listAdtInfo;


//! Network adapter list class
/*!
	 \n
*/
class CNetworkAdapterList
{
public:
	CNetworkAdapterList(void);
	~CNetworkAdapterList(void);

private:
	int				m_nCount;					///<Adapters count
	int				m_nType;					///<Adapter type
	listAdtInfo		m_listAdtInfo;				///<Adapter info list

public:
	bool			IsValid(void);
	int				GetCount(void);
	void			ClearAdtInfolist(void);		///<Clear Adapter info list
	int				LoadAdapter(int nType);		///<Load system Adapters
	CAdapterInfo*	GetAdtInfo(int nIndex);		///<Get adapter info 
	std::string			GetAdtMacAddr(int nIndex);	///<Get adapter mac address
};
