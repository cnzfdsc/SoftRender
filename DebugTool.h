#pragma once

class DebugTool
{
public:
	DebugTool();

	bool IsDrawWireframe();
	bool IsShowMousePos();
	
	void TextOutDebugInfo(HDC hdc, dword color, dword bgColor);

public:
	bool mEnableDrawWireframe;
	bool mDrawWireframe;

	bool mEnableShowMousePos;
	bool mShowMousePos;

	int mMouseX, mMouseY;

	int mCurTextY;
};

extern DebugTool gDebugTool;