#include "inc.h"

float window_width = 0.0f;
float window_height = 0.0f;

std::string UTS(const std::string& str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;
}

std::string STU(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴  
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

namespace FormUtility
{
	void FormBegin(const char* spr_id)
	{
		ImGui::Columns(2, spr_id, false);
		ImGui::SetColumnOffset(1, 120.0f);
	}

	void FormLabelText(const char* label, const char* text)
	{
		ImGui::Text(label);
		ImGui::NextColumn();
		
		ImGui::Text(text);
		ImGui::NextColumn();
	}

	void FormVector3(const char* label, float x, float y, float z)
	{
		ImGui::Text(label);
		ImGui::NextColumn();

		float v[3] = {
			x, y, z
		};
		char c[128];
		memset(c, 0, 128);
		sprintf_s(c, "#FV%s", label);
		ImGui::PushID(c);
		ImGui::InputFloat3("", v);
		ImGui::PopID();
		ImGui::NextColumn();
	}

	bool FormCheckBox(const char* label, bool* check)
	{
		ImGui::Text(label);
		ImGui::NextColumn();

		char c[128];
		memset(c, 0, 128);
		sprintf_s(c, "#FCB%s", label);
		ImGui::PushID(c);
		bool rst = ImGui::Checkbox("", check);
		ImGui::PopID();
		ImGui::NextColumn();
		return rst;
	}

	void FormEnd()
	{
		ImGui::Columns();
	}
};