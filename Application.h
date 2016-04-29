#pragma once
#include <windows.h>

void Init( dword hwnd );
void Render( float elapse, dword hdc );
void Finalize( );

void MouseMove( int x, int y );
void LButtonUp( );
void KeyDown( int key );
void MouseWheel( int dis );