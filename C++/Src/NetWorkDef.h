#ifndef _NET_WORK_DEF_H__
#define _NET_WORK_DEF_H__

enum DTool_STC
{
	DTool_STC_None					= 0,
	DTool_STC_ReqObject				= 1,
	DTool_STC_ReqActive				= 2,
	DTool_STC_ReqObjMemory			= 3,
	DTool_STC_ReqMemory				= 4,

	DTool_STC_Count
};

enum DTool_CTS
{
	DTool_CTS_None					= 0,
	DTool_CTS_UpdateHierarchy		= 1,
	DTool_CTS_UpdateObject			= 2,
	DTool_CTS_AddLog				= 3,
	DTool_CTS_ObjMemory				= 4,
	DTool_CTS_Memory				= 5,

	DTool_CTS_Count
};

#endif // !_NET_WORK_DEF_H__
