#include "InternalHeader.h"
#include "SRHeader.h"
#include "MeshLoader.h"
#include "GraphicsContext.h"

bool gLoadFbxModel = false;

// 用于测试的简单正方形和正方体
Vector4 triangle[] =
{
	Vector4(1.0f, 1.0f, 0.0f, 0),
	Vector4(1.0f, -1.0f, 0.0f),
	Vector4(-1.0f, 1.0f, 0.0f),
	Vector4(-1.0f, 1.0f, 0.0f),
	Vector4(1.0f, -1.0f, 0.0f),
	Vector4(-1.0f, -1.0f, 0.0f)
};

Vector4 vb[] =
{
	Vector4(1, 1, 1, 0),
	Vector4(1, 1, -1, 0),
	Vector4(-1, 1, -1, 0),
	Vector4(-1, 1, 1, 0),
	Vector4(1, -1, 1, 0),
	Vector4(1, -1, -1, 0),
	Vector4(-1, -1, -1, 0),
	Vector4(-1, -1, 1, 0)
};

int ib[] =
{
	0, 1, 2,
	2, 3, 0,
	0, 4, 5,
	5, 1, 0,
	1, 5, 6,
	6, 2, 1,
	2, 6, 7,
	7, 3, 2,
	3, 7, 4,
	4, 0, 3,
	5, 4, 7,
	7, 6, 5
};

RenderContext::RenderContext( )
{
	mTransMtx = Matrix4::cIdentity;
}

RenderContext::RenderContext( Mesh* mesh, const Matrix4& trans )
{
	mMesh = mesh;
	mTransMtx = trans;
}

void RenderContext::SetTranslate( Vector4 vec )
{
	mTransMtx.Translation( vec );
}

GraphicsContext& GraphicsContext::Singleton( )
{
	static GraphicsContext singleton;
	return singleton;
}

void GraphicsContext::Initialize(const char* fileName)
{
	if (gLoadFbxModel)
	{
		if (fileName == NULL)
			return;

		if (mMeshLoader.LoadFile(fileName) == false)
		{
			MessageBox(NULL, "找不到模型文件", "错误", MB_OK);
			return;
		}

		Mesh* pMeshList = NULL;
		int meshNum = 0;
		mMeshLoader.LoadMesh(&pMeshList, &meshNum);

#pragma message( "*** TODO: 暂时只画第一个Mesh" )
		for (int i = 0; i < 1; i++)
			mRenderContextVec.push_back(RenderContext(&pMeshList[i]));
	}
	else
	{
		CustomMesh* mesh = new CustomMesh();
		mesh->SetVertex(triangle, 6);
		//mesh.SetVertex(vb, 8);
		//mesh.SetIndex(ib, 12 * 3);

		mRenderContextVec.push_back(RenderContext(mesh));
	}
}

void GraphicsContext::RenderAll(float elapse, dword hdc)
{
	for (RenderContextVec::iterator it = mRenderContextVec.begin(); it != mRenderContextVec.end(); it++)
	{
		RenderContext& context = *it;
		gRenderer.SetVertexBuffer(context.mMesh->GetVertex(), context.mMesh->mVertexNum);
		if (context.mMesh->mIndex != NULL)
			gRenderer.SetIndexBuffer(context.mMesh->GetIndex(), context.mMesh->mIndexNum);

		gRenderer.SetWireFrame(gDebugTool.IsDrawWireframe());

		gRenderer.SoftRender(elapse, hdc);
	}
}