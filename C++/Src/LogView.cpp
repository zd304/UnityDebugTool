#include "LogView.h"

LogView* LogView::mInstance = NULL;

LogView* LogView::GetInstance()
{
	if (mInstance == NULL)
		mInstance = new LogView();
	return mInstance;
}

void LogView::DeleteInstance()
{
	SAFE_DELETE(mInstance);
}

LogView::LogView()
{
	mOpen = true;
	mLogs.clear();
	mSelect = -1;

	mShowLog = true;
	mShowWarning = true;
	mShowError = true;
}

LogView::~LogView()
{

}

void LogView::AddLog(const char* msg, const char* stack, LogType type)
{
	LogItem item;
	item.message = msg;
	item.stack = stack;
	item.type = type;
	mLogs.push_back(item);
	if (mLogs.size() > 120)
	{
		mLogs.erase(mLogs.begin());
	}
}

void LogView::Update()
{
	if (editor_mode != EditorMode_Scene)
		return;
	ImGui::SetNextWindowPos(ImVec2(0.0f, window_height * 3.0f / 4.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(window_width * 2.0f / 3.0f, window_height / 4.0f), ImGuiCond_Always);
	ImGui::Begin("Log", &mOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	if (ImGui::Button(STU("清空").c_str()))
	{
		mLogs.clear();
		mSelect = -1;
	}
	bool btmp = false;
	//////////////////////////////////////////////////////////////////////////
	ImGui::SameLine();
	btmp = mShowLog;
	if (btmp)
		ImGui::PushStyleColor(ImGuiCol_Button, 0xff008800);
	else
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0xff444444);
	if (ImGui::Button(STU("显示消息").c_str()))
	{
		mShowLog = !mShowLog;
	}
	ImGui::PopStyleColor();
	//////////////////////////////////////////////////////////////////////////
	ImGui::SameLine();
	btmp = mShowWarning;
	if (btmp)
		ImGui::PushStyleColor(ImGuiCol_Button, 0xff008800);
	else
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0xff444444);
	if (ImGui::Button(STU("显示提醒").c_str()))
	{
		mShowWarning = !mShowWarning;
	}
	ImGui::PopStyleColor();
	//////////////////////////////////////////////////////////////////////////
	ImGui::SameLine();
	btmp = mShowError;
	if (btmp)
		ImGui::PushStyleColor(ImGuiCol_Button, 0xff008800);
	else
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0xff444444);
	if (ImGui::Button(STU("显示错误").c_str()))
	{
		mShowError = !mShowError;
	}
	ImGui::PopStyleColor();
	//////////////////////////////////////////////////////////////////////////

	ImGui::BeginChild("#LogChild");
	int index = 0;
	for (std::list<LogItem>::iterator it = mLogs.begin(); it != mLogs.end(); ++it)
	{
		LogItem& item = *it;
		if (!mShowLog && (item.type == LogType_Log || item.type == LogType_Assert))
			continue;
		if (!mShowWarning && item.type == LogType_Warning)
			continue;
		if (!mShowError && (item.type == LogType_Error || item.type == LogType_Exception))
			continue;

		std::string msg = item.message;
		msg += '\n';

		std::string stackInfo = item.stack;
		stackInfo = stackInfo.substr(0, stackInfo.find('\n'));
		msg += stackInfo;

		if (item.type == LogType_Error)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(1.0f, 0.0f, 0.0f));
		}
		else if (item.type == LogType_Exception)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(1.0f, 0.0f, 0.3f));
		}
		else if (item.type == LogType_Warning)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(1.0f, 1.0f, 0.3f));
		}

		char sprid[64];
		memset(sprid, 0, 64);
		sprintf_s(sprid, "#LOGS%d", index);
		ImGui::PushID(sprid);

		bool bSel = (mSelect == index);
		if (ImGui::Selectable(msg.c_str(), &bSel))
			mSelect = index;
		++index;

		ImGui::PopID();

		if (item.type == LogType_Error
			|| item.type == LogType_Exception
			|| item.type == LogType_Warning)
		{
			ImGui::PopStyleColor(1);
		}
	}
	ImGui::EndChild();

	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(window_width * 2.0f / 3.0f, window_height * 3.0f / 4.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(window_width / 3.0f, window_height / 4.0f), ImGuiCond_Always);
	ImGui::Begin("LogDetail", &mOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	if (mSelect >= 0)
	{
		index = 0;
		std::list<LogItem>::iterator it;
		for (it = mLogs.begin(); it != mLogs.end(); ++it)
		{
			if (index != mSelect)
			{
				index++;
				continue;
			}
			LogItem& item = *it;
			std::string msg = item.message;
			msg += '\n';
			msg += item.stack;
			ImGui::Text(STU(msg).c_str());
			break;
		}
	}

	ImGui::End();
}