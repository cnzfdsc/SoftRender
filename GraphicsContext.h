#pragma once

class RenderContext
{
public:
	Mesh* mMesh;
	Matrix4 mTransMtx;

	RenderContext();
	RenderContext(Mesh* mesh, const Matrix4& trans = Matrix4::cIdentity);

	void SetTranslate(Vector4 vec);
};

class GraphicsContext
{
public:
	static GraphicsContext& Singleton();
	typedef std::vector<RenderContext> RenderContextVec;
	RenderContextVec mRenderContextVec;

	void Initialize(const char* fileName);

	inline void AddToRenderList(const RenderContext& context)
	{
		mRenderContextVec.push_back(context);
	}

	void RenderAll(float elapse, dword hdc);

private:
	MeshLoader mMeshLoader;
};