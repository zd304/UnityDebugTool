#include "InspectorView.h"
#include "NetWork.h"
#include "HierarchyTree.h"
#include "HierarchyView.h"

InspectorView* InspectorView::mInstance = NULL;

InspectorView* InspectorView::GetInstance()
{
	if (mInstance == NULL)
		mInstance = new InspectorView();
	return mInstance;
}

void InspectorView::DeleteInstance()
{
	SAFE_DELETE(mInstance);
}

InspectorView::InspectorView()
{
	mData = NULL;
	mOpen = true;
}

InspectorView::~InspectorView()
{
	SAFE_DELETE(mData);
}

void InspectorView::Update()
{
	ImGui::SetNextWindowPos(ImVec2(window_width / 3.0f, 0.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(window_width / 3.0f, window_height * 3.0f / 4.0f), ImGuiCond_Always);
	ImGui::Begin("Inspector", &mOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	if (mData)
	{
		FormUtility::FormBegin("#InspectorView");
		FormUtility::FormLabelText(STU("����·��").c_str(), mData->path.c_str());
		bool bActive = mData->active > 0;
		if (FormUtility::FormCheckBox(STU("����״̬").c_str(), &bActive))
		{
			cJSON* json = cJSON_CreateObject();
			cJSON_AddStringToObject(json, "p", mData->path.c_str());
			mData->active = bActive ? 1 : 0;
			cJSON_AddNumberToObject(json, "a", mData->active = bActive);
			char* text = cJSON_Print(json);
			NetWork::GetInstance()->SendToClient(DTool_STC_ReqActive, text);
			cJSON_Delete(json);

			HierarchyTree& tree = HierarchyView::GetInstance()->mTree;
			HierarchyTreeNode* pNode = tree.GetNode(mData->path);
			if (pNode)
			{
				pNode->mActive = bActive;
			}
		}
		FormUtility::FormVector3(STU("λ��").c_str(), mData->px, mData->py, mData->pz);
		FormUtility::FormVector3(STU("��ת").c_str(), mData->rx, mData->ry, mData->rz);
		FormUtility::FormVector3(STU("����").c_str(), mData->sx, mData->sy, mData->sz);
		FormUtility::FormEnd();
	}

	ImGui::End();
}

void InspectorView::SetData(InspectorData* data)
{
	SAFE_DELETE(mData);
	mData = data;
}