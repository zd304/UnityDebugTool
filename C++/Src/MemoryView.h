#ifndef __MEMORY_VIEW_H__
#define __MEMORY_VIEW_H__

#include "MemoryData.h"

class MemoryView
{
public:
	static MemoryView* mInstance;

	static MemoryView* GetInstance();

	static void DeleteInstance();

	void Update();

	void RequestMemoryObjInfo(MemoryObjType type);

	void Clear();
private:
	MemoryView();

	~MemoryView();

	void SortByMemorySize(int t);
public:
	bool mOpen;
	size_t mSelect;

	bool mLock;
	MemoryData** mDatas;
	int mDatasSize;
	int mTotalSize;
	std::string mMemoryTypeNames[MemoryObjType_Count];
};

#endif
