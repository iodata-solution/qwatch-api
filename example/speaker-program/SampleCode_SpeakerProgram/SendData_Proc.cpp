#include "SendData_Proc.h"


/////////////////////////////////////////////////////////////////////////////
//
//	�����f�[�^�̑��M
//
//  1.Socket�ʂ��ăR���e���c�^�C�v�ɍ��v���鉹���f�[�^���J�����ɑ��M���܂��B
//  2.�ʘb���I������ꍇ�́ASocket��ؒf���܂��B
//
//	�֐����Fvoid Send_Data(SOCKET *sock)
//	�����@�F����
//	�߂�l�F�Ȃ�
//			
/////////////////////////////////////////////////////////////////////////////
void SendData_Proc::Send_Data(SOCKET* sock, CString filename) {

	FILE  *fp;
	bool  exitthread = false;
	char  file_path[128];

	//�����t�@�C���p�X�̍쐬
	memset(file_path, 0, sizeof(file_path));
	WideCharToMultiByte(CP_ACP, 0, filename, -1, file_path, sizeof(file_path), NULL, NULL);

	//���y�t�@�C���I�[�v��
	if (fopen_s(&fp, file_path, "rb") == 0) {
		char* gbuf;
		int   nLen = 0;
		
		gbuf = (char*)malloc(sizeof(char) * 1048576);
		HANDLE hProcess = GetCurrentProcess();
		SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);

		LARGE_INTEGER nFreq, nBefore, nAfter;
		DWORD dwTime = 0;
		memset(&nFreq, 0x00, sizeof nFreq);
		memset(&nBefore, 0x00, sizeof nBefore);
		memset(&nAfter, 0x00, sizeof nAfter);

		QueryPerformanceFrequency(&nFreq);
		
		if (gbuf != NULL) {
			//AAC�p�P�b�g��Length�擾
			while (((fread(gbuf, 1, 7, fp)) > 0) && !exitthread) {
				nLen = ((gbuf[3] & 0x03) << 11 | (gbuf[4] << 3) & 0x7FF | (gbuf[5] >> 5) & 0x07);
				
				//AAC�p�P�b�g�̍쐬
				fread(&gbuf[7], 1, (size_t)nLen - 7, fp);
				send(*sock, gbuf, nLen, 0);

				//�����x�^�C�}�̗��p
				QueryPerformanceCounter(&nBefore);
				do {
					QueryPerformanceCounter(&nAfter);
					if (nAfter.QuadPart < nBefore.QuadPart) {
						dwTime = (DWORD)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
					}
					else {
						dwTime = (DWORD)(((LLONG_MAX - nBefore.QuadPart) + nAfter.QuadPart) * 1000 / nFreq.QuadPart);
					}

					if (dwTime > 10) {
						break;
					}
				} while (!exitthread);
			}
		}
		else {
			//gbuf null
			fclose(fp);
			free(gbuf);
			return;
		}
		SetPriorityClass(hProcess, NORMAL_PRIORITY_CLASS);
		fclose(fp);
		free(gbuf);
	}
	else {
		//transmit.cgi���M�G���[����
		MessageBox(0, L"�����t�@�C�������Ԉ���Ă��܂��B", L"ERROR", MB_ICONHAND);
		return;
	}
	closesocket(*sock);
	WSACleanup();

	exitthread = true;
}

/////////////////////////////////////////////////////////////////////////////
//
//	MD5�n�b�V���l�v�Z����
//
//	�֐����FCString MD5(CString text)
//	�����@�F����
//	�߂�l�F����
//			
/////////////////////////////////////////////////////////////////////////////
CString SendData_Proc::MD5_Calc(CString text)
{
	DWORD		dwStatus = 0, datasize = 0, cbHash;
	HCRYPTPROV	hProv = 0;
	HCRYPTHASH	hHash = 0;
	BYTE		data[2048], rgbHash[16];
	CString		md5;
	char		rgbDigits[] = "0123456789abcdef";
	char		buf[8];

	datasize = text.GetLength();
	WideCharToMultiByte(CP_ACP, 0, text, -1, (LPSTR)data, sizeof(data), NULL, NULL);

	//�R���g�e�L�X�g�̎擾
	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
		dwStatus = GetLastError();
		return L"";
	}

	//�n�b�V���I�u�W�F�N�g�𐶐�
	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
		dwStatus = GetLastError();
		CryptReleaseContext(hProv, 0);
		return L"";
	}

	//�n�b�V���l���v�Z
	if (CryptHashData(hHash, data, datasize, 0)) {
		cbHash = 16;

		//�n�b�V���l�v�Z�̎擾
		if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
			for (DWORD i = 0; i < cbHash; i++) {
				sprintf_s(buf, "%c%c", rgbDigits[rgbHash[i] >> 4], rgbDigits[rgbHash[i] & 0xf]);
				md5 += buf;
			}
		}
		else {
			dwStatus = GetLastError();
		}
	}

	//�㏈��
	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	return md5;
}

