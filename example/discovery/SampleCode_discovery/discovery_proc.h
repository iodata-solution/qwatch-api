#include <windows.h>
#include <WS2tcpip.h>
#include <string>
#include <queue>
#include <thread>

using namespace std;

class DiscoveryCode 
{
public:
    void SendThread(SOCKET sock, char* senddata, sockaddr_in destaddr);
	void ReceiveThread(SOCKET sock, queue<string> *qbuf);
	void StartSearch(char *test, queue<string> *qbuf);
	void Clear(queue<string> qbuf);
	~DiscoveryCode();

	BOOL del_th = false;
} ;