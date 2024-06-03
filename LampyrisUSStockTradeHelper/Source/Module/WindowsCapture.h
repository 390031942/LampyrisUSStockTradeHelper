/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: WindowsCapture.h
** Desc: 实现了股票快速交易的窗口
****************************************************************************/
#pragma once

#include <QPixmap>
#include <Windows.h>
#include <QtWinExtras/QtWin>

class WindowsCapture {
private:
	struct LPARAM_INFO {
		const char* targetClassName;
		HWND        foundWindow; 
		const char* parentClassName;
	};
public:
	enum Anchor {
		TopLeft = 0,
		BottomLeft = 1,
		BottomRight = 2,
		TopRight = 3,
	};
private:
	std::string          m_windowClassName;

	std::string          m_windowParentClassName;

	HWND                 m_hWnd;

	HDC                  m_hWindowDC;

	HDC                  m_hMemoryDC;

	static std::string   ms_targetClassName;

	static BOOL CALLBACK EnumChildWindowProc(HWND hwnd, LPARAM lParam);

	static HWND          FindWindowByClassName(std::string& className,std::string& parentClassName);
public:
	                     WindowsCapture(const std::string& windowClassName, const std::string& windowParentClassName);
			            
	virtual             ~WindowsCapture();
			            
	bool                 Init();
			            
	void                 Capture(const std::string& savePath,float width,float height,float offsetX,float offsetY, Anchor anchor);

	bool                 CheckWindowValid();

	DWORD                GetWindowProcessID();
};