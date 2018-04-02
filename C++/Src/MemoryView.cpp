#include "MemoryView.h"
#include "NetWork.h"

MemoryView* MemoryView::mInstance = NULL;

MemoryView* MemoryView::GetInstance()
{
	if (mInstance == NULL)
		mInstance = new MemoryView();
	return mInstance;
}

void MemoryView::DeleteInstance()
{
	SAFE_DELETE(mInstance);
}

MemoryView::MemoryView()
{
	mDatas = NULL;
	mDatasSize = 0;
	mLock = false;
	mTotalSize = 0;
	mCurSelectType = MemoryObjType_Count;

	mOpen = true;
	mSelect = -1;

	mMemoryTypeNames[MemoryObjType_RenderTexture] = "RenderTexture";
	mMemoryTypeNames[MemoryObjType_Texture2D] = "Texture2D";
	mMemoryTypeNames[MemoryObjType_CubeMap] = "CubeMap";
	mMemoryTypeNames[MemoryObjType_Mesh] = "Mesh";
	mMemoryTypeNames[MemoryObjType_AnimationClip] = "AnimationClip";
}

MemoryView::~MemoryView()
{
	Clear();
}

void MemoryView::Update()
{
	if (editor_mode != EditorMode_Memory)
		return;
	ImGui::SetNextWindowPos(ImVec2(window_width / 3.0f, 0.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(window_width * 2.0f / 3.0f, window_height * 3.0f / 4.0f), ImGuiCond_Always);
	ImGui::Begin("Memory", &mOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	FormUtility::FormBegin("#MemoryView");

	char szTSize[32];
	memset(szTSize, 0, 32);
	sprintf_s(szTSize, "%dm %dk %db", (mTotalSize / 1024) / 1024, (mTotalSize / 1024) % 1024, (mTotalSize % 1024) % 1024);
	FormUtility::FormLabelText(STU("总内存(Byte):").c_str(), szTSize);

	memset(szTSize, 0, 32);
	sprintf_s(szTSize, "%d", mDatasSize);
	FormUtility::FormLabelText(STU("数量(个):").c_str(), szTSize);

	FormUtility::FormEnd();

	if (ImGui::Button(STU("按内存降序排序").c_str()))
	{
		SortByMemorySize(1);
	}
	ImGui::SameLine();
	if (ImGui::Button(STU("按内存升序排序").c_str()))
	{
		SortByMemorySize(2);
	}

	ImGui::Columns(3, "#MemoryViewData");
	ImGui::Separator();
	ImGui::Text(STU("名称").c_str()); ImGui::NextColumn();
	ImGui::Text(STU("类型").c_str()); ImGui::NextColumn();
	ImGui::Text(STU("内存大小(Byte)").c_str()); ImGui::NextColumn();
	ImGui::Separator();

	for (int i = 0; !mLock && i < mDatasSize; ++i)
	{
		MemoryData* data = mDatas[i];
		if (!data) continue;

		char sid[32];
		memset(sid, 0, 32);
		sprintf(sid, "#MVS%04d", i);
		ImGui::PushID(sid);
		if (ImGui::Selectable(data->name.c_str(), mSelect == i, ImGuiSelectableFlags_SpanAllColumns))
			mSelect = i;
		ImGui::PopID();

		ImGui::NextColumn();
		ImGui::Text(mMemoryTypeNames[data->type].c_str()); ImGui::NextColumn();

		char szSize[32];
		memset(szSize, 0, 32);
		sprintf_s(szSize, "%d", data->size);

		std::string ssize = ToCommasNumber(data->size);
		ImGui::Text(ssize.c_str()); ImGui::NextColumn();
	}
	ImGui::Columns();

	ImGui::End();
}

void MemoryView::RequestMemoryObjInfo(MemoryObjType type)
{
	cJSON* json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "t", (int)type);
	char* text = cJSON_Print(json);
	NetWork::GetInstance()->SendToClient(DTool_STC_ReqObjMemory, text);
	cJSON_Delete(json);
}

void MemoryView::Clear()
{
	for (int i = 0; i < mDatasSize; ++i)
	{
		MemoryData* md = mDatas[i];
		SAFE_DELETE(md);
	}
	mTotalSize = 0;
	mLock = false;
	mDatasSize = 0;
	SAFE_DELETE_ARRAY(mDatas);
}

bool SortDescending(MemoryData* a, MemoryData* b)
{
	if (a->size > b->size)
		return true;
	else if (a->size < b->size)
		return false;
	return false;
}

bool SortAscending(MemoryData* a, MemoryData* b)
{
	if (a->size > b->size)
		return false;
	else if (a->size < b->size)
		return true;
	return false;
}

void MemoryView::SortByMemorySize(int t)
{
	std::vector<MemoryData*> v;
	v.resize(mDatasSize);
	for (int i = 0; i < mDatasSize; ++i)
	{
		v[i] = mDatas[i];
	}
	if (t == 1)
		std::sort(v.begin(), v.end(), SortDescending);
	else
		std::sort(v.begin(), v.end(), SortAscending);
	for (int i = 0; i < mDatasSize; ++i)
	{
		mDatas[i] = v[i];
	}
}