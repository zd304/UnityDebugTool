#include "HomeView.h"
#include "MemoryView.h"

HomeView* HomeView::mInstance = NULL;

HomeView* HomeView::GetInstance()
{
	if (mInstance == NULL)
		mInstance = new HomeView();
	return mInstance;
}

void HomeView::DeleteInstance()
{
	SAFE_DELETE(mInstance);
}

HomeView::HomeView()
{
	mOpen = true;
}

void HomeView::OnMenu()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(STU("分析模式").c_str()))
		{
			bool b = (editor_mode == EditorMode_Scene);
			if (ImGui::MenuItem(STU("场景模式").c_str(), NULL, &b))
			{
				editor_mode = EditorMode_Scene;
			}
			b = (editor_mode == EditorMode_Memory);
			if (ImGui::MenuItem(STU("内存模式").c_str(), NULL, &b))
			{
				editor_mode = EditorMode_Memory;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void HomeView::OnSceneMode()
{
	float itemWidth = 200.0f;
	float itemHeight = 30.0f;
	float indent = window_width / 6.0f - itemWidth * 0.5f - 16.0f;
	ImGui::Indent(indent);

	if (ImGui::Button(STU("刷新场景树").c_str(), ImVec2(itemWidth, itemHeight)))
	{
	}

	ImGui::Unindent(indent);
}

void HomeView::OnMemoryMode()
{
	float itemWidth = 200.0f;
	float itemHeight = 30.0f;
	float indent = window_width / 6.0f - itemWidth * 0.5f - 16.0f;
	ImGui::Indent(indent);

	for (int i = 0; i < MemoryObjType_Count; ++i)
	{
		MemoryObjType ct = (MemoryObjType)i;
		MemoryObjType oldCT = MemoryView::GetInstance()->mCurSelectType;
		if (oldCT == ct)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, 0xff008800);
		}

		if (ImGui::Button(STU(MemoryView::GetInstance()->mMemoryTypeNames[i]).c_str(), ImVec2(itemWidth, itemHeight)))
		{
			MemoryView::GetInstance()->mCurSelectType = ct;
			MemoryView::GetInstance()->RequestMemoryObjInfo(ct);
		}

		if (oldCT == ct)
		{
			ImGui::PopStyleColor();
		}
	}

#if 0
	if (ImGui::Button(STU("RenderTexture").c_str(), ImVec2(itemWidth, itemHeight)))
	{
		MemoryView::GetInstance()->RequestMemoryObjInfo(MemoryObjType_RenderTexture);
	}
	if (ImGui::Button(STU("Texture2D").c_str(), ImVec2(itemWidth, itemHeight)))
	{
		MemoryView::GetInstance()->RequestMemoryObjInfo(MemoryObjType_Texture2D);
	}
	if (ImGui::Button(STU("CubeMap").c_str(), ImVec2(itemWidth, itemHeight)))
	{
		MemoryView::GetInstance()->RequestMemoryObjInfo(MemoryObjType_CubeMap);
	}
	if (ImGui::Button(STU("Mesh").c_str(), ImVec2(itemWidth, itemHeight)))
	{
		MemoryView::GetInstance()->RequestMemoryObjInfo(MemoryObjType_Mesh);
	}
	if (ImGui::Button(STU("AnimationClip").c_str(), ImVec2(itemWidth, itemHeight)))
	{
		MemoryView::GetInstance()->RequestMemoryObjInfo(MemoryObjType_AnimationClip);
	}
#endif

	ImGui::Unindent(indent);
}

void HomeView::Update()
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(window_width / 3.0f, window_height * 3.0f / 4.0f), ImGuiCond_Always);
	ImGui::Begin("Home", &mOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);

	OnMenu();
	
	switch (editor_mode)
	{
	case EditorMode_Scene:
		OnSceneMode();
		break;
	case EditorMode_Memory:
		OnMemoryMode();
		break;
	default:
		break;
	}

	ImGui::End();
}