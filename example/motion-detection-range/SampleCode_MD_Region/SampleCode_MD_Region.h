
// SampleCode_MD_Region.h 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		

class CSampleCodeMDRegionApp : public CWinApp
{
public:
	CSampleCodeMDRegionApp();


public:
	virtual BOOL InitInstance();


	DECLARE_MESSAGE_MAP()
};

extern CSampleCodeMDRegionApp theApp;
