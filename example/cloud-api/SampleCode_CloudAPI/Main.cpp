#include "Window_Class.h"

WindowClass Winclass;

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	static TCHAR szTitle[] = L"クラウドAPIサンプルプログラム";
	static TCHAR szWindowClass[] = L"DesktopApp";
	HWND hWnd = 0;


	if (Winclass.SetUp(hWnd, hInstance, nCmdShow) == false) {
		return 0;
	}

	//ウィンドウメッセージの取得
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

/////////////////////////////////////////////////////////////////////////////
//
//　ウィンドウ作成処理
// 
// 
//	関数名：bool SetUp(HWND hWnd, HINSTANCE hInstance, int nCmdShow)
//	引数　：あり
//	戻り値：ウィンドウ作成の結果
//			
/////////////////////////////////////////////////////////////////////////////
bool WindowClass::SetUp(HWND hWnd, HINSTANCE hInstance, int nCmdShow)
{
	static TCHAR szTitle[] = L"クラウドAPIサンプルプログラム";
	static TCHAR szWindowClass[] = L"DesktopApp";
	WNDCLASSEX wcex;

	//ウィンドウクラスの登録
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ManagerProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	//ウィンドウクラスの登録エラー処理
	if (!RegisterClassEx(&wcex)) {
		return false;
	}

	//ウィンドウ作成
	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		750, 730,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	//Edit、Button、Textの作成
	Create_Parts(hWnd, hInstance);

	//ウィンドウの作成エラー処理
	if (!hWnd) {
		return false;
	}

	//ウィンドウ表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return true;
}

