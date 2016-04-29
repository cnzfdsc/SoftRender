#pragma once
#include <fbxsdk.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "SRHeader.h"

// ��ʱ��Mesh��, �Ժ��IMesh, ÿ����ȡ�ļ�����̳�һ��
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

// ��ȡFBX�ļ�����
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