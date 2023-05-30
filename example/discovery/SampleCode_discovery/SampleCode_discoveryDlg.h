
// SampleCode_discoveryDlg.h : ヘッダー ファイル

#include "discovery_proc.h"
#include <string>
#include <thread>
#include <regex>
#include <queue>

#pragma once

using namespace std;

// CSampleCodediscoveryDlg ダイアログ
class CSampleCodediscoveryDlg : public CDialogEx
{
// コンストラクション
public:
	CSampleCodediscoveryDlg(CWnd* pParent = nullptr);	// 標準コンストラクター
	void DisplayThread();
	void OnBnClickedSearchButton();
	void OnBnClickedAccessButton();
	void OnBnClickedNicipButton();
	string Parse(string str, int flag);
	~CSampleCodediscoveryDlg();



// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAMPLECODE_DISCOVERY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CListCtrl m_LC;
	CIPAddressCtrl m_IP;

public:
#define SEARCHADDRESS 0
#define SEARCHDEVICE  1
#define SEARCHUUID    2

	DiscoveryCode* DisCode = NULL;
	char nic_ip[16] = "0.0.0.0";
	queue<string> qbuf;
	
};
