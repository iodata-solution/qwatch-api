
// SampleCode_MD_RegionDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "SampleCode_MD_Region.h"
#include "SampleCode_MD_RegionDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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



CSampleCodeMDRegionDlg::CSampleCodeMDRegionDlg(CWnd* pParent)
	: CDialogEx(IDD_SAMPLECODE_MD_REGION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//初期化処理
	rec1 = {0,0,0,0};
	del_th = false;

}

void CSampleCodeMDRegionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE1, m_Picture);
	DDX_Control(pDX, IDC_EDIT1, m_edit[0]);
	DDX_Control(pDX, IDC_EDIT2, m_edit[1]);
	DDX_Control(pDX, IDC_EDIT3, m_edit[2]);
	DDX_Control(pDX, IDC_EDIT4, m_edit[3]);
	DDX_Control(pDX, IDC_EDIT5, m_edit[4]);
	DDX_Control(pDX, IDC_EDIT6, m_edit[5]);
	DDX_Control(pDX, IDC_EDIT7, m_edit[6]);
	DDX_Control(pDX, IDC_EDIT8, m_edit[7]);
	DDX_Control(pDX, IDC_EDIT9, m_edit[8]);
	DDX_Control(pDX, IDC_EDIT10, m_edit[9]);
	DDX_Control(pDX, IDC_EDIT11, m_edit[10]);
	DDX_Control(pDX, IDC_EDIT12, m_edit[11]);
	DDX_Control(pDX, IDC_EDIT13, m_edit[12]);
	DDX_Control(pDX, IDC_STATIC1, m_static);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ipaddress);
}

BEGIN_MESSAGE_MAP(CSampleCodeMDRegionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, CSampleCodeMDRegionDlg::OnBnClickedStartMDProc)
	ON_BN_CLICKED(IDC_BUTTON1, CSampleCodeMDRegionDlg::OnBnClickedCamStatusButton)
END_MESSAGE_MAP()


BOOL CSampleCodeMDRegionDlg::OnInitDialog()
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

	m_Picture.GetWindowRect(&rec1);

	m_edit[0].SetWindowTextW(_T("0"));
	m_edit[1].SetWindowTextW(_T("0"));
	m_edit[2].SetWindowTextW(_T("1920"));
	m_edit[3].SetWindowTextW(_T("340"));

	m_edit[4].SetWindowTextW(_T("0"));
	m_edit[5].SetWindowTextW(_T("340"));
	m_edit[6].SetWindowTextW(_T("960"));
	m_edit[7].SetWindowTextW(_T("1080"));

	m_edit[8].SetWindowTextW(_T("960"));
	m_edit[9].SetWindowTextW(_T("820"));
	m_edit[10].SetWindowTextW(_T("1920"));
	m_edit[11].SetWindowTextW(_T("1090"));

	return TRUE;
}

void CSampleCodeMDRegionDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


void CSampleCodeMDRegionDlg::OnPaint()
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
		LoadPicture();
	}
}


HCURSOR CSampleCodeMDRegionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/////////////////////////////////////////////////////////////////////////////
//
//	カメラアドレス/カメラパスワードの登録ボタン処理
//
//  1.IPアドレス、カメラのパスワードを登録します。
//  2.IPアドレスが設定されていなかった場合エラーを表示を行います。
//  3.検知ステータスを取得する処理を呼び出します。
// 
//	関数名：void OnBnClickedCamStatusButton()
//	引数　：なし
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void CSampleCodeMDRegionDlg::OnBnClickedCamStatusButton()
{
	BYTE    valueip[4];
	CString m_campass;
	char Check_IP[8] = "0.0.0.0";
	
	//IPアドレス取得
	m_ipaddress.GetAddress(valueip[0], valueip[1], valueip[2], valueip[3]);
	sprintf_s(Cam_IP, 16, "%d.%d.%d.%d", valueip[0], valueip[1], valueip[2], valueip[3]);
	if (strcmp(Cam_IP, Check_IP) == 0) {
		//nic_ipが指定なしのエラー処理
		MessageBox(L"NIC IPアドレスを設定してください", L"Error", MB_ICONHAND);
		return;
	}
	
	//パスワードの取得
	m_edit[12].GetWindowTextW(m_campass);
	memset(Cam_Pass, 0, 64);
	WideCharToMultiByte(CP_ACP, 0, m_campass, -1, Cam_Pass, sizeof(Cam_Pass), NULL, NULL);

	//検知ステータス取得のマルチスレッド処理
	std::thread getstatus_th(&CSampleCodeMDRegionDlg::GetStatusThread, this, Cam_Pass, Cam_IP);
	getstatus_th.detach();
}

