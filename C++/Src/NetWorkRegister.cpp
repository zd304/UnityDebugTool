#include "NetWorkRegister.h"
#include "HierarchyView.h"
#include "InspectorView.h"
#include "LogView.h"
#include "MemoryView.h"
#include "GraphicView.h"
#include <fstream>

static void Msg_UpdateHierarchy(NetWork* net, cJSON* json, const char* str)
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

static void Msg_UpdateObject(NetWork* net, cJSON* json, const char* str)
{
	InspectorData* data = new InspectorData();
	data->path = cJSON_GetObjectItem(json, "p")->valuestring;
	data->active = cJSON_GetObjectItem(json, "a")->valueint;
	data->instanceID = cJSON_GetObjectItem(json, "i")->valueint;
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

static void Msg_AddLog(NetWork* net, cJSON* json, const char* str)
{
	const char* msg = cJSON_GetObjectItem(json, "m")->valuestring;
	const char* stack = cJSON_GetObjectItem(json, "s")->valuestring;
	int type = cJSON_GetObjectItem(json, "t")->valueint;

	LogView::GetInstance()->AddLog(msg, stack, (LogType)type);
}

static void Msg_ObjMemory(NetWork* net, cJSON* json, const char* str)
{
	MemoryView::GetInstance()->mLock = true;

	MemoryView::GetInstance()->Clear();

	cJSON* arrayJson = cJSON_GetObjectItem(json, "a");
	if (arrayJson == NULL)
	{
		MemoryView::GetInstance()->mLock = false;
		return;
	}

	MemoryView::GetInstance()->mTotalSize = cJSON_GetObjectItem(json, "s")->valueint;

	int count = cJSON_GetArraySize(arrayJson);
	MemoryView::GetInstance()->mDatas = new MemoryData*[count];
	for (int i = 0; i < count; ++i)
	{
		MemoryView::GetInstance()->mDatas[i] = NULL;
	}

	MemoryView::GetInstance()->mDatasSize = count;
	for (int i = 0; i < count; ++i)
	{
		cJSON* itemJson = cJSON_GetArrayItem(arrayJson, i);
		MemoryObjType moType = (MemoryObjType)cJSON_GetObjectItem(itemJson, "t")->valueint;
		MemoryData* data = NULL;
		switch (moType)
		{
		case MemoryObjType_RenderTexture:
			{
				MemoryData_RenderTexture* sdata = new MemoryData_RenderTexture();
				data = sdata;
				sdata->depth = cJSON_GetObjectItem(itemJson, "d")->valueint;
				sdata->width = cJSON_GetObjectItem(itemJson, "w")->valueint;
				sdata->height = cJSON_GetObjectItem(itemJson, "h")->valueint;
				sdata->format = (RenderTextureFormat)cJSON_GetObjectItem(itemJson, "f")->valueint;
				break;
			}
		case MemoryObjType_Texture2D:
			{
				MemoryData_Texture2D* sdata = new MemoryData_Texture2D();
				data = sdata;
				sdata->mipmapCount = cJSON_GetObjectItem(itemJson, "m")->valueint;
				sdata->width = cJSON_GetObjectItem(itemJson, "w")->valueint;
				sdata->height = cJSON_GetObjectItem(itemJson, "h")->valueint;
				sdata->format = (TextureFormat)cJSON_GetObjectItem(itemJson, "f")->valueint;
				break;
			}
		case MemoryObjType_CubeMap:
			{
				MemoryData_CubeMap* sdata = new MemoryData_CubeMap();
				data = sdata;
				sdata->mipmapCount = cJSON_GetObjectItem(itemJson, "m")->valueint;
				sdata->width = cJSON_GetObjectItem(itemJson, "w")->valueint;
				sdata->height = cJSON_GetObjectItem(itemJson, "h")->valueint;
				sdata->format = (TextureFormat)cJSON_GetObjectItem(itemJson, "f")->valueint;
				break;
			}
		case MemoryObjType_Mesh:
			{
				MemoryData_Mesh* sdata = new MemoryData_Mesh();
				data = sdata;
				sdata->vcount = cJSON_GetObjectItem(itemJson, "v")->valueint;
				sdata->readable = cJSON_GetObjectItem(itemJson, "r")->valueint;
				sdata->subMeshCount = cJSON_GetObjectItem(itemJson, "smc")->valueint;
				cJSON* jsonICounts = cJSON_GetObjectItem(itemJson, "sm");
				sdata->icounts.clear();
				for (int ic = 0; ic < cJSON_GetArraySize(jsonICounts); ++ic)
				{
					cJSON* itemIC = cJSON_GetArrayItem(jsonICounts, ic);
					int icount = itemIC->valueint;
					sdata->icounts.push_back(icount);
				}
				break;
			}
		case MemoryObjType_AnimationClip:
			{
				MemoryData_AnimationClip* sdata = new MemoryData_AnimationClip();
				data = sdata;
				sdata->length = (float)cJSON_GetObjectItem(itemJson, "l")->valuedouble;
				sdata->isLooping = cJSON_GetObjectItem(itemJson, "il")->valueint;
				break;
			}
		}

		if (data != NULL)
		{
			data->name = cJSON_GetObjectItem(itemJson, "n")->valuestring;
			data->size = cJSON_GetObjectItem(itemJson, "sz")->valueint;
			int objType = cJSON_GetObjectItem(itemJson, "t")->valueint;
			data->type = (MemoryObjType)objType;
			MemoryView::GetInstance()->mDatas[i] = data;
		}
	}
	MemoryView::GetInstance()->mLock = false;
}

static void Msg_Memory(NetWork* net, cJSON* json, const char* str)
{
	TotalMemoryInfo& memoryInfo = MemoryView::GetInstance()->mTotalMemoryInfo;

	memoryInfo.totalReservedMemory = cJSON_GetObjectItem(json, "trm")->valuestring;
	memoryInfo.totalAllocatedMemory = cJSON_GetObjectItem(json, "tam")->valuestring;
	memoryInfo.totalUnusedReservedMemory = cJSON_GetObjectItem(json, "turm")->valuestring;
	memoryInfo.monoHeapSize = cJSON_GetObjectItem(json, "mhs")->valuestring;
	memoryInfo.monoUsedSize = cJSON_GetObjectItem(json, "mus")->valuestring;
}

static void Msg_GraphicBase(NetWork* net, cJSON* json, const char* str)
{
	GraphicBasicInfo& graphicBasicInfo = GraphicView::GetInstance()->mBasicInfo;
	graphicBasicInfo.meshFaceCount = cJSON_GetObjectItem(json, "fnum")->valuestring;
	graphicBasicInfo.meshVerticeCount = cJSON_GetObjectItem(json, "vnum")->valuestring;
}

static void Msg_ErrorLog(NetWork* net, cJSON* json, const char* str)
{
	std::string fileName = cJSON_GetObjectItem(json, "f")->valuestring;
	std::string content = cJSON_GetObjectItem(json, "c")->valuestring;

	std::ofstream stream(fileName, std::ios::out | std::ios::binary);
	//if (stream.is_open())
	{
		stream.write(content.c_str(), content.length());
		stream.flush();
		stream.close();
	}
}

void NetWorkRegister::Init()
{
	cbMsg[DTool_CTS_UpdateHierarchy] = Msg_UpdateHierarchy;
	cbMsg[DTool_CTS_UpdateObject] = Msg_UpdateObject;
	cbMsg[DTool_CTS_AddLog] = Msg_AddLog;
	cbMsg[DTool_CTS_ObjMemory] = Msg_ObjMemory;
	cbMsg[DTool_CTS_Memory] = Msg_Memory;
	cbMsg[DTool_CTS_GraphicBase] = Msg_GraphicBase;
	cbMsg[DTool_CTS_ErrorLog] = Msg_ErrorLog;
}