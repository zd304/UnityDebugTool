#include "NetWork.h"
#include "NetWorkRegister.h"

DWORD WINAPI NetThreadFunc(LPVOID lpParameter);

NetWork* NetWork::mInstance = NULL;

static bool g_ThreadRunning = false;

NetWork* NetWork::GetInstance(HWND hwnd)
{
	if (hwnd == NULL)
		return mInstance;
	if (mInstance == NULL)
		mInstance = new NetWork(hwnd);
	return mInstance;
}

void NetWork::DeleteInstance()
{
	SAFE_DELETE(mInstance);
}

NetWork::NetWork(HWND hwnd)
	: mHwnd(hwnd), hMutex(NULL)
{
	recvBuffer = new char[maxBufferSize];

	mClientSocket = 0;

	mRegister = new NetWorkRegister();
	mRegister->Init();

	WSAData wsadata;
	int ret;

	ret = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (ret != 0)
	{
		std::string msg = "WSAStart up is failed with error\t" + ret;
		MessageBoxA(mHwnd, msg.c_str(), "Net Error", MB_OK);
		return;
	}

	mSrvSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == mSrvSocket)
	{
		std::string msg = "socket is failed with error: " + WSAGetLastError();
		MessageBoxA(mHwnd, msg.c_str(), "Net Error", MB_OK);
		WSACleanup();
		return;
	}

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(mSrvSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if (SOCKET_ERROR == ret)
	{
		std::string msg = "bind is failed with error: " + WSAGetLastError();
		MessageBoxA(mHwnd, msg.c_str(), "Net Error", MB_OK);
		closesocket(mSrvSocket);
		WSACleanup();
		return;
	}

	ret = listen(mSrvSocket, 5);
	if (SOCKET_ERROR == ret)
	{
		std::string msg = "listen is failed with error: " + WSAGetLastError();
		MessageBoxA(mHwnd, msg.c_str(), "Net Error", MB_OK);
		closesocket(mSrvSocket);
		WSACleanup();
		return;
	}

	hMutex = CreateMutex(NULL, FALSE, NULL);
	InitializeCriticalSection(&mCriticalSection);

	g_ThreadRunning = true;
	mThreadHandle = CreateThread(NULL, 0, NetThreadFunc, NULL, 0, NULL);
}

DWORD WINAPI NetThreadFunc(LPVOID lpParameter)
{
	NetWork* netWork = NetWork::GetInstance();
	if (netWork == NULL)
		return 0;

	sockaddr_in clientAddr;
	int clientAddrLen = sizeof(sockaddr_in);
	SOCKET srvSocket = netWork->mSrvSocket;
	SOCKET clientSocket = accept(srvSocket, (sockaddr*)&clientAddr, &clientAddrLen);
	if (INVALID_SOCKET == clientSocket)
	{
		std::string msg = "accept is failed with error: " + WSAGetLastError();
		MessageBoxA(netWork->mHwnd, msg.c_str(), "Net Error", MB_OK);
		closesocket(srvSocket);
		WSACleanup();
		return 0;
	}

	netWork->mClientSocket = clientSocket;

	while (1)
	{
		if (!g_ThreadRunning)
			return 0;

		EnterCriticalSection(&netWork->mCriticalSection);

		memset(netWork->recvBuffer, 0, netWork->maxBufferSize);
		int ret = recv(clientSocket, netWork->recvBuffer, netWork->maxBufferSize, 0);
		if (SOCKET_ERROR == ret || ret == 0)
		{
 			closesocket(clientSocket);
			netWork->mClientSocket = 0;
			clientSocket = accept(srvSocket, (sockaddr*)&clientAddr, &clientAddrLen);
			if (INVALID_SOCKET == clientSocket)
			{
				std::string msg = "accept is failed with error: " + WSAGetLastError();
				MessageBoxA(netWork->mHwnd, msg.c_str(), "Net Error", MB_OK);
				closesocket(srvSocket);
				WSACleanup();
				return 0;
			}
			netWork->mClientSocket = clientSocket;
		}

		//netWork->recvBuffer[ret] = '\0';
		netWork->OnReceive(netWork->recvBuffer, ret);

		LeaveCriticalSection(&netWork->mCriticalSection);
	}

	WSACleanup();
}

NetWork::~NetWork()
{
	SAFE_DELETE_ARRAY(recvBuffer);

	SAFE_DELETE(mRegister);

	g_ThreadRunning = false;
	CloseHandle(mThreadHandle);
	mThreadHandle = 0;

	DeleteCriticalSection(&mCriticalSection);

	closesocket(mClientSocket);
	closesocket(mSrvSocket);

	WSACleanup();
}

struct CTSType
{
	union
	{
		int i;
		char s[4];
	};
};

void NetWork::OnReceive(const char* msg, int len)
{
	if (len < 4)
		return;
	CTSType ih;
	memcpy(ih.s, msg, 4);
	char* cMsg = new char[len+1];
	memset(cMsg, 0, len+1);
	memcpy(cMsg, msg + 4, len - 4);
	std::string sMsg = Utf8ToGbk(cMsg);

	WaitForSingleObject(hMutex, INFINITE);
	if (msgs.size() > 0)
	{
		QMsgData* data = &(*msgs.begin());
		while (msgs.size() > 0 && data->used)
		{
			std::list<QMsgData>::iterator it = msgs.begin();
			std::list<QMsgData>::iterator next = it;
			++next;
			msgs.erase(it);
			if (msgs.size() > 0)
			{
				data = &(*next);
			}
		}
	}

	QMsgData qmsgData;
	qmsgData.used = false;
	qmsgData.cts = (DTool_CTS)ih.i;
	qmsgData.msg = sMsg;
	msgs.push_back(qmsgData);

	ReleaseMutex(hMutex);	
}

void NetWork::Update()
{
	WaitForSingleObject(hMutex, INFINITE);

	std::list<QMsgData>::iterator it = msgs.begin();
	while (it != msgs.end())
	{
		QMsgData& data = *it;

		if (!data.used)
		{
			OnMsg(data.cts, data.msg);
			data.used = true;
		}
		
		++it;
	}

	ReleaseMutex(hMutex);
}

void NetWork::OnMsg(DTool_CTS cts, const std::string& msg)
{
	if (cts > DTool_CTS_Count || cts < 0)
		return;

	NetWorkRegister::CBMsg cb = mRegister->cbMsg[cts];
	if (cb != NULL)
	{
		cJSON* root = cJSON_Parse(msg.c_str());
		if (root)
		{
			cb(this, root);
		}
	}
}

void NetWork::SendToClient(DTool_STC stc, const char* msg)
{
	if (mClientSocket == 0)
	{
		return;
	}

	std::string sMsg = msg;
	int len = sMsg.length() + 4;
	char* c = new char[len];
	memset(c, 0, len);
	memcpy(c, &stc, 4);
	memcpy(c + 4, sMsg.c_str(), sMsg.length());
	int ret = send(mClientSocket, c, len, 0);
	SAFE_DELETE_ARRAY(c);
	if (SOCKET_ERROR == ret)
	{
		closesocket(mClientSocket);
		WSACleanup();
		return;
	}
}