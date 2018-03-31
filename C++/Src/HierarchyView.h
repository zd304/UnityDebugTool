#ifndef _HIERARCHY_VIEW_H__
#define _HIERARCHY_VIEW_H__

#include "inc.h"
#include "HierarchyTree.h"

class HierarchyView
{
public:
	static HierarchyView* mInstance;

	static HierarchyView* GetInstance();

	static void DeleteInstance();

	void Update();
private:
	HierarchyView();

	void NodeGUI(HierarchyTreeNode* node);
public:
	bool mOpen;
	HierarchyTree mTree;
	int mSelect;
};

#endif