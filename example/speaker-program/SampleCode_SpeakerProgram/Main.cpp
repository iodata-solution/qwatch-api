#include "SendData_Proc.h"
#include <stdio.h>


#define BUTTON_ID1 0
#define BUTTON_ID2 1
#define FIELD_ID1 100
#define FIELD_ID2 101
#define FIELD_ID3 102

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static TCHAR szWindowClass[] = L"DesktopApp";
static TCHAR szTitle[] = L"Speakerサンプルプログラム";

HWND hEdit[3];
HWND hBtn[2];
HWND hFld[3];
HINSTANCE hInst;

int WINAPI WinMain(
		_In_ HINSTANCE hInstance,
		_In_opt_ HINSTANCE hPrevInstance,
		_In_ LPSTR lpCmdLine,
		_In_ int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wcex;

	//ウィンドウクラスの登録
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
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
		return 0;
	}

	hInst = hInstance;

	//ウィンドウ作成
	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		600, 300,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	//ウィンドウの作成エラー処理
	if (!hWnd) {
		return 0;
	}

	//ウィンドウ表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

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
//　ウィンドウプロシージャーの処理
// 
// 
//	関数名：LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//	引数　：あり
//	戻り値：処理結果
//			
/////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CString cam_ip = L"0.0.0.0";
	static CString cam_pass;
	static CString file_name;

	TCHAR path[MAX_PATH] = L"";
	static CString str_filepath;
	int lastSlashIndex;

	CString initialDir;

	switch (message) {
	case WM_CREATE:
		//エディットの作成（カメラのIPアドレス）
		hEdit[0] = CreateWindow(TEXT("static"), L"カメラのIPアドレス：", SS_SIMPLE | WS_CHILD | WS_VISIBLE, 30, 15, 200, 30, hWnd, (HMENU)10, hInst, NULL);
		//エディットの作成（カメラのパスワード）
		hEdit[1] = CreateWindow(TEXT("static"), L"カメラのパスワード：", SS_SIMPLE | WS_CHILD | WS_VISIBLE, 30, 80, 200, 30, hWnd, (HMENU)10, hInst, NULL);
		//エディットの作成（音声ファイルの保存先の説明）
		hEdit[2] = CreateWindow(TEXT("static"), L"音声ファイルを選択してください。", SS_SIMPLE | WS_CHILD | WS_VISIBLE, 30, 135, 400, 60, hWnd, (HMENU)10, hInst, NULL);
		//音楽再生ボタンの作成
		hBtn[0] = CreateWindowEx(0, L"BUTTON", L"音楽再生", WS_CHILD | WS_VISIBLE | BS_FLAT, 480, 210, 80, 30, hWnd, (HMENU)BUTTON_ID1, hInst, NULL);
		//ファイル選択ボタンの作成
		hBtn[1] = CreateWindowEx(0, L"BUTTON", L"ファイル選択", WS_CHILD | WS_VISIBLE | BS_FLAT, 30, 180, 150, 20, hWnd, (HMENU)BUTTON_ID2, hInst, NULL);
		//IPアドレス取得TEXT
		hFld[0] = CreateWindow(WC_IPADDRESS, L"", WS_CHILD | WS_VISIBLE | BS_FLAT, 220, 15, 200, 22, hWnd, (HMENU)FIELD_ID1, hInst, NULL);
		//パスワード取得TEXT
		hFld[1] = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER , 220, 80, 200, 22, hWnd, (HMENU)FIELD_ID2, hInst, NULL);
		//音声ファイル名TEXT
		hFld[2] = CreateWindow(TEXT("static"), L"ファイルが選択されていません。", WS_VISIBLE | WS_CHILD | SS_CENTER, 220, 180, 250, 22, hWnd, NULL, hInst, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case FIELD_ID1:
			TCHAR ip[16];

			//IPアドレス取得処理
			GetWindowText(hFld[0], ip, sizeof(ip) / sizeof(TCHAR));
			cam_ip = ip;
			break;
		case FIELD_ID2:
			TCHAR pass[64];

			//パスワード取得処理
			GetWindowText(hFld[1], pass, sizeof(pass) / sizeof(TCHAR));
			cam_pass = pass;
			break;
		case BUTTON_ID2:
			OPENFILENAME ofn;

			//ファイルPATHを取得
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = L"All Files (*.*)\0*.*\0";
			ofn.lpstrFile = path;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;

			GetOpenFileName(&ofn);
			
			//ファイル名を取得
			str_filepath = path;
			lastSlashIndex = str_filepath.ReverseFind('\\');
			file_name = str_filepath.Mid(lastSlashIndex + 1);
			SetWindowText(hFld[2], file_name);
			
			break;
		case BUTTON_ID1:
			bool   OK_200 = false;
			SOCKET sock;
			SendData_Proc SendData;

			//IPアドレス未記入時のエラー処理
			if (cam_ip == L"0.0.0.0") {
				MessageBox(0, L"IPアドレスを設定してください。", L"ERROR", MB_ICONHAND);
			}

			//音楽ファイル未記入時のエラー処理
			if (file_name.GetAllocLength() == 0) {
				MessageBox(0, L"音楽ファイルを記入してください。", L"ERROR", MB_ICONHAND);
			}
			else {
				//transmit.cgiの送信処理
				OK_200 = SendData.Transmit(cam_ip, cam_pass, &sock);
				if (OK_200) {

					//音声データの送信処理
					SendData.Send_Data(&sock, file_name);
				}
				else {

					//transmit.cgi送信エラー処理
					MessageBox(0, L"カメラのアドレス、パスワードを見直してください。", L"ERROR", MB_ICONHAND);
				}
			}
			break;
		}
		break;
	case WM_DESTROY:
		//終了処理
		PostQuitMessage(0);
		break;
	default:
		//ウィンドウの破棄//
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}
