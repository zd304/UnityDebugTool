#include "EditorMain.h"
#include "NetWork.h"
#include "HierarchyView.h"
#include "InspectorView.h"
#include "LogView.h"

EditorMain::EditorMain(LPDIRECT3DDEVICE9 device, HWND hwnd)
{
	mDevice = device;
	mHwnd = hwnd;

	// Init NetWork;
	NetWork::GetInstance(hwnd);

	HierarchyView::GetInstance();
	InspectorView::GetInstance();
	LogView::GetInstance();
}

EditorMain::~EditorMain()
{
	NetWork::DeleteInstance();

	HierarchyView::DeleteInstance();
	InspectorView::DeleteInstance();
	LogView::DeleteInstance();
}

void EditorMain::Update()
{
	RECT rc;
	::GetClientRect(mHwnd, &rc);
	window_width = (float)(rc.right - rc.left);
	window_height = (float)(rc.bottom - rc.top);

	HierarchyView::GetInstance()->Update();
	InspectorView::GetInstance()->Update();
	LogView::GetInstance()->Update();
}