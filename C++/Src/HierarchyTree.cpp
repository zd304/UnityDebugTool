#include "HierarchyTree.h"

HierarchyTreeNode::HierarchyTreeNode(cJSON* json, int layer)
	: mParent(NULL), mLayer(layer)
{
	name = cJSON_GetObjectItem(json, "n")->valuestring;
	mInstanceID = cJSON_GetObjectItem(json, "i")->valueint;
	int active = cJSON_GetObjectItem(json, "a")->valueint;
	mActive = active > 0 ? true : false;
	mFoldout = false;

	HierarchyTree::mSceneNodes[mInstanceID] = this;

	mID = HierarchyTree::mTreeNodeID++;

	cJSON* childrenJson = cJSON_GetObjectItem(json, "c");
	if (childrenJson)
	{
		for (int i = 0; i < cJSON_GetArraySize(childrenJson); ++i)
		{
			cJSON* childJson = cJSON_GetArrayItem(childrenJson, i);
			HierarchyTreeNode* child = new HierarchyTreeNode(childJson, mLayer + 1);
			child->mParent = this;
			mChildren.push_back(child);
		}
	}
}

HierarchyTreeNode::~HierarchyTreeNode()
{
	mParent = NULL;
	for (size_t i = 0; i < mChildren.size(); ++i)
	{
		HierarchyTreeNode* child = mChildren[i];
		SAFE_DELETE(child);
	}
	mChildren.clear();
}

int HierarchyTree::mTreeNodeID = 0;
std::map<int, HierarchyTreeNode*> HierarchyTree::mSceneNodes;

HierarchyTree::HierarchyTree()
{

}

HierarchyTree::~HierarchyTree()
{
	Clear();
}

void HierarchyTree::Clear()
{
	for (size_t i = 0; i < mRoots.size(); ++i)
	{
		HierarchyTreeNode* root = mRoots[i];
		SAFE_DELETE(root);
	}
	mRoots.clear();
}

void HierarchyTree::GetPath(const HierarchyTreeNode* node, std::string& output)
{
	output = "";
	if (node == NULL)
		return;
	const HierarchyTreeNode* cur = node;
	do
	{
		output = cur->name + "/" + output;
		cur = cur->mParent;
	}
	while (cur);
	output = output.substr(0, output.length() - 1);
}

HierarchyTreeNode* HierarchyTree::GetNode(const std::string& path)
{
	std::vector<std::string> pathes;
	std::string stmp = "";
	for (size_t i = 0; i < path.size(); ++i)
	{
		char c = path[i];
		if (c == '/')
		{
			pathes.push_back(stmp);
			stmp = "";
		}
		else if (c == '\0')
		{
			break;
		}
		else
		{
			stmp.push_back(c);
		}
	}
	pathes.push_back(stmp);

	HierarchyTreeNode* root = NULL;
	for (size_t i = 0; i < pathes.size(); ++i)
	{
		std::string f = pathes[i];
		if (i == 0)
		{
			for (size_t j = 0; j < mRoots.size(); ++j)
			{
				HierarchyTreeNode* node = mRoots[j];
				if (node->name == f)
				{
					root = node;
					break;
				}
			}
		}
		else
		{
			for (size_t j = 0; j < root->mChildren.size(); ++j)
			{
				HierarchyTreeNode* node = root->mChildren[j];
				if (node->name == f)
				{
					root = node;
					break;
				}
			}
		}
	}

	return root;
}

HierarchyTreeNode* HierarchyTree::GetNode(int instanceID)
{
	std::map<int, HierarchyTreeNode*>::iterator it;
	it = HierarchyTree::mSceneNodes.find(instanceID);
	if (it != HierarchyTree::mSceneNodes.end())
		return it->second;
	return NULL;
}