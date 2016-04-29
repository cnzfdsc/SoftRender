#pragma once
#include <fbxsdk.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "SRHeader.h"

// 临时的Mesh类, 以后该IMesh, 每个读取文件类里继承一个
class Mesh
{
public:
	Vector4* mVertex;
	int mVertexNum;
	int* mIndex;
	int mIndexNum;
	dword mColor;

public:
	Mesh();

	virtual void SetVertex(void* vertex, int num) = 0;
	void SetIndex(int* index, int num);
	Vector4* GetVertex();
	int* GetIndex();
};

class FBXMesh : public Mesh
{
public:
	virtual void SetVertex(void* vertex, int num);
};

class CustomMesh : public Mesh
{
public:
	virtual void SetVertex(void* vertex, int num);
};

// 读取FBX文件的类
class MeshLoader
{
	FbxScene* mScene;
	FbxManager* mManager;
	std::vector<FbxMesh*> mMeshVec;
	bool mLoad;

public:
	MeshLoader() : mScene(NULL), mManager(NULL), mLoad(false)
	{}

	bool LoadFile(const char* fileName);
	int LoadMesh(Mesh** ppMeshList, int* num);

private:
	void WalkHierarchy(FbxNode* node);
};