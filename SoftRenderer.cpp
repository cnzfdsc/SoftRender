#include "InternalHeader.h"
#include "SRHeader.h"
#include "MeshLoader.h"

// Global

SoftRenderer gRenderer;

SoftRenderer::SoftRenderer()
{
	mFrameBuffer = NULL;
}

void SoftRenderer::SetViewport(const Viewport& vp)
{
	mViewport = vp;
}

void SoftRenderer::SetVertexBuffer( float* v, dword len )
{
	assert( len % 3 == 0 );
	for ( dword i = 0; i < len; ++ i )
	{
		mGeo.orgVertex[i].x = v[i * 3];
		mGeo.orgVertex[i].y = v[i * 3 + 1];
		mGeo.orgVertex[i].z = v[i * 3 + 2];
		mGeo.orgVertex[i].w = 1;
		
		float dis = Vector3::Distance( mGeo.worldPos, mGeo.orgVertex[i] );
		mGeo.radius = mGeo.radius < dis ? dis : mGeo.radius;
	}

	mGeo.vertexCount = len;
	mGeo.primCount = len / 3;
}

void SoftRenderer::SetVertexBuffer( Vector4* v, dword vertexNum )
{
	for (dword i = 0; i < vertexNum; i++)
	{
		mGeo.orgVertex[i] = v[i];

		float dis = Vector3::Distance(mGeo.worldPos, mGeo.orgVertex[i]);
		mGeo.radius = mGeo.radius < dis ? dis : mGeo.radius;
	}

	mGeo.vertexCount = vertexNum;
	if ( mGeo.useIndex == false )
		mGeo.primCount = vertexNum / 3;
}

void SoftRenderer::SetIndexBuffer(int* index, dword indexNum)
{
	for (dword i = 0; i < indexNum; i++)
	{
		mGeo.index[i] = index[i];
	}

	mGeo.useIndex = true;
	mGeo.indexCount = indexNum;
	mGeo.primCount = indexNum / 3;
}

void SoftRenderer::SetWireFrame(bool wireFrame)
{
	mGeo.drawWireframe = wireFrame;
}

dword SoftRenderer::InitRender(dword hwnd, int w, int h)
{
	RECT r;
	r.left = r.top = 100;
	r.right = w + 100;
	r.bottom = h + 100;
	::AdjustWindowRectEx( &r, WS_OVERLAPPEDWINDOW, false, NULL );
	::MoveWindow( (HWND) hwnd, r.left, r.top, r.right - r.left, r.bottom - r.top, true );
	::ShowWindow((HWND)hwnd, SW_SHOW);
	mWidth = w;
	mHeight = h;

	mFrameBuffer = new dword[w * h];

	GdiplusStartupInput	gdiplusStartupInput;
	dword				gdiplusToken;

	// Initialize GDI+.
	::GdiplusStartup( &gdiplusToken, &gdiplusStartupInput, NULL );
	return gdiplusToken;
}

void SoftRenderer::FinalizeRender( dword gdiplusToken )
{
	::GdiplusShutdown( gdiplusToken );
}

