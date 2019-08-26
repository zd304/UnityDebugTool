#ifndef __NET_WORK_H__
#define __NET_WORK_H__

#include "inc.h"
#include "NetWorkDef.h"

class NetWorkRegister;

class QMsgData
{
public:
	bool used;
	DTool_CTS cts;
	std::string msg;
};

class NetWork
{
public:
	static NetWork* GetInstance(HWND hwnd = NULL);
	static void DeleteInstance();
	~NetWork();

	void OnMsg(DTool_CTS cts, const std::string& msg);

	void SendToClient(DTool_STC stc, const char* msg);

	void Update();
private:
	NetWork(HWND hwnd);

	void OnReceive(const char* msg, int len);

	static NetWork* mInstance;

	friend DWORD WINAPI NetThreadFunc(LPVOID lpParameter);
public:
	HWND mHwnd;
	SOCKET mSrvSocket;
	SOCKET mClientSocket;
	HANDLE mThreadHandle;
	CRITICAL_SECTION mCriticalSection;
	static const int port = 5304;
	static const int maxBufferSize = 4096000;
	NetWorkRegister* mRegister;

	char* recvBuffer;

	std::string msgText;

	std::list<QMsgData> msgs;
	HANDLE hMutex;
};

#endif