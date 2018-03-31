#ifndef __INC_H__
#define __INC_H__

#include <d3d9.h>
#include <windows.h>
#include "imgui.h"
#include "cJSON.h"
#include <iostream>
//#include <winsock2.h>
#include <string>
#include <vector>
#include <queue>
#include <list>
#pragma comment(lib, "ws2_32.lib")

#define SAFE_DELETE(x) if (x) { delete x; x = NULL; }
#define SAFE_DELETE_ARRAY(x) if (x) { delete [] x; x = NULL; }

std::string UTS(const std::string & str);

std::string STU(const std::string & str);

extern float window_width;
extern float window_height;

namespace FormUtility
{
	void FormBegin(const char* spr_id);

	void FormLabelText(const char* label, const char* text);

	void FormVector3(const char* label, float x, float y, float z);

	bool FormCheckBox(const char* label, bool* check);

	void FormEnd();
};

#endif