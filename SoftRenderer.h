#pragma once
#include <windows.h>

const float PI = 3.1415926536f;

struct Viewport
{
	long x, y, w, h;
	float n, f;

	Viewport( ) : x( 0 ), y( 0 ), w( 0 ), h( 0 ), n( 0.0f ), f( 0.0f )
		{}
};

struct Camera
{
	Vector3 eye;
	Vector3 look;
	Vector3 up;

	float nearz;
	float farz;
	float fovy;
	float ratio;
};

struct Primitive
{
	dword		vertIndex[3];
	Vector4*	vertList;
	bool		visible;

	inline Primitive( ) : visible( false )
		{}
};

struct Geometry
{
	Vector4			orgVertex[10240];
	Vector4			transVertex[10240];
	int				index[10240];
	Primitive		prim[10240];

	dword			vertexCount;
	dword			indexCount;
	dword			primCount;
	bool			useIndex;
	dword			vertexCountAfterCull;
	dword			primCountAfterCull;

	bool			drawWireframe;

	Vector3			worldPos;
	float			radius;

	Geometry( )
		: useIndex(false)
		, worldPos(0.0f, 0.0f, 0.0f)
		, radius(0.0f)
		, drawWireframe(false)
	{}
};

// Soft Renderer.

class SoftRenderer
{
public:
	Camera mCamera;
	Viewport mViewport;
	Geometry mGeo;
	Matrix4 mWorldTrans, mViewTrans, mProjTrans;
	dword mWidth, mHeight;
	dword* mFrameBuffer;

public:
	SoftRenderer();
	void SetViewport( const Viewport& vp );
	void SetVertexBuffer( float* v, dword len );
	void SetVertexBuffer(Vector4* v, dword vertexNum);
	void SetIndexBuffer( int* i, dword indexNum );
	void SetWireFrame(bool wireFrame);

	dword InitRender( dword hwnd, int w, int h );
	void FinalizeRender( dword gdiplusToken );
	void SoftRender(float elapse, dword hdc);

private:
	void CVVCull(Geometry& geo, dword primIndex);
	void Rasterize(const Vector4& v0, const Vector4& v1, const Vector4& v2, dword* bitmap, int width, int height, bool wireframe);
};

extern SoftRenderer gRenderer;