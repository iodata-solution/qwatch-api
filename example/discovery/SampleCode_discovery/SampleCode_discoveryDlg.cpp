
// SampleCode_discoveryDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "SampleCode_discovery.h"
#include "SampleCode_discoveryDlg.h"
#include "afxdialogex.h"

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


CSampleCodediscoveryDlg::CSampleCodediscoveryDlg(CWnd* pParent )
	: CDialogEx(IDD_SAMPLECODE_DISCOVERY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSampleCodediscoveryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_LC);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IP);
}

BEGIN_MESSAGE_MAP(CSampleCodediscoveryDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, CSampleCodediscoveryDlg::OnBnClickedSearchButton)
	ON_BN_CLICKED(IDOK, CSampleCodediscoveryDlg::OnBnClickedAccessButton)
	ON_BN_CLICKED(IDC_BUTTON2, &CSampleCodediscoveryDlg::OnBnClickedNicipButton)
END_MESSAGE_MAP()

BOOL CSampleCodediscoveryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);	
	m_LC.InsertColumn(0, _T("IPアドレス"), LVCFMT_LEFT, 150);
	m_LC.InsertColumn(1, _T("デバイス名"), LVCFMT_LEFT, 200);
	m_LC.InsertColumn(2, _T("UUID"), LVCFMT_LEFT, 300);

	return TRUE;
	
}

void CSampleCodediscoveryDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CSampleCodediscoveryDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CSampleCodediscoveryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/////////////////////////////////////////////////////////////////////////////
//
//	パース処理
// 
//　1.受信パケットからIPアドレス、デバイス名、uuidを解析します。
//
//	関数名：string Parse(string str, int flag)
//	引数　：あり
//	戻り値：あり
//			
/////////////////////////////////////////////////////////////////////////////
string CSampleCodediscoveryDlg::Parse(string str, int flag)
{
	smatch matchbuf;
	string parse_str;

	switch (flag) {
		case SEARCHADDRESS:
		{
			regex address_res("<wsdd:XAddrs>http://([0-9]+).([0-9]+).([0-9]+).([0-9]+)");
			if (regex_search(str, matchbuf, address_res)) {
				parse_str = matchbuf[1].str() + "." + matchbuf[2].str() + "." + matchbuf[3].str() + "." + matchbuf[4].str();
			}
		}
		break;
		case SEARCHDEVICE:
		{
			regex  device_res("onvif://www.onvif.org/hardware/([A-Z]+)-([0-9a-zA-Z]+)");
			if (regex_search(str, matchbuf, device_res)) {
				parse_str = matchbuf[1].str() + "-" + matchbuf[2].str();
			}
			break;
		}
		case SEARCHUUID:
		{
			regex uri_res("<wsa:Address>(?:urn:)?uuid:([0-9a-f]+)-([0-9a-f]+)-([0-9a-f]+)-([0-9a-f]+)-([0-9a-f]+)");
			if (regex_search(str, matchbuf, uri_res)) {
				parse_str = matchbuf[1].str() + "-" + matchbuf[2].str() + "-" + matchbuf[3].str() + "-" + matchbuf[4].str() + "-" + matchbuf[5].str();
			}
		}
		break;
	}
	return parse_str;
}


/////////////////////////////////////////////////////////////////////////////
//
//	検索結果の表示スレッド処理
// 
//  1.パース処理を呼び出します。
//  2.表示処理を呼び出します。
//
//	関数名：void DisplayThread()
//	引数　：なし
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void CSampleCodediscoveryDlg::DisplayThread()
{
	//検索処理終了まで５秒間待機
	Sleep(5000);
	size_t dev_count = qbuf.size();
	for (int devices = 0; devices < dev_count; devices++) {
		string str = qbuf.front();
		qbuf.pop();
		if (str.size() != 0) {
			//IPアドレスの抽出処理
			CString list_display;
			string parse_result = Parse(str, SEARCHADDRESS);
			list_display = parse_result.c_str();
			m_LC.InsertItem(devices, list_display);

			//デバイス名の抽出処理
			parse_result = Parse(str, SEARCHDEVICE);
			list_display = parse_result.c_str();
			m_LC.SetItemText(devices, SEARCHDEVICE, list_display);
			
			//UUIDの抽出処理
			parse_result = Parse(str, SEARCHUUID);
			list_display = parse_result.c_str();
			m_LC.SetItemText(devices, SEARCHUUID, list_display);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// 検索ボタンの処理
// 
// 1.NIC IPアドレスの設定判別を行います。
// 2.検索開始処理を呼び出します。
// 3.検索結果の表示スレッド処理を呼び出します。
//  
// 関数名：void OnBnClickedSearchButton()
// 引数　：なし
// 戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void CSampleCodediscoveryDlg::OnBnClickedSearchButton()
{
	char check_ip[8] = "0.0.0.0";
	m_LC.DeleteAllItems();

	if (strcmp(nic_ip,check_ip) == 0) {
		//nic_ipが指定なしのエラー処理
		MessageBox(L"NIC IPアドレスを設定してください", L"Error", MB_ICONHAND);
		return;
	}
	//ボタン連続押下時の処理
	if (DisCode != NULL) {
		delete DisCode;
		DisCode = NULL;
	}
	//検索開始
	DisCode = new DiscoveryCode();
	DisCode->StartSearch(nic_ip , &qbuf);

	thread display_th(&CSampleCodediscoveryDlg::DisplayThread, this);
	display_th.detach();
}

/////////////////////////////////////////////////////////////////////////////
//
// ブラウザ表示ボタンの処理
// 
// 関数名：void OnBnClickedAccessButton()
// 引数　：なし
// 戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void CSampleCodediscoveryDlg::OnBnClickedAccessButton()
{
	int idx = -1;
	TCHAR tezText[256];
	CString selectdevice;
	
	CListCtrl* pListCtrl1 = (CListCtrl*)GetDlgItem(IDC_LIST1);
	while ((idx=pListCtrl1->GetNextItem(idx, LVNI_SELECTED)) != -1) {
		pListCtrl1->GetItemText(idx,0,tezText,256);
		selectdevice = (CString)tezText;
	}
	selectdevice = L"http://" + selectdevice;
	ShellExecute(NULL, _T("open"), selectdevice, NULL, NULL, SW_SHOWNORMAL);
}

/////////////////////////////////////////////////////////////////////////////
//
// NICの設定処理
// 
// 関数名：void OnBnClickedNicipButton()
// 引数　：なし
// 戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void CSampleCodediscoveryDlg::OnBnClickedNicipButton()
{
	BYTE valueip[4];

	m_IP.GetAddress(valueip[0], valueip[1], valueip[2], valueip[3]);
	sprintf_s(nic_ip, 16, "%d.%d.%d.%d", valueip[0], valueip[1], valueip[2], valueip[3]);
}

/////////////////////////////////////////////////////////////////////////////
//
//	CSampleCodediscoveryDlgクラスのデストラクタ処理
//
//	関数名：~CSampleCodediscoveryDlg()
//	引数　：なし
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
CSampleCodediscoveryDlg::~CSampleCodediscoveryDlg()
{
	if (DisCode != NULL) {
		delete DisCode;
	}
}