/////////////////////////////////////////////////////////////////////////////
//
//　ウィンドウプロシージャーの処理
// 
// 
//	関数名：LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//	引数　：あり
//	戻り値：処理結果
//			
/////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowClass::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int     len;
	TCHAR   IP_Buff[16];
	TCHAR*  Buff;
	CString Uptimechk;

	switch (message) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case FIELD_IP:
			if (HIWORD(wParam) == EN_KILLFOCUS) {
				//カメラのIPアドレス取得
				GetWindowText(Winclass.hFldIpAddress, IP_Buff, 16);

				//メンバ変数に値を格納
				Winclass.IPAddress = IP_Buff;
			}
			break;
		case FIELD_PASS:
			if (HIWORD(wParam) == EN_KILLFOCUS) {
				//文字数の取得
				len = GetWindowTextLength(Winclass.hFldPassWord);
				len += 1;
				Buff = new TCHAR[len];

				//カメラのパスワード取得
				GetWindowText(Winclass.hFldPassWord, Buff, len);

				//メンバ変数に値を格納
				Winclass.Password = Buff;
			}
			break;
		case FIELD_CLD:
			if (HIWORD(wParam) == EN_KILLFOCUS) {
				//文字数の取得
				len = GetWindowTextLength(Winclass.hFldCloundUrl);
				len += 1;
				Buff = new TCHAR[len];

				//クラウドサーバーURLの取得
				GetWindowText(Winclass.hFldCloundUrl, Buff, len);

				//メンバ変数に値を格納
				Winclass.Cldurl = Buff;
			}
			break;
		case FIELD_UPTIME:
			if (HIWORD(wParam) == EN_KILLFOCUS) {
				//文字数の取得
				len = GetWindowTextLength(Winclass.hFldUptime);
				len += 1;
				Buff = new TCHAR[len];

				//更新間隔の取得
				GetWindowText(Winclass.hFldUptime, Buff, len);

				//メンバ変数に値を格納
				Winclass.Uptime = Buff;
			}
			break;
		case FIELD_RTMP:
			if (HIWORD(wParam) == EN_KILLFOCUS) {
				//文字数の取得
				len = GetWindowTextLength(Winclass.hFldRtmpUrl);
				len += 1;
				Buff = new TCHAR[len];

				//RTMP URLの取得
				GetWindowText(Winclass.hFldRtmpUrl, Buff, len);

				//メンバ変数に値を格納
				Winclass.Rtmpurl = Buff;
			}
			break;
		case FIELD_KEY:
			if (HIWORD(wParam) == EN_KILLFOCUS) {
				//文字数の取得
				len = GetWindowTextLength(Winclass.hFldStreamKey);
				len += 1;
				Buff = new TCHAR[len];

				//ストリームキーの取得
				GetWindowText(Winclass.hFldStreamKey, Buff, len);
				
				//メンバ変数に値を格納
				Winclass.Streamkey = Buff;
			}
			break;
		case BUTTON_CLD_ENABLE:
			//項目グレーアウトを解除
			EnableWindow(Winclass.hFldCloundUrl, TRUE);
			EnableWindow(Winclass.hFldRtmpUrl, TRUE);
			EnableWindow(Winclass.hFldStreamKey, TRUE);
			Winclass.Rtmpflg = L"1";
			break;
		case BUTTON_CLD_DISABLE:
			//各項目をグレーアウト
			SetWindowText(Winclass.hFldCloundUrl, L"");
			SetWindowText(Winclass.hFldRtmpUrl, L"");
			SetWindowText(Winclass.hFldStreamKey, L"");

			EnableWindow(Winclass.hFldCloundUrl, FALSE);
			EnableWindow(Winclass.hFldRtmpUrl, FALSE);
			EnableWindow(Winclass.hFldStreamKey, FALSE);
			Winclass.Rtmpflg = L"0";

			//空に設定
			Winclass.Cldurl.Empty();
			Winclass.Rtmpurl.Empty();
			Winclass.Streamkey.Empty();
			break;
		case BUTTON_ENABLE:
			//項目グレーアウトを解除
			if (IsWindowEnabled(Winclass.hBtnSensorEnable)) {
				EnableWindow(Winclass.hFldUptime, TRUE);
				Winclass.Sensorflg = L"1";
			}
			break;
		case BUTTON_DISABLE:
			//項目グレーアウト
			SetWindowText(Winclass.hFldUptime, L"");
			if (IsWindowEnabled(Winclass.hBtnSensorDisable)) {
				EnableWindow(Winclass.hFldUptime, FALSE);
				Winclass.Sensorflg = L"0";
				Winclass.Uptime = L"0";
			}
			break;
		case BUTTON_WEB:
			//クラウドURLをブラウザで表示
			if (Winclass.Cldurl.GetLength() > 0) {
				ShellExecute(NULL, _T("open"), Winclass.Cldurl, NULL, NULL, SW_SHOWNORMAL);
			}
			else {
				//クラウドURL未記入時のエラー処理
				MessageBox(0, L"クラウドサーバーURLを設定してください。", L"ERROR", MB_ICONHAND);
			}
			break;
		case BUTTON_SET:
			if (SendMessage(Winclass.hBtnSensorEnable, BM_GETCHECK, 0, 0) == BST_CHECKED){
				//数値以外を入力した時のエラー処理
				for (int i = 0; i < Winclass.Uptime.GetLength(); i++) {
					if (!std::isdigit(Winclass.Uptime[i])) {
						MessageBox(0, L"アップロード間隔は数値を入力してください。", L"ERROR", MB_ICONHAND);
						return 0;
					}
				}
			}
			//IPアドレス未記入時のエラー処理
			if (Winclass.IPAddress == L"0.0.0.0") {
				MessageBox(0, L"IPアドレスを設定してください。", L"ERROR", MB_ICONHAND);
			}
			//クラウドモードの有効/無効ボタンが未選択時のエラー処理
			else if (SendMessage(Winclass.hBtnCloudEnable, BM_GETCHECK, 0, 0) != BST_CHECKED && SendMessage(Winclass.hBtnCloudDisable, BM_GETCHECK, 0, 0) != BST_CHECKED) {
				MessageBox(0, L"クラウドモードを有効または無効に設定してください。", L"ERROR", MB_ICONHAND);
			}
			//センサーアップロードの有効/無効ボタンが未選択時のエラー処理
			else if (SendMessage(Winclass.hBtnSensorEnable, BM_GETCHECK, 0 , 0) != BST_CHECKED && SendMessage(Winclass.hBtnSensorDisable, BM_GETCHECK, 0, 0) != BST_CHECKED) {
				MessageBox(0, L"センサー情報のアップロードを有効または無効に設定してください。", L"ERROR", MB_ICONHAND);
			}
			//アップロード間隔未記入時のエラー処理
			else if (Winclass.Uptime.GetLength() == 0) {
				MessageBox(0, L"アップロード間隔を設定してください。", L"ERROR", MB_ICONHAND);
			}
			else {
				//コマンド作成処理
				Build_and_Send_API(Winclass.Password, Winclass.IPAddress, Winclass.Cldurl, Winclass.Sensorflg, Winclass.Uptime, Winclass.Rtmpurl, Winclass.Streamkey, Winclass.Rtmpflg);
			}
			break;
		}
		break;
	case WM_DESTROY:
		//終了処理
		PostQuitMessage(0);
		break;
	default:
		//ウィンドウの破棄
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
//　ウィンドウプロシージャーの管理者
// 
// 
//	関数名：LRESULT CALLBACK ManagerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//	引数　：あり
//	戻り値：処理結果
//			
/////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowClass::ManagerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WindowClass* app;
	if (message == WM_CREATE) {
		// プロパティリストにオブジェクトポインタ(thisポインタ)を設定する
		app = (WindowClass*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
	}
	else {
		// 指定したウィンドウプロシージャの情報を取得
		app = (WindowClass*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}
	// 本当のウィンドウプロシージャを呼び出しメッセージ処理する
	return app->WndProc(hWnd, message, wParam, lParam);
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
void WindowClass::API_Send(const char* cmds)
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
//　Edit、Button、Textの作成
// 
// 
//	関数名：void Create_Parts(HWND hWnd, HINSTANCE hInst)
//	引数　：なし
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void WindowClass::Create_Parts(HWND hWnd, HINSTANCE hInst)
{
	//エディットの作成（カメラのIPアドレス）
	CreateWindow(TEXT("static"), L"カメラのIPアドレス：", SS_SIMPLE | WS_CHILD | WS_VISIBLE, 30, 15, 200, 30, hWnd, (HMENU)10, hInst, NULL);
	//エディットの作成（カメラのパスワード）
	CreateWindow(TEXT("static"), L"カメラのパスワード：", SS_SIMPLE | WS_CHILD | WS_VISIBLE, 350, 15, 200, 30, hWnd, (HMENU)10, hInst, NULL);
	//エディットの作成（クラウドモード有効）
	CreateWindow(TEXT("static"), L"■クラウドモード有効", SS_SIMPLE | WS_CHILD | WS_VISIBLE, 30, 70, 200, 30, hWnd, (HMENU)10, hInst, NULL);
	//エディットの作成（クラウドサーバーURL）
	CreateWindow(TEXT("static"), L"クラウドサーバーURL：", SS_SIMPLE | WS_CHILD | WS_VISIBLE, 30, 150, 200, 30, hWnd, (HMENU)10, hInst, NULL);
	//エディットの作成（センサー情報のアップロード）
	CreateWindow(TEXT("static"), L"■センサー情報のアップロード", SS_SIMPLE | WS_CHILD | WS_VISIBLE, 30, 250, 250, 30, hWnd, (HMENU)10, hInst, NULL);
	//エディットの作成（アップロード間隔）
	CreateWindow(TEXT("static"), L"アップロード間隔：", SS_SIMPLE | WS_CHILD | WS_VISIBLE, 30, 330, 250, 30, hWnd, (HMENU)10, hInst, NULL);
	//エディットの作成（RTMP配信設定）
	CreateWindow(TEXT("static"), L"■RTMP配信設定：", SS_SIMPLE | WS_CHILD | WS_VISIBLE, 30, 410, 250, 30, hWnd, (HMENU)10, hInst, NULL);
	//エディットの作成（設定する際はクラウドモードを有効にしてください）
	CreateWindow(TEXT("static"), L"※設定する際はクラウドモードを有効にしてください。", SS_SIMPLE | WS_CHILD | WS_VISIBLE, 30, 430, 400, 30, hWnd, (HMENU)10, hInst, NULL);
	//エディットの作成（RTMPの配信先URL）
	CreateWindow(TEXT("static"), L"RTMPの配信先URL：", SS_SIMPLE | WS_CHILD | WS_VISIBLE, 30, 480, 400, 30, hWnd, (HMENU)10, hInst, NULL);
	//エディットの作成（ストリームキー）
	CreateWindow(TEXT("static"), L"ストリームキー：", SS_SIMPLE | WS_CHILD | WS_VISIBLE, 30, 560, 400, 30, hWnd, (HMENU)10, hInst, NULL);

	//IPアドレス取得TEXT
	Winclass.hFldIpAddress = CreateWindow(WC_IPADDRESS, L"", WS_CHILD | WS_VISIBLE | BS_FLAT, 190, 15, 150, 22, hWnd, (HMENU)FIELD_IP, hInst, NULL);
	//パスワード取得TEXT
	Winclass.hFldPassWord = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 515, 15, 200, 22, hWnd, (HMENU)FIELD_PASS, hInst, NULL);
	//クラウドサーバーURL取得TEXT
	Winclass.hFldCloundUrl = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 30, 180, 600, 25, hWnd, (HMENU)FIELD_CLD, hInst, NULL);
	//時間取得TEXT
	Winclass.hFldUptime = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 180, 330, 100, 25, hWnd, (HMENU)FIELD_UPTIME, hInst, NULL);
	//RTMPのURL取得TEXT
	Winclass.hFldRtmpUrl = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 30, 510, 600, 25, hWnd, (HMENU)FIELD_RTMP, hInst, NULL);
	//ストリームキーの取得TEXT
	Winclass.hFldStreamKey = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 30, 590, 400, 25, hWnd, (HMENU)FIELD_KEY, hInst, NULL);

	//クラウド有効ボタンの作成
	Winclass.hBtnCloudEnable = CreateWindowEx(0, L"BUTTON", L"有効", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 30, 100, 80, 20, hWnd, (HMENU)BUTTON_CLD_ENABLE, hInst, NULL);
	//クラウド無効ボタンの作成
	Winclass.hBtnCloudDisable = CreateWindowEx(0, L"BUTTON", L"無効", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 140, 100, 80, 20, hWnd, (HMENU)BUTTON_CLD_DISABLE, hInst, NULL);
	//有効ボタンの作成（センサー情報のアップロード）
	Winclass.hBtnSensorEnable = CreateWindowEx(0, L"BUTTON", L"有効", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 30, 280, 80, 20, hWnd, (HMENU)BUTTON_ENABLE, hInst, NULL);
	//無効ボタンの作成（センサー情報のアップロード）
	Winclass.hBtnSensorDisable = CreateWindowEx(0, L"BUTTON", L"無効", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 140, 280, 80, 20, hWnd, (HMENU)BUTTON_DISABLE, hInst, NULL);
	//ブラウザボタンの作成
	Winclass.hBtnBrowser = CreateWindowEx(0, L"BUTTON", L"ブラウザ表示", WS_CHILD | WS_VISIBLE | BS_FLAT, 450, 590, 110, 30, hWnd, (HMENU)BUTTON_WEB, hInst, NULL);
	//設定ボタンの作成
	Winclass.hBtnSet = CreateWindowEx(0, L"BUTTON", L"設定", WS_CHILD | WS_VISIBLE | BS_FLAT, 640, 630, 80, 30, hWnd, (HMENU)BUTTON_SET, hInst, NULL); 
}

