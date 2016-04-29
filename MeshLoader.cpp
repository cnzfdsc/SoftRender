#include "InternalHeader.h"
#include "SRHeader.h"
#include "MeshLoader.h"

Mesh::Mesh()
	: mVertex(NULL)
	, mVertexNum(0)
	, mIndex(NULL)
	, mIndexNum(0)
	, mColor(0xff000000)
{

}

void Mesh::SetIndex(int* index, int num)
{
	mIndex = new int[num];
	mIndexNum = num;

	for (int i = 0; i < num; i++)
		mIndex[i] = index[i];
}

Vector4* Mesh::GetVertex()
{
	return mVertex;
}

int* Mesh::GetIndex()
{
	return mIndex;
}

void FBXMesh::SetVertex(void* vertex, int num)
{
	mVertex = new Vector4[num];
	mVertexNum = num;

	FbxVector4* fbxVertex = (FbxVector4*)vertex;
	for (int i = 0; i < num; i++)
	{
		mVertex[i].x = (float)fbxVertex[i][0];
		mVertex[i].y = (float)fbxVertex[i][1];
		mVertex[i].z = (float)fbxVertex[i][2];
		mVertex[i].w = (float)fbxVertex[i][3];
	}
}

void CustomMesh::SetVertex(void* vertex, int num)
{
	mVertex = new Vector4[num];
	mVertexNum = num;

	Vector4* customVertex = (Vector4*)vertex;
	for (int i = 0; i < num; i++)
	{
		mVertex[i].x = (float)customVertex[i].x;
		mVertex[i].y = (float)customVertex[i].y;
		mVertex[i].z = (float)customVertex[i].z;
		mVertex[i].w = (float)customVertex[i].w;
	}
}

bool MeshLoader::LoadFile(const char* fileName)
{
	// 初始化
	mManager = FbxManager::Create();

	FbxIOSettings* ios = FbxIOSettings::Create(mManager, IOSROOT);
	ios->SetBoolProp(IMP_ANIMATION, false);
	ios->SetBoolProp(IMP_SETLOCKEDATTRIB, false);
	mManager->SetIOSettings(ios);

	mScene = FbxScene::Create(mManager, "My Scene");

	// 读文件
	FbxImporter* importer = FbxImporter::Create(mManager, "");
	importer->Initialize(fileName, -1, mManager->GetIOSettings());
	importer->Import(mScene);

	// 转换格式
	FbxGeometryConverter geoConverter(mManager);
	geoConverter.Triangulate(mScene, true);

	// 遍历节点
	FbxNode* rootNode = mScene->GetRootNode();
	WalkHierarchy(rootNode);
	importer->Destroy();

	mLoad = true;

	return true;
}

int MeshLoader::LoadMesh(Mesh** ppMeshList, int* num)
{
	*num = mMeshVec.size();
	*ppMeshList = new FBXMesh[mMeshVec.size()];

	for (dword i = 0; i < mMeshVec.size(); i++)
	{
		FBXMesh* mesh = (FBXMesh*)&(*ppMeshList)[i];

		mesh->SetVertex(mMeshVec[i]->GetControlPoints(), mMeshVec[i]->GetControlPointsCount());
		mesh->SetIndex(mMeshVec[i]->GetPolygonVertices(), mMeshVec[i]->GetPolygonVertexCount());
	}

	return true;
}

void MeshLoader::WalkHierarchy(FbxNode* node)
{
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		FbxNode* childNode = node->GetChild(i);
		FbxNodeAttribute* attr = childNode->GetNodeAttribute();

		if (attr->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			mMeshVec.push_back((FbxMesh*)attr);

			FbxMesh* mesh = (FbxMesh*)attr;
			std::cout << "TriangleCount:" << mesh->GetPolygonCount()
				<< "  VertexCount:" << mesh->GetControlPointsCount()
				<< "  IndexCount:" << mesh->GetPolygonVertexCount()
				<< "   Layer:" << mesh->GetLayerCount()
				<< "  DeformerCount:" << mesh->GetDeformerCount(FbxDeformer::eSkin)
				<< "  MaterialCount:" << childNode->GetMaterialCount();
		}

		//FbxSurfaceMaterial* matl = childNode->GetMaterial();

		if (childNode->GetChildCount() != 0)
			WalkHierarchy(childNode);
	}
}