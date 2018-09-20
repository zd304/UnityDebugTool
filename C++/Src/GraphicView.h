#ifndef __GRAPHIC_VIEW_H__
#define __GRAPHIC_VIEW_H__

#include "GraphicData.h"

class GraphicView
{
public:
	static GraphicView* mInstance;

	static GraphicView* GetInstance();

	static void DeleteInstance();

	void Update();
private:
	GraphicView();

	~GraphicView();

	void UpdateBasicInfo();
private:
	bool mOpen;
public:
	GraphicBasicInfo mBasicInfo;
};

#endif
