#include "NetWorkRegister.h"
#include "HierarchyView.h"
#include "InspectorView.h"
#include "LogView.h"

static void Msg_UpdateHierarchy(NetWork* net, cJSON* json)
{
	HierarchyView* view = HierarchyView::GetInstance();
	view->mTree.Clear();
	for (int i = 0; i < cJSON_GetArraySize(json); ++i)
	{
		cJSON* rootJson = cJSON_GetArrayItem(json, i);
		HierarchyTreeNode* rootNode = new HierarchyTreeNode(rootJson);
		view->mTree.mRoots.push_back(rootNode);
	}
}

static void Msg_UpdateObject(NetWork* net, cJSON* json)
{
	InspectorData* data = new InspectorData();
	data->path = cJSON_GetObjectItem(json, "p")->valuestring;
	data->active = cJSON_GetObjectItem(json, "a")->valueint;
	data->px = (float)cJSON_GetObjectItem(json, "px")->valuedouble;
	data->py = (float)cJSON_GetObjectItem(json, "py")->valuedouble;
	data->pz = (float)cJSON_GetObjectItem(json, "pz")->valuedouble;
	data->rx = (float)cJSON_GetObjectItem(json, "rx")->valuedouble;
	data->ry = (float)cJSON_GetObjectItem(json, "ry")->valuedouble;
	data->rz = (float)cJSON_GetObjectItem(json, "rz")->valuedouble;
	data->sx = (float)cJSON_GetObjectItem(json, "sx")->valuedouble;
	data->sy = (float)cJSON_GetObjectItem(json, "sy")->valuedouble;
	data->sz = (float)cJSON_GetObjectItem(json, "sz")->valuedouble;

	InspectorView::GetInstance()->SetData(data);
}

static void Msg_AddLog(NetWork* net, cJSON* json)
{
	std::string msg = cJSON_GetObjectItem(json, "m")->valuestring;
	std::string stack = cJSON_GetObjectItem(json, "s")->valuestring;
	int type = cJSON_GetObjectItem(json, "t")->valueint;

	LogView::GetInstance()->AddLog(msg.c_str(), stack.c_str(), (LogType)type);
}

void NetWorkRegister::Init()
{
	cbMsg[DTool_CTS_UpdateHierarchy] = Msg_UpdateHierarchy;
	cbMsg[DTool_CTS_UpdateObject] = Msg_UpdateObject;
	cbMsg[DTool_CTS_AddLog] = Msg_AddLog;
}