void SoftRenderer::SoftRender( float elapse, dword hdc )
{
	HDC dc = ::CreateCompatibleDC( (HDC) hdc );
	HBITMAP bmp = ::CreateCompatibleBitmap( (HDC) hdc, mWidth, mHeight );
	::SelectObject( dc, (HGDIOBJ) bmp );
	Graphics g( dc );
	#pragma message( "*** TODO: vertex color" )

	for (dword i = 0; i < mWidth * mHeight; i++)
		mFrameBuffer[i] = 0xFFAAAAAA;

	// Geometry clip.
	// �����Һܻ���, ��Ϊ�Ҿ��ÿɱ�̹�����û������һ���.
	#pragma message( "*** TODO: remove the mGeo.worldPos" )
	Vector3 pos = mGeo.worldPos * mWorldTrans * mViewTrans;
	float yedge = ::tan( mCamera.fovy / 2.0f ) * pos.z;
	float xedge = yedge * mCamera.ratio;
	if ( pos.x - mGeo.radius < xedge && pos.x + mGeo.radius > - xedge &&
		 pos.y - mGeo.radius < yedge && pos.y + mGeo.radius > - yedge &&
		 pos.z - mGeo.radius < mCamera.farz && pos.z + mGeo.radius > mCamera.nearz )
	{
		Matrix4 vpMatrix = Matrix4(
			mViewport.w / 2.0f,			     0.0f,		0.0f,		0.0f, 
						   0.0f, -mViewport.h / 2.0f,		0.0f,		0.0f,
						   0.0f,				 0.0f,		1.0f,		0.0f,
			mViewport.w / 2.0f,  mViewport.h / 2.0f,		0.0f,		1.0f );

		Matrix4 m = mWorldTrans * mViewTrans * mProjTrans;
		// Assemble primitives ͼԪ��װ.
		for (dword i = 0; i < mGeo.vertexCount; ++i)
			mGeo.transVertex[i] = (mGeo.orgVertex[i] + mGeo.worldPos) * m;

		if ( mGeo.useIndex == false )
		{
			for (dword i = 0; i < mGeo.vertexCount; ++i)
			{
				mGeo.prim[i / 3].vertIndex[i % 3] = i;
				mGeo.prim[i / 3].vertList = mGeo.transVertex;
				mGeo.prim[i / 3].visible = true;
			}
		}
		else
		{
			for (dword i = 0; i < mGeo.indexCount; i++)
			{
				mGeo.prim[i / 3].vertIndex[i % 3] = mGeo.index[i];
				mGeo.prim[i / 3].vertList = mGeo.transVertex;
				mGeo.prim[i / 3].visible = true;
			}
		}

		// Cull
		mGeo.primCountAfterCull = mGeo.primCount;
		mGeo.vertexCountAfterCull = mGeo.vertexCount;

		for( dword i = 0; i < mGeo.primCount; ++ i )
		{
			Primitive& prim = mGeo.prim[i];
			Vector4& v1 = prim.vertList[prim.vertIndex[0]],
				&v2 = prim.vertList[prim.vertIndex[1]],
				&v3 = prim.vertList[prim.vertIndex[2]];
			// ����������ϸ���˿�ogl����ʾЧ��, ������û�������µ������ΰ�, ֻ�ǰ�ͶӰ��Z�����ϵĶ��������һ������ͻ���
			// **��OPENGL����һ��ʵ�飬�о�Ӧ���ǽ���������.
			// **��׶�������εĲü��ͱ�����ѡ������������ε�, ����vertex shader��ʱ��Ӧ�û�û�������εĸ���.
			// **��������Ӧ������vs�����. �����Ժ���ܻ���Geometry Shaderʲô��, �Ժ���˵��.
			// ����: �ü�������͸��ͶӰ֮��, ͸�ӳ���֮ǰ, ��ʱ���껹���������.
			// ��ʱ�������εĸ���, �����޷��ü�.

			// ��׶�ü�������
			CVVCull( mGeo, i );

#pragma message( "*** TODO: disable cull back face on wireframe rendering" )
			//if ( Vector3::Cross( v2 - v1, v3 - v1 ).z >= 0.0f )
			//{
			//	prim.visible = false;
			//	continue;
			//}
		}

		for ( dword i = 0; i < mGeo.vertexCountAfterCull; ++ i )
		{
			mGeo.transVertex[i].PerspectiveDivision( );
			mGeo.transVertex[i] *= vpMatrix;
		}

		// Render
		Pen p( Color( 255, 0, 0 ) );
		for( dword i = 0; i < mGeo.primCountAfterCull; ++ i )
		{
			Primitive& prim = mGeo.prim[i];
			if ( prim.visible == false )
				continue;

			Vector4& v1 = prim.vertList[prim.vertIndex[0]],
				&v2 = prim.vertList[prim.vertIndex[1]],
				&v3 = prim.vertList[prim.vertIndex[2]];

			Rasterize(v1, v2, v3, mFrameBuffer, mWidth, mHeight, mGeo.drawWireframe);

// 			g.DrawLine( &p, v1.x, v1.y, v2.x, v2.y );
// 			g.DrawLine( &p, v2.x, v2.y, v3.x, v3.y );
// 			g.DrawLine( &p, v3.x, v3.y, v1.x, v1.y );
		}
	}
	
	BITMAPINFO info;
	memset(&info, 0, sizeof(info));
	info.bmiHeader.biSize = sizeof(info.bmiHeader);
	info.bmiHeader.biWidth = mWidth;
	info.bmiHeader.biHeight = mHeight;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = BI_RGB;
	info.bmiHeader.biSizeImage = 0;
	info.bmiHeader.biXPelsPerMeter = 0;
	info.bmiHeader.biYPelsPerMeter = 0;
	info.bmiHeader.biClrUsed = 0;
	info.bmiHeader.biClrImportant = 0;

	SetDIBits(dc, bmp, 0, mHeight, (byte*)mFrameBuffer, &info, DIB_RGB_COLORS);

	::BitBlt( (HDC) hdc, 0, 0, mWidth, mHeight, dc, 0, 0, SRCCOPY );
	
	gDebugTool.TextOutDebugInfo((HDC)hdc, 0x0000FFFF, 0x00AAAAAA);

	::DeleteObject( (HGDIOBJ)bmp );
	::DeleteDC( dc );
}

