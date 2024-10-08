﻿#pragma once
#include "CApp.h"

class CWndMain : public eck::CForm
{
private:
	eck::CCheckButton m_CBTopLevelWnd{};
	eck::CCheckButton m_CBTopMost{};
	eck::CCheckButton m_CBAutoHome{};
	eck::CCheckButton m_CBClipboard{};
	eck::CHitter m_HTT{};
	eck::CEditExt m_EDHwnd{};

	eck::CEditExt m_EDText{};
	eck::CPushButton m_BTBegin{};

	eck::CLinearLayoutH m_LytTop{};
	eck::CLinearLayoutV m_LytText{};
	eck::CLinearLayoutV m_Lyt{};

	HWND m_hwTarget{};

	HFONT m_hFont{};

	int m_iDpi = USER_DEFAULT_SCREEN_DPI;
	ECK_DS_BEGIN(DPIS)
		ECK_DS_ENTRY(cxyHitter, 32)
		ECK_DS_ENTRY(cxCB, 80)
		ECK_DS_ENTRY(cyCB, 26)
		ECK_DS_ENTRY(cxBtn, 70)
		ECK_DS_ENTRY(cyBtn, 26)
		ECK_DS_ENTRY(Padding, 6)
		ECK_DS_ENTRY(cxyCursor, 32)
		;
	ECK_DS_END_VAR(m_Ds)
private:
	EckInline void UpdateDpiInit(int iDpi)
	{
		m_iDpi = iDpi;
		eck::UpdateDpiSize(m_Ds, iDpi);
	}

	void UpdateDpi(int iDpi);

	void UpdateFixedUISize();

	void ClearRes();

	BOOL OnCreate(HWND hWnd, CREATESTRUCT* lpCreateStruct);

public:
	ECK_CWND_SINGLEOWNER;

	static ATOM RegisterWndClass()
	{
		return eck::EzRegisterWndClass(WCN_MAIN, eck::CS_STDWND);
	}

	~CWndMain();

	LRESULT OnMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	BOOL PreTranslateMessage(const MSG& Msg) override;

	HWND Create(PCWSTR pszText, DWORD dwStyle, DWORD dwExStyle,
		int x, int y, int cx, int cy, HWND hParent, HMENU hMenu, PCVOID pData = NULL) override;
};