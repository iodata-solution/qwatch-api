
// SampleCode_discovery.h 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CSampleCodediscoveryApp:
// このクラスの実装については、SampleCode_discovery.cpp を参照してください
//

class CSampleCodediscoveryApp : public CWinApp
{
public:
	CSampleCodediscoveryApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CSampleCodediscoveryApp theApp;