// ÿ������һ�βü��ж�
// ÿ�������������һ������һ���ж�, ��Ϊ4�����
// �����ڲ�, ���ӵ��������
// �����ⲿ, ������
// һ��һ��, ǰ��Ķ���(��˳ʱ����ʱ��ü�˳���ǰ��)������, �������ü����ཻ��, ��������
// һ��һ��, ����Ķ���������, ǰ��Ķ����������, �����ü����ཻ��, Ҳ�������� (��˳��)
// ������, ֻ�е����ж��㶼û�б仯, ����������ȫ�ڲü����ڲ�, ��ʹ��ԭ����������
// �������ԭ����������, ʹ������Ľ�������¹���������
// ����˳��Ϊ012, 023, 034����, ȷ��ʱ�뷽��˳�򲻱�

// �ĳɴ�geometry �� prim��index , ��Ϊ�����µ�prim. 
void SoftRenderer::CVVCull(Geometry& geo, dword primIndex)
{
	Primitive& prim = geo.prim[primIndex];
	Vector4 orgVertex[9] = { prim.vertList[prim.vertIndex[0]], prim.vertList[prim.vertIndex[1]], prim.vertList[prim.vertIndex[2]] };
	bool vertexInside[9];
	Vector4 resVertex[9];
	dword orgCount = 3, resCount = 0;

	// Plane, z = 0
	for (dword i = 0; i < orgCount; ++i)
		vertexInside[i] = orgVertex[i].z > 0;

	for (dword i = 0; i < orgCount; ++i)
	{
		dword second = (i + 1) % orgCount;
		if (vertexInside[i] && vertexInside[second])
		{
			resVertex[resCount++] = orgVertex[i];
			continue;
		}

		if (vertexInside[i] == false && vertexInside[second] == false)
			continue;

		dword in = vertexInside[i] ? i : second;
		dword out = vertexInside[i] ? second : i;

		if (out == (in + 1) % orgCount)
			resVertex[resCount++] = orgVertex[in];

		Vector4 inV = orgVertex[in], outV = orgVertex[out];
		Vector4 dir = outV - inV;
		outV.w *= -1.f;
		float t = -inV.z / dir.z;
		resVertex[resCount++] = inV + dir * t;
	}

	memcpy(orgVertex, resVertex, sizeof(Vector4) * 9);
	orgCount = resCount;
	resCount = 0;

	// Plane, x = 1
	for (dword i = 0; i < orgCount; ++i)
		vertexInside[i] = orgVertex[i].x < orgVertex[i].w;

	for (dword i = 0; i < orgCount; ++i)
	{
		dword second = (i + 1) % orgCount;
		if (vertexInside[i] && vertexInside[second])
		{
			resVertex[resCount++] = orgVertex[i];
			continue;
		}

		if (vertexInside[i] == false && vertexInside[second] == false)
			continue;

		dword in = vertexInside[i] ? i : second;
		dword out = vertexInside[i] ? second : i;

		if (out == (in + 1) % orgCount)
			resVertex[resCount++] = orgVertex[in];

		Vector4 dir = orgVertex[out] - orgVertex[in];
		float t = (orgVertex[in].w - orgVertex[in].x) / (dir.x - dir.w);
		resVertex[resCount++] = orgVertex[in] + dir * t;
	}

	memcpy(orgVertex, resVertex, sizeof(Vector4) * 9);
	orgCount = resCount;
	resCount = 0;

	// Plane, x = -1
	for (dword i = 0; i < orgCount; ++i)
		vertexInside[i] = -orgVertex[i].x < orgVertex[i].w;

	for (dword i = 0; i < orgCount; ++i)
	{
		dword second = (i + 1) % orgCount;
		if (vertexInside[i] && vertexInside[second])
		{
			resVertex[resCount++] = orgVertex[i];
			continue;
		}

		if (vertexInside[i] == false && vertexInside[second] == false)
			continue;

		dword in = vertexInside[i] ? i : second;
		dword out = vertexInside[i] ? second : i;

		if (out == (in + 1) % orgCount)
			resVertex[resCount++] = orgVertex[in];

		Vector4 dir = orgVertex[out] - orgVertex[in];
		float t = -(orgVertex[in].w + orgVertex[in].x) / (dir.x + dir.w);
		resVertex[resCount++] = orgVertex[in] + dir * t;
	}

	memcpy(orgVertex, resVertex, sizeof(Vector4) * 9);
	orgCount = resCount;
	resCount = 0;

	// Plane, y = 1
	for (dword i = 0; i < orgCount; ++i)
		vertexInside[i] = orgVertex[i].y < orgVertex[i].w;

	for (dword i = 0; i < orgCount; ++i)
	{
		dword second = (i + 1) % orgCount;
		if (vertexInside[i] && vertexInside[second])
		{
			resVertex[resCount++] = orgVertex[i];
			continue;
		}

		if (vertexInside[i] == false && vertexInside[second] == false)
			continue;

		dword in = vertexInside[i] ? i : second;
		dword out = vertexInside[i] ? second : i;

		if (out == (in + 1) % orgCount)
			resVertex[resCount++] = orgVertex[in];

		Vector4 dir = orgVertex[out] - orgVertex[in];
		float t = (orgVertex[in].w - orgVertex[in].y) / (dir.y - dir.w);
		resVertex[resCount++] = orgVertex[in] + dir * t;
	}

	memcpy(orgVertex, resVertex, sizeof(Vector4) * 9);
	orgCount = resCount;
	resCount = 0;

	// Plane, y = -1
	for (dword i = 0; i < orgCount; ++i)
		vertexInside[i] = -orgVertex[i].y < orgVertex[i].w;

	for (dword i = 0; i < orgCount; ++i)
	{
		dword second = (i + 1) % orgCount;
		if (vertexInside[i] && vertexInside[second])
		{
			resVertex[resCount++] = orgVertex[i];
			continue;
		}

		if (vertexInside[i] == false && vertexInside[second] == false)
			continue;

		dword in = vertexInside[i] ? i : second;
		dword out = vertexInside[i] ? second : i;

		if (out == (in + 1) % orgCount)
			resVertex[resCount++] = orgVertex[in];

		Vector4 dir = orgVertex[out] - orgVertex[in];
		float t = -(orgVertex[in].w + orgVertex[in].y) / (dir.y + dir.w);
		resVertex[resCount++] = orgVertex[in] + dir * t;
	}

	memcpy(orgVertex, resVertex, sizeof(Vector4) * 9);
	orgCount = resCount;
	resCount = 0;

	// Plane, z = 1
	for (dword i = 0; i < orgCount; ++i)
		vertexInside[i] = orgVertex[i].z < orgVertex[i].w;

	for (dword i = 0; i < orgCount; ++i)
	{
		dword second = (i + 1) % orgCount;
		if (vertexInside[i] && vertexInside[second])
		{
			resVertex[resCount++] = orgVertex[i];
			continue;
		}

		if (vertexInside[i] == false && vertexInside[second] == false)
			continue;

		dword in = vertexInside[i] ? i : second;
		dword out = vertexInside[i] ? second : i;

		if (out == (in + 1) % orgCount)
			resVertex[resCount++] = orgVertex[in];

		Vector4 dir = orgVertex[out] - orgVertex[in];
		float t = (orgVertex[in].w - orgVertex[in].z) / (dir.z - dir.w);
		resVertex[resCount++] = orgVertex[in] + dir * t;
	}

	// ֻ��ԭ��������ȫû�б��ü�ʱ��ʹ��ԭ�����εĶ�������
	if (resCount == 3 &&
		resVertex[0] == prim.vertList[prim.vertIndex[0]] &&
		resVertex[1] == prim.vertList[prim.vertIndex[1]] &&
		resVertex[2] == prim.vertList[prim.vertIndex[2]])
		return;
	else
		prim.visible = false;

	if (resCount == 0)
	{
		prim.visible = false;
		return;
	}

	// ����һ�ɷŵ��������еĺ���
	// �µ�prim��Ϸ�ʽ�����η�ʽ
	for (dword i = 0; i < resCount; ++i)
	{
		geo.transVertex[geo.vertexCountAfterCull++] = resVertex[i];
	}
	// �Ѳü���Ķ������������
	dword startIndex = geo.vertexCountAfterCull - resCount;
	// ��û�뵽ʲô�ð취��һ���м���������, ������ȥ���Ƕ���-2��
	for (dword i = 0; i < resCount - 2; ++i)
	{
		geo.prim[geo.primCountAfterCull].vertList = geo.transVertex;
		geo.prim[geo.primCountAfterCull].visible = true;

		for (dword j = 0; j < 3; ++j)
		{
			geo.prim[geo.primCountAfterCull].vertIndex[0] = startIndex;
			geo.prim[geo.primCountAfterCull].vertIndex[1] = startIndex + i + 1;
			geo.prim[geo.primCountAfterCull].vertIndex[2] = startIndex + i + 2;
		}

		geo.primCountAfterCull++;
	}
}

