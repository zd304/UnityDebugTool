#ifndef __NET_WORK_H__
#define __NET_WORK_H__

#include "inc.h"
#include "NetWorkDef.h"

class NetWorkRegister;

class NetWork
{
public:
	static NetWork* GetInstance(HWND hwnd = NULL);
	static void DeleteInstance();
	~NetWork();

	void OnMsg(DTool_CTS cts, const std::string& msg);

	void SendToClient(DTool_STC stc, const char* msg);
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
	NetWorkRegister* mRegister;
};

#endif