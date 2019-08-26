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
	Clear();
	mSelect = -1;

	mShowLog = true;
	mShowWarning = true;
	mShowError = true;
}

LogView::~LogView()
{
	Clear();
}

void LogView::Clear()
{
	for (std::vector<LogItem*>::iterator it = mLogs.begin(); it != mLogs.end(); ++it)
	{
		LogItem* item = *it;
		SAFE_DELETE(item);
	}
	mLogs.clear();
}

void LogView::AddLog(const char* msg, const char* stack, LogType type)
{
	LogItem* item = new LogItem();
	item->message = msg;
	item->stack = stack;
	item->type = type;
	mLogs.push_back(item);
	if (mLogs.size() > 120)
	{
		LogItem* first = *(mLogs.begin());
		SAFE_DELETE(first);
		mLogs.erase(mLogs.begin());
	}
}

void LogView::OnShowEnable(const char* showName, bool& bShow)
{
	ImGui::SameLine();
	bool btmp = bShow;
	if (btmp)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, 0xff008800);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0xff008800);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0xff008800);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, 0xff444444);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0xff444444);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0xff444444);
	}
	if (ImGui::Button(showName))
	{
		bShow = !bShow;
		mSelect = -1;
	}
	if (btmp)
	{
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PopStyleColor(3);
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
		Clear();
		mSelect = -1;
	}

	OnShowEnable(STU("显示提醒").c_str(), mShowLog);
	OnShowEnable(STU("显示警告").c_str(), mShowWarning);
	OnShowEnable(STU("显示错误").c_str(), mShowError);

	ImGui::BeginChild("#LogChild");
	int index = 0;
	for (std::vector<LogItem*>::iterator it = mLogs.begin(); it != mLogs.end(); ++it, ++index)
	{
		LogItem* item = *it;
		if (!mShowLog && (item->type == LogType_Log || item->type == LogType_Assert))
			continue;
		if (!mShowWarning && item->type == LogType_Warning)
			continue;
		if (!mShowError && (item->type == LogType_Error || item->type == LogType_Exception))
			continue;

		std::string msg = item->message;
		msg += '\n';

		std::string stackInfo = item->stack;
		stackInfo = stackInfo.substr(0, stackInfo.find('\n'));
		msg += stackInfo;

		if (item->type == LogType_Error)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(1.0f, 0.0f, 0.0f));
		}
		else if (item->type == LogType_Exception)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(1.0f, 0.0f, 0.3f));
		}
		else if (item->type == LogType_Warning)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(1.0f, 1.0f, 0.3f));
		}

		char sprid[64];
		memset(sprid, 0, 64);
		sprintf_s(sprid, "#LOGS%d", index);
		ImGui::PushID(sprid);

		bool bSel = (mSelect == index);
		if (ImGui::Selectable(GbkToUtf8(msg.c_str()).c_str(), &bSel))
			mSelect = index;

		ImGui::PopID();

		if (item->type == LogType_Error
			|| item->type == LogType_Exception
			|| item->type == LogType_Warning)
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
		for (std::vector<LogItem*>::iterator it = mLogs.begin(); it != mLogs.end(); ++it)
		{
			if (index != mSelect)
			{
				index++;
				continue;
			}
			LogItem* item = *it;
			std::string msg = item->message;
			msg += '\n';
			msg += item->stack;
			ImGui::TextWrapped(GbkToUtf8(msg.c_str()).c_str());
			break;
		}
	}

	ImGui::End();
}