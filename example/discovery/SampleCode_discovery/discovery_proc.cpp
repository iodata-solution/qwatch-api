#include "pch.h"
#include "discovery_proc.h"


/////////////////////////////////////////////////////////////////////////////
//
//	queueの中身削除
//
//	関数名：void Clear(queue<string> &qbuf)
//	引数　：あり
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void DiscoveryCode::Clear(queue<string> qbuf)
{
	queue<string> empty;
	swap(qbuf, empty);
}

/////////////////////////////////////////////////////////////////////////////
//
//	送信スレッド処理
// 
//　1.discoveryパケットを送信します。
// 
//	関数名：void SendThread(SOCKET sock, char* senddata, sockaddr_in destaddr)
//	引数　：あり
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void DiscoveryCode::SendThread(SOCKET sock, char* senddata, sockaddr_in destaddr)
{
	sendto(sock, senddata, 512, 0, (struct sockaddr*)&destaddr, sizeof(destaddr));
}

/////////////////////////////////////////////////////////////////////////////
//
//	受信スレッド処理
// 
//　1.応答パケットを5秒間受信します。
//　2.受信したパケットをqueueにpushします。
//  3.タイムアウト時にsocket切断します。
// 
//	関数名：void ReceiveThread(SOCKET sock, queue<string> *qbuf)
//	引数　：あり
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void DiscoveryCode::ReceiveThread(SOCKET sock, queue<string>* qbuf)
{
	char rcvbuf[2048];
	fd_set fds, readfds;
	timeval timeout;

	FD_ZERO(&readfds);
	FD_SET(sock, &readfds);
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;

	while (1) {
		memcpy(&fds, &readfds, sizeof(fd_set));
		int ret = select(0, &fds, NULL, NULL, &timeout);
		if (del_th) {
			//強制終了
			break;
		}
		if (ret <= 0) {
			//Timeout
			break;
		}
		else {
			recv(sock, rcvbuf, sizeof(rcvbuf), 0);
			qbuf->push(rcvbuf);
			memset(rcvbuf, 0, sizeof(rcvbuf));
		}
	}
	closesocket(sock);
	WSACleanup();
}

/////////////////////////////////////////////////////////////////////////////
//
//	検索開始処理
// 
//　1.WS-Discoveryパケットを作成します。
//　2.Socket生成します。
//　3.送信スレッド処理の呼び出します。
//  4.受信スレッド処理の呼び出します。
//
//	関数名：void StartSearch(char *nic_ip, queue<string> *qbuf)
//	引数　：あり
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
void DiscoveryCode::StartSearch(char* nic_ip, queue<string>* qbuf)
{
	char senddata[] =
		"<?Xml version=\"1.0\" "
		"encoding=\"utf-8\"?>"
		"<soap:Envelope "
		"xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" "
		"xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" "
		"xmlns:wsd=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\">"
		"<soap:Header>"
		"<wsa:MessageID>"
		"uuid:f3477564-4157-42db-bb28-c4c06087e0ad"
		"</wsa:MessageID>"
		"<wsa:To>"
		"urn:schemas-xmlsoap-org:ws:2005:04:discovery"
		"</wsa:To>"
		"<wsa:Action>"
		"http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe"
		"</wsa:Action>"
		"</soap:Header>"
		"<soap:Body>"
		"<wsd:Probe/>"
		"</soap:Body>"
		"</Envelope>";

	//CSampleCodediscoveryDlg CSampleDlg;
	WSADATA wsaData;
	SOCKET  sock;
	struct  sockaddr_in destaddr;
	DWORD  srcaddr;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	//送信socketの準備
	destaddr.sin_family = AF_INET;
	destaddr.sin_port = htons(3702);
	//マルチキャスト通信の準備
	inet_pton(destaddr.sin_family, "239.255.255.250", &destaddr.sin_addr.S_un.S_addr);

	//送信インターフェイスの選択
	inet_pton(AF_INET, nic_ip, &srcaddr);
	setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, (char*)&srcaddr, sizeof(srcaddr));
	//int errornum = WSAGetLastError();

	//queueの初期化
	Clear(*qbuf);

	thread Send_th(&DiscoveryCode::SendThread, this, sock, senddata, destaddr);
	thread Receive_th(&DiscoveryCode::ReceiveThread, this, sock, qbuf);

	//パケットの送信処理
	Send_th.join();
	//パケットの受信処理
	Receive_th.detach();
}

/////////////////////////////////////////////////////////////////////////////
//
//	DiscoveryCodeクラスのデストラクタ処理
//
//	関数名：~DiscoveryCode()
//	引数　：なし
//	戻り値：なし
//			
/////////////////////////////////////////////////////////////////////////////
DiscoveryCode::~DiscoveryCode()
{
	//スレッド終了フラグを有効化
	del_th = true;
}