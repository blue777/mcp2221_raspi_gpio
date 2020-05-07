
// mcp2221_raspi_gpio.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// Cmcp2221raspigpioApp:
// このクラスの実装については、mcp2221_raspi_gpio.cpp を参照してください
//

class Cmcp2221raspigpioApp : public CWinApp
{
public:
	Cmcp2221raspigpioApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern Cmcp2221raspigpioApp theApp;
