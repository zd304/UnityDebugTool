#ifndef __LOG_VIEW_H__
#define __LOG_VIEW_H__

#include "inc.h"

enum LogType
{
	LogType_Error,
	LogType_Assert,
	LogType_Warning,
	LogType_Log,
	LogType_Exception
};

struct LogItem
{
	std::string message;
	std::string stack;
	LogType type;
};

class LogView
{
public:
	static LogView* GetInstance();

	static void DeleteInstance();

	void Update();

	void AddLog(const char* msg, const char* stack, LogType type);
private:
	LogView();
	~LogView();
private:
	static LogView* mInstance;
	bool mOpen;
	std::list<LogItem> mLogs;
	int mSelect;

	bool mShowLog;
	bool mShowWarning;
	bool mShowError;
};

#endif
