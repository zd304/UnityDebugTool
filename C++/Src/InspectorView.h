#ifndef __INSPECTOR_VIEW_H__
#define __INSPECTOR_VIEW_H__

#include "inc.h"

struct InspectorData
{
	std::string path;
	int active;
	float px, py, pz;
	float rx, ry, rz;
	float sx, sy, sz;
};

class InspectorView
{
public:
	static InspectorView* GetInstance();

	static void DeleteInstance();

	void Update();

	void SetData(InspectorData* data);
private:
	static InspectorView* mInstance;

	InspectorView();
	~InspectorView();
public:
	bool mOpen;
private:
	InspectorData* mData;
};

#endif
