#include <windows.h>
#include <CommCtrl.h>
#include <atlstr.h>
#include <cctype>

#define FIELD_IP     100
#define FIELD_PASS   101
#define FIELD_CLD    102
#define FIELD_UPTIME 103
#define FIELD_RTMP   104
#define FIELD_KEY    105

#define BUTTON_CLD_ENABLE  109
#define BUTTON_CLD_DISABLE 110
#define BUTTON_ENABLE  111
#define BUTTON_DISABLE 112
#define BUTTON_WEB     113
#define BUTTON_SET     114

class WindowClass {
public:
	bool SetUp(HWND hWnd, HINSTANCE hInstance, int nCmdShow);
	void Create_Parts(HWND hWnd, HINSTANCE hInst);
	void API_Send(const char* cmds);
	void Build_and_Send_API(CString, CString, CString, CString, CString, CString, CString, CString);
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



private:
	static LRESULT CALLBACK ManagerProc(HWND, UINT, WPARAM, LPARAM);

	HWND hFldIpAddress;
	HWND hFldPassWord;
	HWND hFldCloundUrl;
	HWND hFldUptime;
	HWND hFldRtmpUrl;
	HWND hFldStreamKey;
	
	HWND hBtnCloudEnable;
	HWND hBtnCloudDisable;
	HWND hBtnSensorEnable;
	HWND hBtnSensorDisable;
	HWND hBtnBrowser;
	HWND hBtnSet;

	CString IPAddress = L"0.0.0.0";
	CString Password;
	CString Cldurl;
	CString Uptime;
	CString Rtmpurl;
	CString Streamkey;
	CString Sensorflg;
	CString Rtmpflg = L"1";
};
