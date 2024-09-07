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

	{
		const MARGINS Mar{ .cxRightWidth = m_Ds.Padding };
		m_HTT.Create(NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 0,
			0, 0, m_Ds.cxyHitter, m_Ds.cxyHitter, hWnd, 0);
		m_LytTop.Add(&m_HTT, Mar, eck::LF_FIX_HEIGHT | eck::LF_FIX_WIDTH);

		m_EDHwnd.Create(NULL, WS_CHILD | WS_VISIBLE, WS_EX_CLIENTEDGE,
			0, 0, 0, m_Ds.cyCB, hWnd, 0);
		m_LytTop.Add(&m_EDHwnd, Mar, eck::LF_FIX_HEIGHT | eck::LF_FILL_WIDTH | eck::LF_ALIGN_CENTER, 1);

		m_CBTopLevelWnd.Create(L"不深度搜索", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 0,
			0, 0, m_Ds.cxCB, m_Ds.cyCB, hWnd, 0);
		m_CBTopLevelWnd.SetCheckState(1);
		m_LytTop.Add(&m_CBTopLevelWnd, Mar, eck::LF_FIX_HEIGHT | eck::LF_FIX_WIDTH | eck::LF_ALIGN_CENTER);

		m_CBTopMost.Create(L"总在最前", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 0,
			0, 0, m_Ds.cxCB, m_Ds.cyCB, hWnd, 0);
		m_CBTopMost.SetCheckState(1);
		m_LytTop.Add(&m_CBTopMost, Mar, eck::LF_FIX_HEIGHT | eck::LF_FIX_WIDTH | eck::LF_ALIGN_CENTER);

		m_CBAutoHome.Create(L"自动Home", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 0,
			0, 0, m_Ds.cxCB, m_Ds.cyCB, hWnd, 0);
		m_CBAutoHome.SetCheckState(1);
		m_LytTop.Add(&m_CBAutoHome, Mar, eck::LF_FIX_HEIGHT | eck::LF_FIX_WIDTH | eck::LF_ALIGN_CENTER);

		m_CBClipboard.Create(L"使用剪贴板", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 0,
			0, 0, m_Ds.cxCB, m_Ds.cyCB, hWnd, 0);
		m_CBClipboard.SetCheckState(1);
		m_LytTop.Add(&m_CBClipboard, Mar, eck::LF_FIX_HEIGHT | eck::LF_FIX_WIDTH | eck::LF_ALIGN_CENTER);
	}
	m_Lyt.Add(&m_LytTop, { .cyBottomHeight = m_Ds.Padding }, eck::LF_FILL_WIDTH | eck::LF_FIX_HEIGHT);

	{
		m_EDText.SetAutoWrap(TRUE);
		m_EDText.SetMultiLine(TRUE);
		m_EDText.Create(NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL, WS_EX_CLIENTEDGE,
			0, 0, 0, 0, hWnd, 0);
		m_LytText.Add(&m_EDText, {}, eck::LF_FILL_WIDTH | eck::LF_FILL_HEIGHT, 1);

		m_BTBegin.Create(L"开始", WS_CHILD | WS_VISIBLE, 0,
			0, 0, m_Ds.cxBtn, m_Ds.cyBtn, hWnd, 0);
		m_LytText.Add(&m_BTBegin, { .cyTopHeight = m_Ds.Padding },
			eck::LF_FIX_WIDTH | eck::LF_FIX_HEIGHT | eck::LF_ALIGN_FAR);
	}
	m_Lyt.Add(&m_LytText, {}, eck::LF_FILL_WIDTH | eck::LF_FILL_HEIGHT, 1);

	eck::SetFontForWndAndCtrl(hWnd, m_hFont);

	SetTopMost(TRUE);
	return TRUE;
}

CWndMain::~CWndMain()
{

}

void Send(PCWSTR pszText, int cchText, BOOL bAutoHome,
	BOOL bExtended = FALSE, BOOL bReplaceEndOfLine = TRUE)
{
	if (cchText < 0)
		cchText = (int)wcslen(pszText);
	if (bReplaceEndOfLine)
		for (int i{}; i < cchText;)
		{
			auto ch = pszText[i++];
			if (ch == L'\r' && i < cchText && pszText[i] == L'\n')
			{
				ch = L'\n';
				++i;
			}
			eck::InputChar(ch, bExtended);
			if (bAutoHome && (ch == L'\n' || ch == L'\r'))
			{
				INPUT input{ .type = INPUT_KEYBOARD };
				input.ki.wVk = VK_HOME;
				SendInput(1, &input, sizeof(input));

				input.ki.dwFlags |= KEYEVENTF_KEYUP;
				SendInput(1, &input, sizeof(input));
			}
		}
	else
	{
		EckCounter(cchText, i)
		{
			eck::InputChar(pszText[i], bExtended);
			if (bAutoHome && (pszText[i] == L'\n' || pszText[i] == L'\r'))
			{
				INPUT input{ .type = INPUT_KEYBOARD };
				input.ki.wVk = VK_HOME;
				SendInput(1, &input, sizeof(input));

				input.ki.dwFlags |= KEYEVENTF_KEYUP;
				SendInput(1, &input, sizeof(input));
			}
		}
	}
}

LRESULT CWndMain::OnMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
	{
		const auto d = m_Ds.Padding;
		m_Lyt.LoSetPosSize(d, d, LOWORD(lParam) - d * 2, HIWORD(lParam) - d * 2);
		m_Lyt.LoCommit();
	}
	return 0;

	case WM_NOTIFY:
	{
		const auto* const pnmhdr = (NMHDR*)lParam;
		if (pnmhdr->hwndFrom == m_HTT.HWnd)
		{
			if (pnmhdr->code == eck::NM_HTT_SEL)
			{
				const auto* const p = (eck::NMHTTSEL*)lParam;
				if (m_CBTopLevelWnd.GetCheckState())
					m_hwTarget = WindowFromPoint(p->pt);
				else
					m_hwTarget = eck::WndFromPoint(p->pt);
				eck::CWnd w(m_hwTarget);
				m_EDHwnd.SetText(eck::Format(L"0x%08X(%s;%s)",
					(UINT)(UINT_PTR)m_hwTarget, w.GetText().Data(), w.GetClsName().Data()).Data());
			}
		}
	}
	return 0;

	case WM_CREATE:
		return HANDLE_WM_CREATE(hWnd, wParam, lParam, OnCreate);
	case WM_COMMAND:
	{
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
			if ((HWND)lParam == m_BTBegin.HWnd)
			{
				const auto rsText = (m_CBClipboard.GetCheckState() ?
					eck::GetClipboardString() : m_EDText.GetText());
				if (!rsText.IsEmpty())
				{
					SetForegroundWindow(m_hwTarget);
					Send(rsText.Data(), rsText.Size(), m_CBAutoHome.GetCheckState());
				}
			}
			else if ((HWND)lParam == m_CBTopMost.HWnd)
			{
				SetTopMost(m_CBTopMost.GetCheckState());
			}
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
			const auto ptc = eck::GetThreadCtx();
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