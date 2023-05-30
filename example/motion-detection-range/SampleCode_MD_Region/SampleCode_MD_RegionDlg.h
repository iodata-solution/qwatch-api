
// SampleCode_MD_RegionDlg.h : ヘッダー ファイル
//
#include <string>
#include <thread>

using namespace std;
#pragma once



class CSampleCodeMDRegionDlg : public CDialogEx
{

public:
	CSampleCodeMDRegionDlg(CWnd* pParent = nullptr);
	~CSampleCodeMDRegionDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAMPLECODE_MD_REGION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	


protected:
	HICON m_hIcon;

	
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void LoadPicture();
	void OnBnClickedStartMDProc();
	void OnBnClickedCamStatusButton();
	void GetStatusThread(char* Cam_Pass, char* Cam_IP);
	void APIReply_Receive(string command, DWORD waittime, string *result);
	void API_Send(const char* cmds);
	string GetExePath();
	
	
	

	private:
		RECT rec1;

	public:
		char  Cam_IP[16] = "0.0.0.0";
		char  Cam_Pass[64];
		CEdit m_edit[13];
		CStatic m_Picture;
		CStatic m_static;
		CIPAddressCtrl m_ipaddress;
		BOOL del_th;
};
