#ifndef __NET_WORK_REGISTER_H__
#define __NET_WORK_REGISTER_H__

#include "inc.h"
#include "NetWorkDef.h"

class NetWork;

class NetWorkRegister
{
public:
	void Init();
public:
	typedef void (*CBMsg)(NetWork* net, cJSON* json);
	CBMsg cbMsg[DTool_CTS_Count];
};

#endif
