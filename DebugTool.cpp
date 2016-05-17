#include "InternalHeader.h"
#include "SRHeader.h"
#include "DebugTool.h"

DebugTool gDebugTool;

DebugTool::DebugTool()
	: mCurTextY(0)
	, mEnableDrawWireframe(false)
	, mEnableShowMousePos(true)
	, mMouseX(0)
	, mMouseY(0)
{

}

bool DebugTool::IsDrawWireframe()
{
	return mDrawWireframe && mEnableDrawWireframe;
}

bool DebugTool::IsShowMousePos()
{
	return mEnableShowMousePos && mShowMousePos;
}

const int YOffset = 20;

void DebugTool::TextOutDebugInfo(HDC hdc, dword color, dword bgColor)
{
	mCurTextY = 0;

	::SetTextColor((HDC)hdc, color);
	::SetBkColor((HDC)hdc, bgColor);

	if (mEnableDrawWireframe)
	{
		const char* str = "Press 'W' to switch WireFrame";
		::TextOut(hdc, 0, mCurTextY, str, strlen(str));
		mCurTextY += YOffset;
	}

	if (mEnableShowMousePos)
	{
		const char* str = "Press 'M' to switch show mouse pos";
		::TextOut(hdc, 0, mCurTextY, str, strlen(str));
		mCurTextY += YOffset;

		if (mShowMousePos)
		{
			char mouseStr[32] = { 0 };
			::sprintf_s(mouseStr, 32, "MousePos: %d, %d", mMouseX, mMouseY);
			::TextOut(hdc, 0, gRenderer.mHeight - YOffset, mouseStr, strlen(mouseStr));
		}
	}
}