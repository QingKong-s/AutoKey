#include "pch.h"

#include "CWndMain.h"

void CWndMain::UpdateDpi(int iDpi)
{
	const int iDpiOld = m_iDpi;
	UpdateDpiInit(iDpi);

	// TODO: 更新字体大小
	HFONT hFont = eck::ReCreateFontForDpiChanged(m_hFont, iDpi, iDpiOld);
	eck::SetFontForWndAndCtrl(HWnd, hFont, FALSE);
	std::swap(m_hFont, hFont);
	DeleteObject(hFont);

	UpdateFixedUISize();
}

void CWndMain::UpdateFixedUISize()
{
	// TODO: 更新固定大小的控件
	
}

void CWndMain::ClearRes()
{
	DeleteObject(m_hFont);
}

BOOL CWndMain::OnCreate(HWND hWnd, CREATESTRUCT* lpCreateStruct)
{
	eck::GetThreadCtx()->UpdateDefColor();

	UpdateDpiInit(eck::GetDpi(hWnd));
	m_hFont = eck::EzFont(L"微软雅黑", 9);
	m_BT.Create(L"Button", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0,
		10, 10, m_Ds.cx, m_Ds.cy, hWnd, IDC_BT, NULL);

	eck::SetFontForWndAndCtrl(hWnd, m_hFont);
	return TRUE;
}

CWndMain::~CWndMain()
{
	
}

LRESULT CWndMain::OnMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
	{
	case WM_CREATE:
		return HANDLE_WM_CREATE(hWnd, wParam, lParam, OnCreate);
	case WM_COMMAND:
	{
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
			if (LOWORD(wParam) == IDC_BT)
			{
				MessageBoxW(hWnd, L"Hello World!", L"Hello", MB_OK);
			}
			return 0;
		}
	}
	break;
	case WM_DESTROY:
		ClearRes();
		PostQuitMessage(0);
		return 0;
	case WM_SETTINGCHANGE:
	{
		if (eck::IsColorSchemeChangeMessage(lParam))
		{
			const auto ptc  = eck::GetThreadCtx();
			ptc->UpdateDefColor();
			ptc->SetNcDarkModeForAllTopWnd(ShouldAppsUseDarkMode());
			ptc->SendThemeChangedToAllTopWindow();
			return 0;
		}
	}
	break;
	case WM_DPICHANGED:
		eck::MsgOnDpiChanged(hWnd, lParam);
		UpdateDpi(HIWORD(wParam));
		return 0;
	}
	return CForm::OnMsg(hWnd, uMsg, wParam, lParam);
}

BOOL CWndMain::PreTranslateMessage(const MSG& Msg)
{
    return CForm::PreTranslateMessage(Msg);
}

HWND CWndMain::Create(PCWSTR pszText, DWORD dwStyle, DWORD dwExStyle,
    int x, int y, int cx, int cy, HWND hParent, HMENU hMenu, PCVOID pData)
{
    IntCreate(dwExStyle, WCN_MAIN, pszText, dwStyle,
        x, y, cx, cy, hParent, hMenu, eck::g_hInstance, NULL);
    return NULL;
}