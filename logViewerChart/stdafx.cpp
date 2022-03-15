// stdafx.cpp : source file that includes just the standard includes
// [project_name].pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


bool IsLocalExist(LPCTSTR fullpath)
{
	//TraceLog(("IsLocalExist(%s)", fullpath));
	CFileFind ff;
	BOOL bFind = ff.FindFile(fullpath);
	ff.Close();
	if (bFind)
	{
		return true;
	}
	return false;
}