/////////////////////////////////////////////////////////////////////////////
//
//　APIのコマンド作成
// 
// 
//	関数名：void Create_API(CString Password, CString IPAddress, 
// 　　　　　　　　　　　　　CString Cldurl, CString Sensorflg, 
// 　　　　　　　　　　　　　CString Uptime, CString Rtmpurl, 
// 　　　　　　　　　　　　　CString Streamkey, CString Rtmpflg)
//	引数　：なし
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void WindowClass::Build_and_Send_API(CString Password, CString IPAddress, CString Cldurl, CString Sensorflg, CString Uptime, CString Rtmpurl, CString Streamkey, CString Rtmpflg)
{
	CString CSendAPI;
	char    APIBuff[1024];

	CSendAPI = L"curl -kX GET --digest -u admin:" + Password + L" \"http://" + IPAddress;
	//クラウドサーバー設定APIの作成
	CSendAPI += L"/camera-cgi/admin/param.cgi?action=update&Cloud_cloudserver_url=" + Cldurl;
	//センサー情報アップロードAPI作成
	CSendAPI += L"&Cloud_EventSensor_enable=" + Sensorflg + L"&Cloud_EventSensor_interval=" + Uptime;
	//RTMPサーバー設定APIの作成
	CSendAPI += L"&Cloud_RTMP_url=" + Rtmpurl + L"/" + Streamkey + L"&Cloud_RTMP_container=" + Rtmpflg + L"&Cloud_Movie_enable=" + Rtmpflg + L"\"";

	memset(APIBuff, 0, 1024);
	WideCharToMultiByte(CP_ACP, 0, CSendAPI, -1, APIBuff, 1024, NULL, NULL);

	API_Send(APIBuff);
}

