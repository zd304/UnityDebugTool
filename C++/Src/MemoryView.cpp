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

	mRenderTextureFormatNames[RenderTextureFormat_ARGB32		] = "ARGB32";
	mRenderTextureFormatNames[RenderTextureFormat_Depth			] = "Depth";
	mRenderTextureFormatNames[RenderTextureFormat_ARGBHalf		] = "ARGBHalf";
	mRenderTextureFormatNames[RenderTextureFormat_Shadowmap		] = "Shadowmap";
	mRenderTextureFormatNames[RenderTextureFormat_RGB565		] = "RGB565";
	mRenderTextureFormatNames[RenderTextureFormat_ARGB4444		] = "ARGB4444";
	mRenderTextureFormatNames[RenderTextureFormat_ARGB1555		] = "ARGB1555";
	mRenderTextureFormatNames[RenderTextureFormat_Default		] = "Default";
	mRenderTextureFormatNames[RenderTextureFormat_ARGB2101010	] = "ARGB2101010";
	mRenderTextureFormatNames[RenderTextureFormat_DefaultHDR	] = "DefaultHDR";
	mRenderTextureFormatNames[RenderTextureFormat_ARGB64		] = "ARGB64";
	mRenderTextureFormatNames[RenderTextureFormat_ARGBFloat		] = "ARGBFloat";
	mRenderTextureFormatNames[RenderTextureFormat_RGFloat		] = "RGFloat";
	mRenderTextureFormatNames[RenderTextureFormat_RGHalf		] = "RGHalf";
	mRenderTextureFormatNames[RenderTextureFormat_RFloat		] = "RFloat";
	mRenderTextureFormatNames[RenderTextureFormat_RHalf			] = "RHalf";
	mRenderTextureFormatNames[RenderTextureFormat_R8			] = "R8";
	mRenderTextureFormatNames[RenderTextureFormat_ARGBInt		] = "ARGBInt";
	mRenderTextureFormatNames[RenderTextureFormat_RGInt			] = "RGInt";
	mRenderTextureFormatNames[RenderTextureFormat_RInt			] = "RInt";
	mRenderTextureFormatNames[RenderTextureFormat_BGRA32		] = "BGRA32";
	mRenderTextureFormatNames[RenderTextureFormat_RGB111110Float] = "RGB111110Float";
	mRenderTextureFormatNames[RenderTextureFormat_RG32			] = "RG32";
	mRenderTextureFormatNames[RenderTextureFormat_RGBAUShort	] = "RGBAUShort";
	mRenderTextureFormatNames[RenderTextureFormat_RG16			] = "RG16";

	mTextureFormatNames[TextureFormat_Alpha8			] = "Alpha8";
	mTextureFormatNames[TextureFormat_ARGB4444			] = "ARGB4444";
	mTextureFormatNames[TextureFormat_RGB24				] = "RGB24";
	mTextureFormatNames[TextureFormat_RGBA32			] = "RGBA32";
	mTextureFormatNames[TextureFormat_ARGB32			] = "ARGB32";
	mTextureFormatNames[TextureFormat_RGB565			] = "RGB565";
	mTextureFormatNames[TextureFormat_R16				] = "R16";
	mTextureFormatNames[TextureFormat_DXT1				] = "DXT1";
	mTextureFormatNames[TextureFormat_DXT5				] = "DXT5";
	mTextureFormatNames[TextureFormat_RGBA4444			] = "RGBA4444";
	mTextureFormatNames[TextureFormat_BGRA32			] = "BGRA32";
	mTextureFormatNames[TextureFormat_RHalf				] = "RHalf";
	mTextureFormatNames[TextureFormat_RGHalf			] = "RGHalf";
	mTextureFormatNames[TextureFormat_RGBAHalf			] = "RGBAHalf";
	mTextureFormatNames[TextureFormat_RFloat			] = "RFloat";
	mTextureFormatNames[TextureFormat_RGFloat			] = "RGFloat";
	mTextureFormatNames[TextureFormat_RGBAFloat			] = "RGBAFloat";
	mTextureFormatNames[TextureFormat_YUY2				] = "YUY2";
	mTextureFormatNames[TextureFormat_RGB9e5Float		] = "RGB9e5Float";
	mTextureFormatNames[TextureFormat_BC4				] = "BC4";
	mTextureFormatNames[TextureFormat_BC5				] = "BC5";
	mTextureFormatNames[TextureFormat_BC6H				] = "BC6H";
	mTextureFormatNames[TextureFormat_BC7				] = "BC7";
	mTextureFormatNames[TextureFormat_DXT1Crunched		] = "DXT1Crunched";
	mTextureFormatNames[TextureFormat_DXT5Crunched		] = "DXT5Crunched";
	mTextureFormatNames[TextureFormat_PVRTC_RGB2		] = "PVRTC_RGB2";
	mTextureFormatNames[TextureFormat_PVRTC_RGBA2		] = "PVRTC_RGBA2";
	mTextureFormatNames[TextureFormat_PVRTC_RGB4		] = "PVRTC_RGB4";
	mTextureFormatNames[TextureFormat_PVRTC_RGBA4		] = "PVRTC_RGBA4";
	mTextureFormatNames[TextureFormat_ETC_RGB4			] = "ETC_RGB4";
	mTextureFormatNames[TextureFormat_ATC_RGB4			] = "ATC_RGB4";
	mTextureFormatNames[TextureFormat_ATC_RGBA8			] = "ATC_RGBA8";
	mTextureFormatNames[TextureFormat_EAC_R				] = "EAC_R";
	mTextureFormatNames[TextureFormat_EAC_R_SIGNED		] = "EAC_R_SIGNED";
	mTextureFormatNames[TextureFormat_EAC_RG			] = "EAC_RG";
	mTextureFormatNames[TextureFormat_EAC_RG_SIGNED		] = "EAC_RG_SIGNED";
	mTextureFormatNames[TextureFormat_ETC2_RGB			] = "ETC2_RGB";
	mTextureFormatNames[TextureFormat_ETC2_RGBA1		] = "ETC2_RGBA1";
	mTextureFormatNames[TextureFormat_ETC2_RGBA8		] = "ETC2_RGBA8";
	mTextureFormatNames[TextureFormat_ASTC_RGB_4x4		] = "ASTC_RGB_4x4";
	mTextureFormatNames[TextureFormat_ASTC_RGB_5x5		] = "ASTC_RGB_5x5";
	mTextureFormatNames[TextureFormat_ASTC_RGB_6x6		] = "ASTC_RGB_6x6";
	mTextureFormatNames[TextureFormat_ASTC_RGB_8x8		] = "ASTC_RGB_8x8";
	mTextureFormatNames[TextureFormat_ASTC_RGB_10x10	] = "ASTC_RGB_10x10";
	mTextureFormatNames[TextureFormat_ASTC_RGB_12x12	] = "ASTC_RGB_12x12";
	mTextureFormatNames[TextureFormat_ASTC_RGBA_4x4		] = "ASTC_RGBA_4x4";
	mTextureFormatNames[TextureFormat_ASTC_RGBA_5x5		] = "ASTC_RGBA_5x5";
	mTextureFormatNames[TextureFormat_ASTC_RGBA_6x6		] = "ASTC_RGBA_6x6";
	mTextureFormatNames[TextureFormat_ASTC_RGBA_8x8		] = "ASTC_RGBA_8x8";
	mTextureFormatNames[TextureFormat_ASTC_RGBA_10x10	] = "ASTC_RGBA_10x10";
	mTextureFormatNames[TextureFormat_ASTC_RGBA_12x12	] = "ASTC_RGBA_12x12";
	mTextureFormatNames[TextureFormat_ETC_RGB4_3DS		] = "ETC_RGB4_3DS";
	mTextureFormatNames[TextureFormat_ETC_RGBA8_3DS		] = "ETC_RGBA8_3DS";
	mTextureFormatNames[TextureFormat_RG16				] = "RG16";
	mTextureFormatNames[TextureFormat_R8				] = "R8";
}

