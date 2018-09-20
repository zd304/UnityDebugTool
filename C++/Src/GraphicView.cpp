#include "GraphicView.h"
#include "NetWork.h"

GraphicView* GraphicView::mInstance = NULL;

GraphicView* GraphicView::GetInstance()
{
	if (mInstance == NULL)
		mInstance = new GraphicView();
	return mInstance;
}

void GraphicView::DeleteInstance()
{
	SAFE_DELETE(mInstance);
}

GraphicView::GraphicView()
{
	mOpen = true;
}

GraphicView::~GraphicView()
{

}

void GraphicView::Update()
{
	if (editor_mode != EditorMode_Graphic)
		return;

	UpdateBasicInfo();
}

void GraphicView::UpdateBasicInfo()
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, window_height * 3.0f / 4.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(window_width / 3.0f, window_height / 4.0f), ImGuiCond_Always);
	ImGui::Begin("Memory", &mOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	FormUtility::FormBegin("#BasicInfo", 200.0f);

	FormUtility::FormLabelText(STU("Read/Write Enable的所有模型").c_str(), "");
	FormUtility::FormLabelText(STU("总面数").c_str(), mBasicInfo.meshFaceCount.c_str());
	FormUtility::FormLabelText(STU("总顶点数").c_str(), mBasicInfo.meshVerticeCount.c_str());

	FormUtility::FormEnd();

	ImGui::End();
}