#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <atlstr.h>
#include <windows.h>
#include <CommCtrl.h>

class SendData_Proc
{
public:
	void Send_Data(SOCKET *sock, CString filename);
	CString MD5_Calc(CString text);
	CString Digest_Proc(CString nonse, CString user, CString pass);
	bool Transmit(CString m_destip, CString m_destpass, SOCKET *sock);
};