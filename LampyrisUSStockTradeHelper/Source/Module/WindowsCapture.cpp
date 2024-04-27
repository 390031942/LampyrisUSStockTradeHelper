#include "WindowsCapture.h"

std::string WindowsCapture::ms_targetClassName;

// "ContainerWnd_DC"
BOOL WindowsCapture::EnumChildWindowProc(HWND hwnd, LPARAM lParam) {
	char className[256];
	LPARAM_INFO* pInfo = reinterpret_cast<LPARAM_INFO*>(lParam); // 从lParam获取传递的数据

	GetClassName(hwnd, className, sizeof(className));
	if (strcmp(className, pInfo->targetClassName) == 0) {
		RECT rect;
		::GetClientRect(hwnd, &rect); // 获取窗口大小
		int height = rect.bottom - rect.top;
		if (height > 10.0f) {
			pInfo->foundWindow = hwnd;
			return FALSE; // 找到窗口后停止遍历
		}
	}
	return TRUE; // 继续遍历
}

HWND WindowsCapture::FindWindowByClassName(std::string& className) {
	HWND hwnd = GetDesktopWindow(); // 获取桌面窗口句柄

	LPARAM_INFO data;
	data.targetClassName = className.c_str();
	data.foundWindow = NULL;

	EnumChildWindows(hwnd, EnumChildWindowProc, reinterpret_cast<LPARAM>(&data));
	return data.foundWindow;
}

WindowsCapture::WindowsCapture(const std::string& windowClassName) :
	m_windowClassName(windowClassName), m_hWnd(NULL), m_hWindowDC(NULL), m_hMemoryDC(NULL) {
}

WindowsCapture::~WindowsCapture() {
	::DeleteDC(this->m_hMemoryDC);
	::ReleaseDC(this->m_hWnd, this->m_hWindowDC);
}

bool WindowsCapture::Init() {
	this->m_hWnd = FindWindowByClassName(m_windowClassName);
	if (!this->m_hWnd) {
		return false;
	}

	this->m_hWindowDC = ::GetWindowDC(this->m_hWnd); // 获取窗口DC
	this->m_hMemoryDC = ::CreateCompatibleDC(this->m_hWindowDC); // 创建兼容DC

	return true;
}

void WindowsCapture::Capture(const std::string& savePath, float width, float height, float offsetX, float offsetY, Anchor anchor) {
	RECT rect;
	GetClientRect(this->m_hWnd, &rect); // 获取窗口大小
	int winWidth = rect.right - rect.left;
	int winHeight = rect.bottom - rect.top;

	HBITMAP hBitmap = CreateCompatibleBitmap(this->m_hWindowDC, winWidth, winHeight); // 创建位图
	HGDIOBJ oldObj = SelectObject(this->m_hMemoryDC, hBitmap); // 选择位图到DC

	// 复制窗口内容到位图
	BitBlt(this->m_hMemoryDC, 0, 0, winWidth, winHeight, this->m_hWindowDC, 0, 0, SRCCOPY);

	// 将HBITMAP转换为QPixmap
	QPixmap pixmap = QtWin::fromHBITMAP(hBitmap);

	// 释放资源
	SelectObject(this->m_hMemoryDC, oldObj);
	DeleteObject(hBitmap);

	switch (anchor) {
	case Anchor::TopLeft : // 左上角
		pixmap = pixmap.copy(offsetX, offsetY, width, height);
		break;
	case Anchor::BottomLeft: // 左下角
		pixmap = pixmap.copy(offsetX, winHeight - height - offsetY, width, height);
		break;
	case Anchor::TopRight: // 右上角
		pixmap = pixmap.copy(winWidth - width - offsetX, offsetY, width, height);
		break;
	case Anchor::BottomRight: // 右下角
		pixmap = pixmap.copy(winWidth - width - offsetX, winHeight - height - offsetY, width, height);
		break;
	}
	pixmap.save(QString::fromStdString(savePath));
}
