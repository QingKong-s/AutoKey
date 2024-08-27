#pragma once
#include "pch.h"

using eck::PCVOID;
using eck::PCBYTE;

constexpr PCWSTR WCN_MAIN = L"WndClass.EckTemplate";

class CApp
{
public:
	static BOOL PreTranslateMessage(const MSG& Msg);

	static void Init();
};

extern CApp* App;