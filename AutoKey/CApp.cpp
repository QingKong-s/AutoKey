#include "pch.h"

#include "CWndMain.h"

CApp* App = NULL;

BOOL CApp::PreTranslateMessage(const MSG& Msg)
{
    return FALSE;
}

void CApp::Init()
{
	CWndMain::RegisterWndClass();
}