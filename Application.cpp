#include "InternalHeader.h"
#include "SRHeader.h"
#include "Application.h"

#include "MeshLoader.h"
#include "GraphicsContext.h"

static dword token;
GraphicsContext gGraphics;

void Init( dword hwnd )
{
	token = gRenderer.InitRender( hwnd, 800, 600 );

	Viewport vp;
	vp.n = 1.0f;
	vp.f = 5000.0f;
	vp.x = vp.y = 0;
	vp.w = 800;
	vp.h = 600;
	gRenderer.SetViewport( vp );

	gRenderer.mWorldTrans = Matrix4::CalcRotationMatrix( Vector3( 1.0f, 0.0f, 0.0f ), 0.0f, Vector3( 0.0f, 0.0f, 0.0f ) );

	gRenderer.mCamera.eye = Vector3( 0.0f, 0.0f, -5.0f );
	gRenderer.mCamera.look =  Vector3( 0.0f, 0.0f, 0.0f );
	gRenderer.mCamera.up = Vector3( 0.0f, 1.0f, 0.0f );

	gRenderer.mCamera.fovy = PI / 2;
	gRenderer.mCamera.ratio = (float) gRenderer.mViewport.w / gRenderer.mViewport.h;
	gRenderer.mCamera.nearz = gRenderer.mViewport.n;
	gRenderer.mCamera.farz = gRenderer.mViewport.f;

	gRenderer.mViewTrans = Matrix4::CalcViewMatrix( gRenderer.mCamera.eye, gRenderer.mCamera.look, gRenderer.mCamera.up );
	gRenderer.mProjTrans = Matrix4::CalcProjMatrix( gRenderer.mCamera.fovy, gRenderer.mCamera.ratio, gRenderer.mCamera.nearz, gRenderer.mCamera.farz );

	gGraphics.Initialize("Normals.fbx");
}

void Render(float elapse, dword hdc)
{
	gGraphics.RenderAll(elapse, hdc);
}

void Finalize( )
{
	gRenderer.FinalizeRender( token );
}

static int xx = -1, yy = -1;

void MouseMove( int x, int y )
{
	if ( ( GetAsyncKeyState(VK_LBUTTON) & 0x80000000 ) == 0 )
		return;

	if ( xx == -1 && yy == -1 )
	{
		xx = x;
		yy = y;
	}

	float xr = ( x - xx ) / 360.0f * PI;
	float yr = ( y - yy ) / 360.0f * PI;

	if ( ( GetAsyncKeyState(VK_LSHIFT) & 0x80000000 ) )
	{
		xr *= 0.2f;
		yr *= 0.2f;
	}

	xx = x;
	yy = y;

	Vector3 zaxis = gRenderer.mCamera.look - gRenderer.mCamera.eye;
	float len = zaxis.Magnitude( );
	Vector3 right = Vector3::Cross( gRenderer.mCamera.up, zaxis ).Normalize( );
	gRenderer.mCamera.eye *= Matrix4( ).Rotation( right, yr );
	gRenderer.mCamera.eye *= Matrix4( ).Rotation( gRenderer.mCamera.up, xr );
	Vector3 zaxisN = gRenderer.mCamera.look - gRenderer.mCamera.eye;
	if ( Vector3::Dot( Vector3::Cross( zaxis, right ), gRenderer.mCamera.up ) * Vector3::Dot( Vector3::Cross( zaxisN, right ), gRenderer.mCamera.up ) < 0.0f )
		gRenderer.mCamera.up = -gRenderer.mCamera.up;

	gRenderer.mViewTrans = Matrix4::CalcViewMatrix( gRenderer.mCamera.eye, gRenderer.mCamera.look, gRenderer.mCamera.up );
}

void LButtonUp( )
{
	xx = -1;
	yy = -1;
}

void KeyDown( int key )
{
	switch( key )
	{
	case VK_LEFT:
		gRenderer.mGeo.worldPos.x -= 0.2f;
		break;

	case VK_RIGHT:
		gRenderer.mGeo.worldPos.x += 0.2f;
		break;

	case VK_UP:
		gRenderer.mGeo.worldPos.y += 0.2f;
		break;

	case VK_DOWN:
		gRenderer.mGeo.worldPos.y -= 0.2f;
		break;

	case '1':
//		gDebugTool.mEnableDrawWireframe = !gDebugTool.mEnableDrawWireframe;
//		gDebugTool.mEnableShowMousePos = !gDebugTool.mEnableShowMousePos;
		break;

	case 'W':
	case 'w':
		gDebugTool.mDrawWireframe = !gDebugTool.mDrawWireframe;
		break;

	case 'M':
	case 'm':
		gDebugTool.mShowMousePos = !gDebugTool.mShowMousePos;
		break;
	}
}

void MouseWheel(int dis)
{
	Vector3 zaxis = gRenderer.mCamera.eye - gRenderer.mCamera.look;

	float len = zaxis.Magnitude();
	len = (len - ((float) dis) / WHEEL_DELTA / 3 <= 0) ? len : (len - ((float) dis) / WHEEL_DELTA / 3);
	zaxis.Normalize() *= len;

	gRenderer.mCamera.eye = gRenderer.mCamera.look + zaxis;

	gRenderer.mViewTrans = Matrix4::CalcViewMatrix(gRenderer.mCamera.eye, gRenderer.mCamera.look, gRenderer.mCamera.up);
}