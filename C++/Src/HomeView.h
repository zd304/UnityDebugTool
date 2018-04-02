#ifndef __HOME_VIEW_H__
#define __HOME_VIEW_H__

#include "inc.h"

class HomeView
{
public:
	static HomeView* mInstance;

	static HomeView* GetInstance();

	static void DeleteInstance();

	void Update();
private:
	HomeView();

	void OnMenu();

	void OnSceneMode();

	void OnMemoryMode();
public:
	bool mOpen;
};

#endif
