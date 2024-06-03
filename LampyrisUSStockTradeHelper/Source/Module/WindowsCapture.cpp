/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: WindowsCapture.cpp
** Desc: ʵ���˹�Ʊ���ٽ��׵Ĵ���
****************************************************************************/
#include "WindowsCapture.h"
#include <QFile>

std::string WindowsCapture::ms_targetClassName;

// "ContainerWnd_DC"
BOOL WindowsCapture::EnumChildWindowProc(HWND hwnd, LPARAM lParam) {
	char className[256];
	LPARAM_INFO* pInfo = reinterpret_cast<LPARAM_INFO*>(lParam); // ��lParam��ȡ���ݵ�����

	GetClassName(hwnd, className, sizeof(className));
	if (strcmp(className, pInfo->targetClassName) == 0) {
		// ���ڴ洢����������
		const int classLen = 256;
		char parentClassName[classLen];
		pInfo->foundWindow = hwnd;
		HWND parent = GetParent(hwnd);
		// ��ȡ�����ڵ�����
		if (GetClassName(parent, className, classLen) == 0) {
			// ��ȡ����ʧ�ܣ����������ﴦ�����
			return TRUE;
		}

		if (strcmp(parentClassName, pInfo->targetClassName) == 0) {
			return FALSE;
		}
	}
	return TRUE; // ��������
}

HWND WindowsCapture::FindWindowByClassName(std::string& className, std::string& parentClassName) {
	HWND hwnd = GetDesktopWindow(); // ��ȡ���洰�ھ��

	LPARAM_INFO data;
	data.targetClassName = className.c_str();
	data.foundWindow = NULL;
	data.parentClassName = parentClassName.c_str();

	EnumChildWindows(hwnd, EnumChildWindowProc, reinterpret_cast<LPARAM>(&data));
	return data.foundWindow;
}

WindowsCapture::WindowsCapture(const std::string& windowClassName,const std::string& windowParentClassName) :
	m_windowClassName(windowClassName), m_windowParentClassName(windowParentClassName), m_hWnd(NULL), m_hWindowDC(NULL), m_hMemoryDC(NULL) {
}

WindowsCapture::~WindowsCapture() {
	::DeleteDC(this->m_hMemoryDC);
	::ReleaseDC(this->m_hWnd, this->m_hWindowDC);
}

bool WindowsCapture::Init() {
	this->m_hWnd = FindWindowByClassName(m_windowClassName,m_windowParentClassName);
	if (!this->m_hWnd) {
		return false;
	}

	this->m_hWindowDC = ::GetWindowDC(this->m_hWnd); // ��ȡ����DC
	this->m_hMemoryDC = ::CreateCompatibleDC(this->m_hWindowDC); // ��������DC

	return true;
}

void WindowsCapture::Capture(const std::string& savePath, float width, float height, float offsetX, float offsetY, Anchor anchor) {
	RECT rect;
	GetClientRect(this->m_hWnd, &rect); // ��ȡ���ڴ�С
	int winWidth = rect.right - rect.left;
	int winHeight = rect.bottom - rect.top;

	HBITMAP hBitmap = CreateCompatibleBitmap(this->m_hWindowDC, winWidth, winHeight); // ����λͼ
	HGDIOBJ oldObj = SelectObject(this->m_hMemoryDC, hBitmap); // ѡ��λͼ��DC

	// ���ƴ������ݵ�λͼ
	BitBlt(this->m_hMemoryDC, 0, 0, winWidth, winHeight, this->m_hWindowDC, 0, 0, SRCCOPY);

	// ��HBITMAPת��ΪQPixmap
	QPixmap pixmap = QtWin::fromHBITMAP(hBitmap);

	// �ͷ���Դ
	SelectObject(this->m_hMemoryDC, oldObj);
	DeleteObject(hBitmap);

	switch (anchor) {
	case Anchor::TopLeft: // ���Ͻ�
		pixmap = pixmap.copy(offsetX, offsetY, width, height);
		break;
	case Anchor::BottomLeft: // ���½�
		pixmap = pixmap.copy(offsetX, winHeight - height - offsetY, width, height);
		break;
	case Anchor::TopRight: // ���Ͻ�
		pixmap = pixmap.copy(winWidth - width - offsetX, offsetY, width, height);
		break;
	case Anchor::BottomRight: // ���½�
		pixmap = pixmap.copy(winWidth - width - offsetX, winHeight - height - offsetY, width, height);
		break;
	}
	
	QFile pngFile(savePath.c_str());
	if (pngFile.exists()) {
		pngFile.remove();
	}
	pixmap.save(QString::fromStdString(savePath));
}

bool WindowsCapture::CheckWindowValid() {
	if (this->m_hWnd == NULL) {
		return false;
	}

	DWORD dwTID = 0;
	DWORD dwPID = NULL;
	dwTID = ::GetWindowThreadProcessId(this->m_hWnd, &dwPID);//��ȡָ�������߳�ID�ͽ���ID

	return dwPID > 0;
}

DWORD WindowsCapture::GetWindowProcessID() {
	if (this->m_hWnd == NULL) {
		return 0;
	}

	DWORD dwTID = 0;
	DWORD dwPID = NULL;
	dwTID = ::GetWindowThreadProcessId(this->m_hWnd, &dwPID);//��ȡָ�������߳�ID�ͽ���ID

	return dwPID;
}
