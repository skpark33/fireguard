#pragma once

#include "afxmt.h"


void OpenIExplorer(CString strParam, int cx = 1024, int cy = 768);


unsigned long getPid(const char* exename, bool likeCond/*=false*/);

typedef struct {
	DWORD pid;
	HWND hwnd;
} find_hwnd_from_pid_t;

static BOOL CALLBACK find_hwnd_from_pid_proc(HWND hwnd, LPARAM lParam);
HWND getWHandle(unsigned long pid);
HWND getWHandle(const char* exename, bool likeCond=false);

CString LoadStringById(UINT nID);

bool deleteOldFile(const char* rootDir, int day, int hour, int min, const char* filter);