/////////////////////////////////////////////////////////////////////////////
//
//	Digest�F�؏���
//
//	�֐����FCString calcDigestResponse(CString nonse, CString user, CString pass)
//	�����@�F����
//	�߂�l�F����
//			
/////////////////////////////////////////////////////////////////////////////
CString SendData_Proc::Digest_Proc(CString nonse, CString user, CString pass)
{
	CString f1 = user + L":IPCam:" + pass;
	CString f2 = L"GET:/camera-cgi/audio/transmit.cgi";
	CString res = MD5_Calc(MD5_Calc(f1) + L":" + nonse + L":00000001:0123456789abcdef:auth:" + MD5_Calc(f2));
	return res;
}

/////////////////////////////////////////////////////////////////////////////
//
//	�J����API(transmit.cgi)�̏���
//
//�@1.�J������HTTP Port��Socket�ڑ����܂��B
//�@2.Socket��transmit.cgi�𑗐M���܂��B
//  3.�J��������401 Unauthorized�̉������������ꍇ�ADiget�F�؏���t�^���čđ����܂��B
//�@4.HTTP/1.1 200 OK�̉��������邱�Ɗm�F���܂��B
//�@5.�����ʘb�\�ł��邩�m�F���܂��B�\�ȏꍇ�̓R���e���c�^�C�v�����M����܂��B
//
//	�֐����FBOOL Transmit(CString m_destip, CString m_destpass)
//	�����@�F����
//	�߂�l�F����
//			
/////////////////////////////////////////////////////////////////////////////
bool SendData_Proc::Transmit(CString m_destip, CString m_destpass, SOCKET *sock)
{
	WSAData				wsaData;
	CString				request;
	CString				response;
	CString				nonce;
	struct sockaddr_in	addr;
	static char			sndBuf[4096];
	static char			rcvBuf[4096];
	char				destip[1024];
	int					retry = 0;
	int					iResult;


	memset(destip, 0, sizeof(destip));
	WideCharToMultiByte(CP_ACP, 0, m_destip, -1, destip, sizeof(destip), NULL, NULL);

	//Winsock�̏�����
	iResult = WSAStartup(MAKEWORD(2,0), &wsaData);
	if (iResult != NO_ERROR) {
		return 1;
	}

	//���Msocket�̏���
	*sock = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	inet_pton(addr.sin_family, destip, &addr.sin_addr.S_un.S_addr);

	//socket�̐ڑ�
	connect(*sock, (struct sockaddr*)&addr, sizeof(addr));
	Sleep(100);

RETRY:
	if (retry != 0) {
		closesocket(*sock);

		//���Msocket�̍ď���
		*sock = socket(AF_INET, SOCK_STREAM, 0);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(80);
		inet_pton(addr.sin_family, destip, &addr.sin_addr.S_un.S_addr);

		//socket�̍Đڑ�
		connect(*sock, (struct sockaddr*)&addr, sizeof(addr));
	}

	//���M����API�R�}���h�̍쐬
	request = L"GET /camera-cgi/audio/transmit.cgi HTTP/1.1\r\nConnection: Keep-Alive\r\nCache-Control: no-cache\r\n";
	if (!nonce.IsEmpty()) {

		//Digest�F�؏���
		CString res = Digest_Proc(nonce, L"admin", m_destpass);
		request += L"Authorization: Digest username=\"admin\", realm=\"IPCam\", nonce=" + nonce + L", uri=\"/camera-cgi/audio/transmit.cgi\"";
		request += L", response=\"" + res + L"\", qop=\"auth\", nc=00000001, cnonce=\"0123456789abcdef\"\r\n\r\n\r\n\r\n";

	}
	else {
		request += L"\r\n\r\n";
	}
	WideCharToMultiByte(CP_ACP, 0, request, -1, sndBuf, sizeof(sndBuf), NULL, NULL);

	//�p�P�b�g�̑��M����
	iResult = send(*sock, sndBuf, (int)strlen(sndBuf), 0);
	if (iResult == SOCKET_ERROR) {
		int a = WSAGetLastError();
		closesocket(*sock);
		WSACleanup();
		return 1;
	}

	//�p�P�b�g�̎�M����
	recv(*sock, rcvBuf, 4096, 0);
	Sleep(100);
	response = rcvBuf;
	
	//401�G���[����
	if (response.Find(L"401 Unauthorized") > 0) {
		if (nonce.IsEmpty()) {
			int pos1 = response.Find(L"nonce=", 0) + 6;
			if (response.Mid(pos1, 1) == "\"") {
				pos1 += 1;
			}
			int pos2 = response.Find(L"\"", pos1);
			nonce = response.Mid(pos1, pos2 - pos1);
			if (retry++ < 2) {
				goto RETRY;
			}
		}
	}

	//200 OK����
	else if (response.Find(L"200 OK") > 0) {
		if ((response.Find(L"Content-Type") > 0) && (response.Find(L"audio/aac-lc") > 0)) {
			return true;
		}
	}
	return false;
}

