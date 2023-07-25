#include "SendData_Proc.h"


/////////////////////////////////////////////////////////////////////////////
//
//	音声データの送信
//
//  1.Socket通じてコンテンツタイプに合致する音声データをカメラに送信します。
//  2.通話を終了する場合は、Socketを切断します。
//
//	関数名：void Send_Data(SOCKET *sock)
//	引数　：あり
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void SendData_Proc::Send_Data(SOCKET* sock, CString filename) {

	FILE  *fp;
	bool  exitthread = false;
	char  file_path[128];

	//音声ファイルパスの作成
	memset(file_path, 0, sizeof(file_path));
	WideCharToMultiByte(CP_ACP, 0, filename, -1, file_path, sizeof(file_path), NULL, NULL);

	//音楽ファイルオープン
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
			//AACパケットのLength取得
			while (((fread(gbuf, 1, 7, fp)) > 0) && !exitthread) {
				nLen = ((gbuf[3] & 0x03) << 11 | (gbuf[4] << 3) & 0x7FF | (gbuf[5] >> 5) & 0x07);
				
				//AACパケットの作成
				fread(&gbuf[7], 1, (size_t)nLen - 7, fp);
				send(*sock, gbuf, nLen, 0);

				//高精度タイマの利用
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
		//transmit.cgi送信エラー処理
		MessageBox(0, L"音声ファイル名が間違っています。", L"ERROR", MB_ICONHAND);
		return;
	}
	closesocket(*sock);
	WSACleanup();

	exitthread = true;
}

/////////////////////////////////////////////////////////////////////////////
//
//	MD5ハッシュ値計算処理
//
//	関数名：CString MD5(CString text)
//	引数　：あり
//	戻り値：あり
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

	//コントテキストの取得
	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
		dwStatus = GetLastError();
		return L"";
	}

	//ハッシュオブジェクトを生成
	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
		dwStatus = GetLastError();
		CryptReleaseContext(hProv, 0);
		return L"";
	}

	//ハッシュ値を計算
	if (CryptHashData(hHash, data, datasize, 0)) {
		cbHash = 16;

		//ハッシュ値計算の取得
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

	//後処理
	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	return md5;
}

/////////////////////////////////////////////////////////////////////////////
//
//	Digest認証処理
//
//	関数名：CString calcDigestResponse(CString nonse, CString user, CString pass)
//	引数　：あり
//	戻り値：あり
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
//	カメラAPI(transmit.cgi)の処理
//
//　1.カメラのHTTP PortにSocket接続します。
//　2.Socketでtransmit.cgiを送信します。
//  3.カメラから401 Unauthorizedの応答があった場合、Diget認証情報を付与して再送します。
//　4.HTTP/1.1 200 OKの応答があること確認します。
//　5.音声通話可能であるか確認します。可能な場合はコンテンツタイプが送信されます。
//
//	関数名：BOOL Transmit(CString m_destip, CString m_destpass)
//	引数　：あり
//	戻り値：あり
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

	//Winsockの初期化
	iResult = WSAStartup(MAKEWORD(2,0), &wsaData);
	if (iResult != NO_ERROR) {
		return 1;
	}

	//送信socketの準備
	*sock = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	inet_pton(addr.sin_family, destip, &addr.sin_addr.S_un.S_addr);

	//socketの接続
	connect(*sock, (struct sockaddr*)&addr, sizeof(addr));
	Sleep(100);

RETRY:
	if (retry != 0) {
		closesocket(*sock);

		//送信socketの再準備
		*sock = socket(AF_INET, SOCK_STREAM, 0);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(80);
		inet_pton(addr.sin_family, destip, &addr.sin_addr.S_un.S_addr);

		//socketの再接続
		connect(*sock, (struct sockaddr*)&addr, sizeof(addr));
	}

	//送信するAPIコマンドの作成
	request = L"GET /camera-cgi/audio/transmit.cgi HTTP/1.1\r\nConnection: Keep-Alive\r\nCache-Control: no-cache\r\n";
	if (!nonce.IsEmpty()) {

		//Digest認証処理
		CString res = Digest_Proc(nonce, L"admin", m_destpass);
		request += L"Authorization: Digest username=\"admin\", realm=\"IPCam\", nonce=" + nonce + L", uri=\"/camera-cgi/audio/transmit.cgi\"";
		request += L", response=\"" + res + L"\", qop=\"auth\", nc=00000001, cnonce=\"0123456789abcdef\"\r\n\r\n\r\n\r\n";

	}
	else {
		request += L"\r\n\r\n";
	}
	WideCharToMultiByte(CP_ACP, 0, request, -1, sndBuf, sizeof(sndBuf), NULL, NULL);

	//パケットの送信処理
	iResult = send(*sock, sndBuf, (int)strlen(sndBuf), 0);
	if (iResult == SOCKET_ERROR) {
		int a = WSAGetLastError();
		closesocket(*sock);
		WSACleanup();
		return 1;
	}

	//パケットの受信処理
	recv(*sock, rcvBuf, 4096, 0);
	Sleep(100);
	response = rcvBuf;
	
	//401エラー処理
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

	//200 OK処理
	else if (response.Find(L"200 OK") > 0) {
		if ((response.Find(L"Content-Type") > 0) && (response.Find(L"audio/aac-lc") > 0)) {
			return true;
		}
	}
	return false;
}

