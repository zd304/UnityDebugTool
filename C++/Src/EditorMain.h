#ifndef __EDITOR_MAIN_H__
#define __EDITOR_MAIN_H__

#include "inc.h"

class HierarchyView;

class EditorMain
{
public:
	EditorMain(LPDIRECT3DDEVICE9 device, HWND hwnd);
	~EditorMain();
	void Update();
public:
	LPDIRECT3DDEVICE9 mDevice;
	HWND mHwnd;
private:
	HierarchyView* mHierarchyView;
};

#endif