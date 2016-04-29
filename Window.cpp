#include "InternalHeader.h"
#include "SRHeader.h"
#include "Application.h"

#include <windows.h>
#include <mmsystem.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING] = "SoftRender";
TCHAR szWindowClass[MAX_LOADSTRING] = "SoftRenderWindow";

ATOM				MyRegisterClass( HINSTANCE hInstance );
HWND				InitInstance( HINSTANCE, int );
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MyRegisterClass(hInstance);

	// Perform application initialization:
	HWND hwnd = NULL;
	if ( ( hwnd = InitInstance( hInstance, nCmdShow ) ) == NULL )
		return FALSE;

	Init( (dword) hwnd );

	bool done = false;
	dword lastTime = timeGetTime( );
	MSG msg;

	while( !done )
	{
		while( !done && PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if( WM_QUIT == msg.message )
				done = true;

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		dword currTime = timeGetTime( );
		dword delta = currTime - lastTime;
		if ( delta < 17 )
			Sleep( 17 - delta );
		#pragma message( "*** TODO: render to frame buffer" )
		::InvalidateRect( hwnd, NULL, true );

		lastTime = currTime;
	}

	Finalize( );
	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground	= (HBRUSH)( COLOR_WINDOW + 1 );
	wcex.lpszMenuName	= MAKEINTRESOURCE(0);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

HWND InitInstance( HINSTANCE hInstance, int nCmdShow )
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, 0, 0, NULL, NULL, hInstance, NULL );

   if ( !hWnd )
      return NULL;

   ShowWindow( hWnd, SW_HIDE );
   UpdateWindow( hWnd );

   return hWnd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		{
			hdc = ::BeginPaint(hWnd, &ps);

			static float lastTime = (float) timeGetTime( );

			float currTime = (float) timeGetTime( );
			// Render to Frame buffer.

			Render((currTime - lastTime) / 1000.f, (dword)hdc);

			lastTime = currTime;

			::EndPaint(hWnd, &ps);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_ERASEBKGND:
		return true;

	case WM_MOUSEMOVE:
		gDebugTool.mMouseX = LOWORD(lParam);
		gDebugTool.mMouseY = HIWORD(lParam);
		MouseMove( LOWORD( lParam ), HIWORD( lParam ) );
		break;

	case WM_LBUTTONUP:
		LButtonUp( );
		break;

	case WM_MOUSEWHEEL:
		MouseWheel( (short) ( (wParam & 0xFFFF0000) >> 16 ) );
		break;

	case WM_KEYDOWN:
		KeyDown( wParam );
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}