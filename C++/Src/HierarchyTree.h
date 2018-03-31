#ifndef __HIERARCHY_TREE_H__
#define __HIERARCHY_TREE_H__

#include "inc.h"

class HierarchyTreeNode
{
public:
	HierarchyTreeNode(cJSON* json, int layer = 0);
	~HierarchyTreeNode();
public:
	HierarchyTreeNode* mParent;
	std::vector<HierarchyTreeNode*> mChildren;

	std::string name;
	bool mActive;
	bool mFoldout;
	int mID;
	int mLayer;
};

class HierarchyTree
{
public:
	HierarchyTree();
	~HierarchyTree();

	void Clear();

	void GetPath(const HierarchyTreeNode* node, std::string& output);

	HierarchyTreeNode* GetNode(const std::string& path);
public:
	std::vector<HierarchyTreeNode*> mRoots;
	static int mTreeNodeID;
};

#endif