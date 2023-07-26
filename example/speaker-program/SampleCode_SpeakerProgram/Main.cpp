#include "SendData_Proc.h"
#include <stdio.h>


#define BUTTON_ID1 0
#define BUTTON_ID2 1
#define FIELD_ID1 100
#define FIELD_ID2 101
#define FIELD_ID3 102

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static TCHAR szWindowClass[] = L"DesktopApp";
static TCHAR szTitle[] = L"Speaker�T���v���v���O����";

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

	//�E�B���h�E�N���X�̓o�^
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

	//�E�B���h�E�N���X�̓o�^�G���[����
	if (!RegisterClassEx(&wcex)) {
		return 0;
	}

	hInst = hInstance;

	//�E�B���h�E�쐬
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

	//�E�B���h�E�̍쐬�G���[����
	if (!hWnd) {
		return 0;
	}

	//�E�B���h�E�\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//�E�B���h�E���b�Z�[�W�̎擾
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

/////////////////////////////////////////////////////////////////////////////
//
//�@�E�B���h�E�v���V�[�W���[�̏���
// 
// 
//	�֐����FLRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//	�����@�F����
//	�߂�l�F��������
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
		//�G�f�B�b�g�̍쐬�i�J������IP�A�h���X�j
		hEdit[0] = CreateWindow(TEXT("static"), L"�J������IP�A�h���X�F", SS_SIMPLE | WS_CHILD | WS_VISIBLE, 30, 15, 200, 30, hWnd, (HMENU)10, hInst, NULL);
		//�G�f�B�b�g�̍쐬�i�J�����̃p�X���[�h�j
		hEdit[1] = CreateWindow(TEXT("static"), L"�J�����̃p�X���[�h�F", SS_SIMPLE | WS_CHILD | WS_VISIBLE, 30, 80, 200, 30, hWnd, (HMENU)10, hInst, NULL);
		//�G�f�B�b�g�̍쐬�i�����t�@�C���̕ۑ���̐����j
		hEdit[2] = CreateWindow(TEXT("static"), L"�����t�@�C����I�����Ă��������B", SS_SIMPLE | WS_CHILD | WS_VISIBLE, 30, 135, 400, 60, hWnd, (HMENU)10, hInst, NULL);
		//���y�Đ��{�^���̍쐬
		hBtn[0] = CreateWindowEx(0, L"BUTTON", L"���y�Đ�", WS_CHILD | WS_VISIBLE | BS_FLAT, 480, 210, 80, 30, hWnd, (HMENU)BUTTON_ID1, hInst, NULL);
		//�t�@�C���I���{�^���̍쐬
		hBtn[1] = CreateWindowEx(0, L"BUTTON", L"�t�@�C���I��", WS_CHILD | WS_VISIBLE | BS_FLAT, 30, 180, 150, 20, hWnd, (HMENU)BUTTON_ID2, hInst, NULL);
		//IP�A�h���X�擾TEXT
		hFld[0] = CreateWindow(WC_IPADDRESS, L"", WS_CHILD | WS_VISIBLE | BS_FLAT, 220, 15, 200, 22, hWnd, (HMENU)FIELD_ID1, hInst, NULL);
		//�p�X���[�h�擾TEXT
		hFld[1] = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER , 220, 80, 200, 22, hWnd, (HMENU)FIELD_ID2, hInst, NULL);
		//�����t�@�C����TEXT
		hFld[2] = CreateWindow(TEXT("static"), L"�t�@�C�����I������Ă��܂���B", WS_VISIBLE | WS_CHILD | SS_CENTER, 220, 180, 250, 22, hWnd, NULL, hInst, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case FIELD_ID1:
			TCHAR ip[16];

			//IP�A�h���X�擾����
			GetWindowText(hFld[0], ip, sizeof(ip) / sizeof(TCHAR));
			cam_ip = ip;
			break;
		case FIELD_ID2:
			TCHAR pass[64];

			//�p�X���[�h�擾����
			GetWindowText(hFld[1], pass, sizeof(pass) / sizeof(TCHAR));
			cam_pass = pass;
			break;
		case BUTTON_ID2:
			OPENFILENAME ofn;

			//�t�@�C��PATH���擾
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = L"All Files (*.*)\0*.*\0";
			ofn.lpstrFile = path;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;

			GetOpenFileName(&ofn);
			
			//�t�@�C�������擾
			str_filepath = path;
			lastSlashIndex = str_filepath.ReverseFind('\\');
			file_name = str_filepath.Mid(lastSlashIndex + 1);
			SetWindowText(hFld[2], file_name);
			
			break;
		case BUTTON_ID1:
			bool   OK_200 = false;
			SOCKET sock;
			SendData_Proc SendData;

			//IP�A�h���X���L�����̃G���[����
			if (cam_ip == L"0.0.0.0") {
				MessageBox(0, L"IP�A�h���X��ݒ肵�Ă��������B", L"ERROR", MB_ICONHAND);
			}

			//���y�t�@�C�����L�����̃G���[����
			if (file_name.GetAllocLength() == 0) {
				MessageBox(0, L"���y�t�@�C�����L�����Ă��������B", L"ERROR", MB_ICONHAND);
			}
			else {
				//transmit.cgi�̑��M����
				OK_200 = SendData.Transmit(cam_ip, cam_pass, &sock);
				if (OK_200) {

					//�����f�[�^�̑��M����
					SendData.Send_Data(&sock, file_name);
				}
				else {

					//transmit.cgi���M�G���[����
					MessageBox(0, L"�J�����̃A�h���X�A�p�X���[�h���������Ă��������B", L"ERROR", MB_ICONHAND);
				}
			}
			break;
		}
		break;
	case WM_DESTROY:
		//�I������
		PostQuitMessage(0);
		break;
	default:
		//�E�B���h�E�̔j��//
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}