MemoryView::~MemoryView()
{
	Clear();
}

void MemoryView::UpdateObjectsView()
{
	ImGui::SetNextWindowPos(ImVec2(window_width / 3.0f, 0.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(window_width * 2.0f / 3.0f, window_height * 3.0f / 4.0f), ImGuiCond_Always);
	ImGui::Begin("Objects", &mOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	FormUtility::FormBegin("#MemoryObjectsView");

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

void MemoryView::UpdateDetailView()
{
	ImGui::SetNextWindowPos(ImVec2(window_width / 3.0f, window_height * 3.0f / 4.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(window_width * 2.0f / 3.0f, window_height / 4.0f), ImGuiCond_Always);
	ImGui::Begin("Detail", &mOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	if (mSelect >= 0 && mSelect < (size_t)mDatasSize && !mLock)
	{
		MemoryData* data = mDatas[mSelect];
		if (data)
		{
			FormUtility::FormBegin("#DetailView", 300.0f);

			FormUtility::FormLabelText(STU("名称").c_str(), data->name.c_str());
			std::string sSize = ToCommasNumber(data->size);
			FormUtility::FormLabelText(STU("内存大小(Byte)").c_str(), sSize.c_str());
			FormUtility::FormLabelText(STU("物体类型").c_str(), mMemoryTypeNames[data->type].c_str());

			switch (data->type)
			{
			case MemoryObjType_RenderTexture:
				{
					MemoryData_RenderTexture* sdata = (MemoryData_RenderTexture*)data;
					FormUtility::FormLabelText(STU("宽度").c_str(), ToString(sdata->width).c_str());
					FormUtility::FormLabelText(STU("高度").c_str(), ToString(sdata->height).c_str());
					FormUtility::FormLabelText(STU("深度").c_str(), ToString(sdata->depth).c_str());
					FormUtility::FormLabelText(STU("格式").c_str(), mRenderTextureFormatNames[sdata->format].c_str());
				}
				break;
			case MemoryObjType_Texture2D:
				{
					MemoryData_Texture2D* sdata = (MemoryData_Texture2D*)data;
					FormUtility::FormLabelText(STU("宽度").c_str(), ToString(sdata->width).c_str());
					FormUtility::FormLabelText(STU("高度").c_str(), ToString(sdata->height).c_str());
					FormUtility::FormLabelText(STU("Mipmap数量").c_str(), ToString(sdata->mipmapCount).c_str());
					FormUtility::FormLabelText(STU("格式").c_str(), mTextureFormatNames[sdata->format].c_str());
				}
				break;
			case MemoryObjType_CubeMap:
				{
					MemoryData_CubeMap* sdata = (MemoryData_CubeMap*)data;
					FormUtility::FormLabelText(STU("宽度").c_str(), ToString(sdata->width).c_str());
					FormUtility::FormLabelText(STU("高度").c_str(), ToString(sdata->height).c_str());
					FormUtility::FormLabelText(STU("Mipmap数量").c_str(), ToString(sdata->mipmapCount).c_str());
					FormUtility::FormLabelText(STU("格式").c_str(), mTextureFormatNames[sdata->format].c_str());
				}
				break;
			case MemoryObjType_Mesh:
				{
					MemoryData_Mesh* sdata = (MemoryData_Mesh*)data;
					FormUtility::FormLabelText(STU("Read/Write Enable").c_str(), sdata->readable ? STU("开启").c_str() : STU("关闭").c_str());
					FormUtility::FormLabelText(STU("顶点数量").c_str(), ToString(sdata->vcount).c_str());
					FormUtility::FormLabelText(STU("子模型数量").c_str(), ToString(sdata->subMeshCount).c_str());

					ImGui::Indent(16.0f);
					for (size_t i = 0; i < sdata->icounts.size(); ++i)
					{
						int icount = sdata->icounts[i];

						char title[32];
						memset(title, 0, 32);
						sprintf_s(title, "子模型[%d]面数", i);
						FormUtility::FormLabelText(STU(title).c_str(), ToString(icount / 3).c_str());
					}
					ImGui::Unindent(16.0f);
				}
				break;
			case MemoryObjType_AnimationClip:
				{
					MemoryData_AnimationClip* sdata = (MemoryData_AnimationClip*)data;
					FormUtility::FormLabelText(STU("动画长度(秒)").c_str(), ToString(sdata->length).c_str());
					FormUtility::FormLabelText(STU("是否循环").c_str(), sdata->isLooping ? STU("是").c_str() : STU("否").c_str());
				}
				break;
			default:
				break;
			}

			FormUtility::FormEnd();
		}
	}

	ImGui::End();
}

void MemoryView::UpdateMemoryView()
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, window_height * 3.0f / 4.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(window_width / 3.0f, window_height / 4.0f), ImGuiCond_Always);
	ImGui::Begin("Memory", &mOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	FormUtility::FormBegin("#MemoryView", 200.0f);

	FormUtility::FormLabelText(STU("总堆内存").c_str(), mTotalMemoryInfo.totalReservedMemory.c_str());
	FormUtility::FormLabelText(STU("已占用内存").c_str(), mTotalMemoryInfo.totalAllocatedMemory.c_str());
	FormUtility::FormLabelText(STU("空闲中内存").c_str(), mTotalMemoryInfo.totalUnusedReservedMemory.c_str());
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	FormUtility::FormLabelText(STU("总Mono堆内存").c_str(), mTotalMemoryInfo.monoHeapSize.c_str());
	FormUtility::FormLabelText(STU("已占用Mono堆内存").c_str(), mTotalMemoryInfo.monoUsedSize.c_str());

	FormUtility::FormEnd();

	ImGui::End();
}

void MemoryView::Update()
{
	if (editor_mode != EditorMode_Memory)
		return;
	UpdateMemoryView();

	UpdateObjectsView();

	UpdateDetailView();
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