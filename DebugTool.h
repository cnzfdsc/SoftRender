#pragma once

class DebugTool
{
public:
	DebugTool();

	void EnableDrawWireframe(bool enable);
	void SetDrawWireframe(bool wireframe);
	bool IsDrawWireframe();

	void EnableShowMousePos(bool enable);
	void SetShowMousePos(bool show);
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