bool SortByY(const Vector4* v1, const Vector4* v2)
{
	return v1->y < v2->y;
}

float AdjustXPos(float orgX, float destX, float curY, const Vector4& diff, bool left)
{
	// �����ֻ����б��Ϊ��ʱ����������, �ұ���ֻ����б��Ϊ���ǲ���������. ���б����0��ʲô������
	if (diff.x < 0 != left || abs(diff.x) < ZERO_DELTA || abs(diff.y) < ZERO_DELTA )
		return orgX;

	float slope = diff.x / diff.y;
	float moveX = slope * ((int)curY + 1 - curY);
	float retX = orgX;
	
	// ���ܸ���һ����ͬ����X����
	retX = orgX + moveX;
	float nextX = orgX + slope;
	if (abs((int)(nextX + 0.5)) == abs((int)(orgX + 0.5)) 
		&& abs(abs((int)(orgX + 0.5)) - abs((int)(nextX + 0.5))) >= 1)
		retX -= slope > 0 ? 1 : -1;

	if (retX * diff.x > destX * diff.x)
		retX = destX;

	return retX;
}

void SoftRenderer::Rasterize(const Vector4& v0, const Vector4& v1, const Vector4& v2, dword* bitmap, int width, int height, bool wireframe)
{
#pragma message( "*** TODO: ����汾��ɨ�����㷨̫������, Ҫ��дһ��" )
	std::vector<const Vector4*> vList;
	vList.push_back(&v0);
	vList.push_back(&v1);
	vList.push_back(&v2);  

	std::sort(vList.begin(), vList.end(), SortByY);
	
	// ����Ŀ���
	Vector4 slope10 = *vList[1] - *vList[0];
	Vector4 slope20 = *vList[2] - *vList[0];
	const Vector4* leftDest = slope10.x / slope10.y < slope20.x / slope20.y ? vList[1] : vList[2];
	const Vector4* rightDest = slope10.x / slope10.y > slope20.x / slope20.y ? vList[1] : vList[2];
	// ����б��
	Vector4 leftDiff = *leftDest - *vList[0];
	Vector4 rightDiff = *rightDest - *vList[0];
	// ��ǰY����
	float curY = vList[0]->y;
	// ���ҵ�ǰX����
	float leftCurX = AdjustXPos(vList[0]->x, leftDest->x, curY, leftDiff, true);
	float rightCurX = AdjustXPos(vList[0]->x, rightDest->x, curY, rightDiff, false);
	float lastLeftX = leftCurX;
	float lastRightX = rightCurX;
	
	bool drawHorizontal = false;

	// ��������ĵ㿪ʼ����ɨ��
	for (curY = vList[0]->y; curY < vList[2]->y + 0.5f; curY += 1.0f)
	{
		// �����Ϊб��̫С, Y��������1, X�����Խ�������ζ���, ���X�����׼�����ζ���
		if (leftCurX * leftDiff.x >= leftDest->x * leftDiff.x)
			leftCurX = leftDest->x;

		if (rightCurX * rightDiff.x >= rightDest->x * rightDiff.x)
			rightCurX = rightDest->x;

		// ��б����0(Ҳ��������������һ��ˮƽ����), ����ɨ�����Ѿ�Խ��һ������ʱ, �����趨б��
		if ((abs(leftDiff.y) <= ZERO_DELTA || curY > leftDest->y) && leftDest !=  vList[2])
		{
			const Vector4* oldLeftDest = leftDest;
			leftDest = vList[2];
			leftDiff = *leftDest - *oldLeftDest;
			leftCurX = AdjustXPos(oldLeftDest->x, leftDest->x, curY, leftDiff, true);
		}

		if ((abs(rightDiff.y) <= ZERO_DELTA || curY > rightDest->y) && rightDest != vList[2])
		{
			const Vector4* oldRightDest = rightDest;
			rightDest = vList[2];
			rightDiff = *rightDest - *oldRightDest;
			rightCurX = AdjustXPos(oldRightDest->x, rightDest->x, curY, rightDiff, false);
		}

		// ��ɨ������������ʼ������������
		int sign = leftCurX < rightCurX ? 1 : -1;

		// �߲��ܶ�. 
		// �����Ϊ��, ��ôд��Ϊ�˷�ֹ curLeftX > lastRightX + 1 �������
		// ���ǲ���ֱ�ӸĶ� leftCurX, ��Ϊ���Ǹ���б�ʼ���ı��е�X, ����Ķ�, б�ʼ���ʹ���. ���Ե�дһ��PaintX
		float leftPaint = leftCurX;
		float rightPaint = rightCurX;

		if (leftPaint > lastLeftX + 1)
			leftPaint = lastLeftX + 1;

		if (rightPaint < lastRightX - 1)
			rightPaint = lastRightX - 1;

		for (int i = (int)(leftPaint + 0.5); i * sign <= (int)(rightPaint + 0.5) * sign; i += sign)
		{
			int py = (int)(curY + 0.5);
			if (py < 0 || py >= height)
				break;
			bitmap[width * (height - py - 1) + i] = 0xFFAA0000;
		}

		// �ƶ���һ����ʼ�ͽ������X����
		lastLeftX = leftCurX;
		lastRightX = rightCurX;

		leftCurX += leftDiff.x / leftDiff.y;
		rightCurX += rightDiff.x / rightDiff.y;

		drawHorizontal = false;
	}
}