/////////////////////////////////////////////////////////////////////////////
//
//	動作検知情報の取得/ステータスの変更
//
//　1.動作検知情報を取得するコマンドの準備します。
//　2.curlを使い、コマンドを送信します。
//　3.検知状態を定期的に取得します。
//  4.取得した結果に応じてステータスを変更します。
//
//	関数名：void GetStatusThread(char* Cam_Pass, char* Cam_IP)
//	引数　：あり
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void CSampleCodeMDRegionDlg::GetStatusThread(char* Cam_Pass, char* Cam_IP)
{
	string SendAPI;
	string RecvData;

	//動作検知情報を取得するAPIを呼び出します。
	//検知状態の時、MD_Triggerの値は1を返します。検知なしの場合、値は0を返します。
	SendAPI = "curl -sX GET --digest -u admin:";
	SendAPI.append(Cam_Pass);
	SendAPI += " \"http://";
	SendAPI.append(Cam_IP);
	SendAPI += "/camera-cgi/public/getSysteminfo.cgi?action=list&group=MD_Trigger";

	while (1) {
		//API送信
		APIReply_Receive(SendAPI, 0, &RecvData);
		if (RecvData.find("<Trigger>1") == string::npos) {
			//<Trigger>0
			m_static.SetWindowTextW(_T("検知なし"));
		}
		else {
			//<Trigger>1
			m_static.SetWindowTextW(_T("検知中"));
		}
		if (del_th) {
			//強制終了
			break;
		}
		Sleep(2000);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//　カメラAPI（検知範囲指定）の処理
//	1.設定された検知範囲の値を取得します。
//	2.1で取得した値を使い、範囲指定のAPIを準備します。
//	3.curlを使い、APIを送信します。
//
//	関数名：void OnBnClickedStartMDProc()
//	引数　：なし
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void CSampleCodeMDRegionDlg::OnBnClickedStartMDProc() 
{
	char   APIBuff[1024];
	CString region[12];
	CString CSendAPI;
	string SendAPI;
	string RecvData;
	

	//値の取得
	m_edit[0].GetWindowTextW(region[0]);
	m_edit[1].GetWindowTextW(region[1]);
	m_edit[2].GetWindowTextW(region[2]);
	m_edit[3].GetWindowTextW(region[3]);

	m_edit[4].GetWindowTextW(region[4]);
	m_edit[5].GetWindowTextW(region[5]);
	m_edit[6].GetWindowTextW(region[6]);
	m_edit[7].GetWindowTextW(region[7]);

	m_edit[8].GetWindowTextW(region[8]);
	m_edit[9].GetWindowTextW(region[9]);
	m_edit[10].GetWindowTextW(region[10]);
	m_edit[11].GetWindowTextW(region[11]);

	//入力エラー処理
	for (int num = 0; num < 12; num++) {
		CString regionchk = region[num].SpanIncluding(L"0123456789");
		if(region[num] != regionchk){
			MessageBox(L"正しい数値を入力してください。", L"Error", MB_ICONHAND);
			return;
		}
	}

	//エリア1の検知範囲を設定しています。
	CSendAPI += L"&MD_Region1_x0=" + region[0] + L"&MD_Region1_y0=" + region[1] + L"&MD_Region1_x1=" + region[2] + L"&MD_Region1_y1=" + region[3];
	//エリア2の検知範囲を設定しています。
	CSendAPI += L"&MD_Region2_x0=" + region[4] + L"&MD_Region2_y0=" + region[5] + L"&MD_Region2_x1=" + region[6] + L"&MD_Region2_y1=" + region[7];
	//エリア3の範囲範囲を設定しています。
	CSendAPI += L"&MD_Region3_x0=" + region[8] + L"&MD_Region3_y0=" + region[9] + L"&MD_Region3_x1=" + region[10] + L"&MD_Region3_y1=" + region[11];

	memset(APIBuff, 0, 1024);
	WideCharToMultiByte(CP_ACP, 0, CSendAPI, -1, APIBuff, sizeof(APIBuff), NULL, NULL);

	//動作検知、検知感度、検知範囲を設定するAPIを呼び出します。
	SendAPI ="curl -kX GET --digest -u admin:";
	SendAPI.append(Cam_Pass);
	SendAPI += " \"http://";
	SendAPI.append(Cam_IP);
	SendAPI += "/camera-cgi/admin/param.cgi?action=update&AIDetection_enable=1";
	SendAPI += "&AIDetection_detectionType=0&MD_sensitivity=5";
	SendAPI.append(APIBuff);

	//送信処理
	APIReply_Receive(SendAPI, 0, &RecvData);
}

/////////////////////////////////////////////////////////////////////////////
//
//　exeファイル先のディレクトリ検索処理
//  
//  tmpファイルを保存する先を検索します。
//	1.exeファイルのあるディレクトリを判断し、値を返します。
//
//	関数名：string GetExePath()
//	引数　：なし
//	戻り値：保存先のパス
//			
/////////////////////////////////////////////////////////////////////////////
string CSampleCodeMDRegionDlg::GetExePath()
{
	char	ExePath[261];
	string sExePath;

	memset(ExePath, 0, sizeof(ExePath));
	GetModuleFileNameA(NULL, ExePath, sizeof(ExePath));
	if (strstr(ExePath, "SampleCode_MD_Region.exe") != NULL ) {
		//文字列「SampleCode_MD_Region.exe」を削除
		ExePath[strlen(ExePath) - 25] = NULL;
		sExePath = ExePath;
	}
	return sExePath;
}

/////////////////////////////////////////////////////////////////////////////
//
//　画像表示処理
// 
// 
//	関数名：void LoadPicture()
//	引数　：なし
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void CSampleCodeMDRegionDlg::LoadPicture() 
{
	CDC*    cdc;
	CImage	img;
	CString	cscFileName;
	string  PicPath;
	int		iWidth, iHeight;
	CDC		bmpDC;
	CBitmap *cbmp;
	CBitmap *oldbmp;

	//画像ファイルパスの取得。
	PicPath = GetExePath();
	PicPath += "\\picture\\pic.png";
	cscFileName = PicPath.c_str();

	//ピクチャーボックスのサイズ取得
	iWidth = rec1.right - rec1.left;
	iHeight = rec1.bottom - rec1.top;
	
	//画像ファイルの読み込み
	img.Load(cscFileName);
	cbmp = CBitmap::FromHandle(img);

	//ピクチャーボックスに画像を描画
	cdc = m_Picture.GetDC();
	bmpDC.CreateCompatibleDC(cdc);
	oldbmp = bmpDC.SelectObject(cbmp);

	//描画領域に合わせて画像をリサイズ
	cdc->SetStretchBltMode(STRETCH_HALFTONE);
	cdc->SetBrushOrg(0, 0);
	cdc->StretchBlt(0, 0, iWidth, iHeight, &bmpDC, 0, 0, img.GetWidth(), img.GetHeight(), SRCCOPY);
	bmpDC.SelectObject(oldbmp);

	//不要変数の解放
	cbmp->DeleteObject();
	bmpDC.DeleteDC();
	ReleaseDC(cdc);
	img.Destroy();
}

/////////////////////////////////////////////////////////////////////////////
//
//　APIコマンド送信処理
// 
// 
//	関数名：void API_Send(const char* cmds )
//	引数　：あり
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void CSampleCodeMDRegionDlg::API_Send(const char* cmds)
{
	char		buff[4096];
	STARTUPINFOA si;
	PROCESS_INFORMATION	pi;

	::GetStartupInfoA(&si);
	snprintf(buff, sizeof(buff) - 1, "cmd.exe /c %s", cmds);
	int ret = ::CreateProcessA(NULL, (LPSTR)buff, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	::WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

/////////////////////////////////////////////////////////////////////////////
//
//　API応答結果取得処理
// 
// 
//	関数名：void APIReply_Receive(string command, DWORD waittime, string *result)
//	引数　：あり
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void CSampleCodeMDRegionDlg::APIReply_Receive(string command, DWORD waittime, string *result)
{
	char buff[1024] = "0";
	const char	*SendAPI_buff;
	string	CurrentDir;
	

	CurrentDir = GetExePath();

	if (result != NULL) {
		command += "\" > \"" + CurrentDir + "\\tmp\"";
	}

	CurrentDir += "\\tmp";
	SendAPI_buff = command.c_str();

	//コマンド送信処理
	API_Send(SendAPI_buff);
	if (result != NULL) {
		struct stat sb { };
		FILE* fp;
		if ((fopen_s(&fp, CurrentDir.c_str(), "r")) == NULL) {
			if (fp != NULL) {
				stat(CurrentDir.c_str(), &sb);
				fread_s(buff, sizeof(buff), sb.st_size, 1, fp);
				fclose(fp);
			}
			else {
				fclose(fp);
			}
		}
		remove(const_cast<char*>(CurrentDir.data()));
	}
	if (waittime != 0) Sleep(waittime);
	*result = buff;
}

/////////////////////////////////////////////////////////////////////////////
//
//	CSampleCodeMDRegionDlgクラスのデストラクタ処理
//
//	関数名：~CSampleCodeMDRegionDlg()
//	引数　：なし
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
CSampleCodeMDRegionDlg::~CSampleCodeMDRegionDlg()
{
	del_th = true;
}










