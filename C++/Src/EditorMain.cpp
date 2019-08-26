#include "EditorMain.h"
#include "NetWork.h"
#include "HomeView.h"
#include "HierarchyView.h"
#include "InspectorView.h"
#include "LogView.h"
#include "MemoryView.h"
#include "GraphicView.h"

EditorMain::EditorMain(LPDIRECT3DDEVICE9 device, HWND hwnd)
{
	mDevice = device;
	mHwnd = hwnd;

	// Init NetWork;
	NetWork::GetInstance(hwnd);

	HomeView::GetInstance();
	HierarchyView::GetInstance();
	InspectorView::GetInstance();
	LogView::GetInstance();
	MemoryView::GetInstance();
	GraphicView::GetInstance();
}

EditorMain::~EditorMain()
{
	NetWork::DeleteInstance();

	HomeView::DeleteInstance();
	HierarchyView::DeleteInstance();
	InspectorView::DeleteInstance();
	LogView::DeleteInstance();
	MemoryView::DeleteInstance();
	GraphicView::DeleteInstance();
}

void EditorMain::Update()
{
	NetWork::GetInstance()->Update();

	RECT rc;
	::GetClientRect(mHwnd, &rc);
	window_width = (float)(rc.right - rc.left);
	window_height = (float)(rc.bottom - rc.top);

	ImGui::PushStyleColor(ImGuiCol_TitleBg, (ImVec4)ImColor(0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, (ImVec4)ImColor(0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, (ImVec4)ImColor(0.2f, 0.2f, 0.2f));

	HomeView::GetInstance()->Update();
	HierarchyView::GetInstance()->Update();
	InspectorView::GetInstance()->Update();
	LogView::GetInstance()->Update();
	MemoryView::GetInstance()->Update();
	GraphicView::GetInstance()->Update();

	ImGui::PopStyleColor(3);
}