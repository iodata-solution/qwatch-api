#include <windows.h>
#include <wrl.h>
#include <wil/com.h>
#include <CommCtrl.h>
#include <atlstr.h>
#include "WebView2.h"

#define BUTTON_ID 100

using namespace Microsoft::WRL;

static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("Live viewサンプルプログラム");
static wil::com_ptr<ICoreWebView2Controller>webviewController;
static wil::com_ptr<ICoreWebView2>webview;

HWND hEdit;
HWND hBtn;
HWND hIPFld;
HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////////////////////////////////////////////////////////////////////
//
//	メイン処理
//
//　WebView2を使用し、windowsアプリ内にブラウザを表示させます。
//　メイン処理ではWebView2の環境設定を行っています。
//
//	関数名：int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
//	引数　：あり
//	戻り値：あり
//			
/////////////////////////////////////////////////////////////////////////////
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
	if (!RegisterClassEx(&wcex)){
		return 0;
	}

	hInst = hInstance;

	//ウィンドウ作成
	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1200, 900,
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

	//WebView2の環境作成
	CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
		Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
			[hWnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {

				//WebView2の作成
				env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
					[hWnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
						//WebView2コントロールの取得
						webviewController = controller;
						webviewController->get_CoreWebView2(&webview);

						//WebView2の設定
						wil::com_ptr<ICoreWebView2Settings> settings;
						webview->get_Settings(&settings);
						settings->put_IsScriptEnabled(TRUE);
						settings->put_AreDefaultScriptDialogsEnabled(TRUE);
						settings->put_IsWebMessageEnabled(TRUE);

						//クライアントエリアの座標を取得
						RECT bounds;
						GetClientRect(hWnd, &bounds);
						bounds.top = 100;
						//WebView2の座標設定
						webviewController->put_Bounds(bounds);
						return S_OK;
					}).Get());
				return S_OK;
			}).Get());

	MSG msg;
	 //ウィンドウメッセージの取得
	while (GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

/////////////////////////////////////////////////////////////////////////////
//
//	エディターとボタンの作成、カメラ設定画面の表示
//
//
//	関数名：LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//	引数　：あり
//	戻り値：メッセージの処理結果
//			
/////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case WM_CREATE:
		//エディットの作成
		hEdit = CreateWindow(TEXT("static"), _T("カメラのIPアドレス:"), SS_SIMPLE | WS_CHILD | WS_VISIBLE, 50, 10, 200, 30, hWnd, (HMENU)10, hInst, NULL);
		//ボタンの作成
		hBtn = CreateWindowEx(0, _T("BUTTON"), _T("設定"), WS_CHILD | WS_VISIBLE | BS_FLAT, 300, 35, 50, 30, hWnd, (HMENU)BUTTON_ID, hInst, NULL);
		//IPアドレスコントロールの作成
		hIPFld = CreateWindow(WC_IPADDRESS, L"", WS_CHILD | WS_VISIBLE | BS_FLAT, 50, 35, 200, 30, hWnd, (HMENU)0, hInst, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case BUTTON_ID:
			//ブラウザのURL作成
			TCHAR buf[16];
			GetWindowText(hIPFld, buf, sizeof(buf) / sizeof(TCHAR));
			CString address = buf;
			CString fullpath = L"http://" + address;
			//上記で作成したURLを表示
			webview->Navigate(fullpath);
			break;
		}
		break;
	case WM_SIZE:
		//webviewのサイズコントロール
		if (webviewController != nullptr) {
			RECT bounds;
			GetClientRect(hWnd, &bounds);
			webviewController->put_Bounds(bounds);
		};
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
