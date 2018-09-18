#include "HierarchyView.h"
#include "NetWork.h"

HierarchyView* HierarchyView::mInstance = NULL;

HierarchyView* HierarchyView::GetInstance()
{
	if (mInstance == NULL)
		mInstance = new HierarchyView();
	return mInstance;
}

void HierarchyView::DeleteInstance()
{
	SAFE_DELETE(mInstance);
}

HierarchyView::HierarchyView()
{
	mOpen = true;
}

void HierarchyView::Update()
{
	if (editor_mode != EditorMode_Scene)
		return;
	ImGui::SetNextWindowPos(ImVec2(window_width / 3.0f, 0.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(window_width / 3.0f, window_height * 3.0f / 4.0f), ImGuiCond_Always);
	ImGui::Begin("Hierarchy", &mOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	for (size_t i = 0; i < mTree.mRoots.size(); ++i)
	{
		NodeGUI(mTree.mRoots[i]);
	}
	ImGui::End();
}

void HierarchyView::NodeGUI(HierarchyTreeNode* node)
{
	char sprID[64];
	
	ImGui::Indent((node->mLayer + 1) * 12.0f);
	if (node->mChildren.size() <= 0)
	{
		node->mFoldout = true;
		ImGui::Bullet();
	}
	else
	{
		memset(sprID, 0, 64);
		sprintf_s(sprID, "#T%d", node->mID);

		ImGui::PushID(sprID);
		ImGui::Checkbox("", &node->mFoldout);
		ImGui::PopID();
	}
	ImGui::SameLine();

	memset(sprID, 0, 64);
	sprintf_s(sprID, "#S%d", node->mID);
	ImGui::PushID(sprID);
	bool bSel = (mSelect == node->mID);

	if (!node->mActive)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(0.5f, 0.5, 0.5f));
	}
	if (ImGui::Selectable(node->name.c_str(), &bSel))
	{
		if (mSelect != node->mID)
		{
			cJSON* json = cJSON_CreateObject();
			std::string path = "";
			mTree.GetPath(node, path);
			cJSON_AddStringToObject(json, "p", path.c_str());
			cJSON_AddNumberToObject(json, "i", node->mInstanceID);
			char* text = cJSON_Print(json);
			NetWork::GetInstance()->SendToClient(DTool_STC_ReqObject, text);
			cJSON_Delete(json);
		}
		mSelect = node->mID;
	}
	if (!node->mActive)
	{
		ImGui::PopStyleColor(1);
	}
	ImGui::PopID();
	ImGui::Unindent((node->mLayer + 1) * 12.0f);

	if (!node->mFoldout)
		return;
	for (size_t i = 0; i < node->mChildren.size(); ++i)
	{
		HierarchyTreeNode* child = node->mChildren[i];
		NodeGUI(child);
	}
}