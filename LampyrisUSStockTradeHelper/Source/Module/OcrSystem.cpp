/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: OcrSystem.cpp
** Desc: 实现了对图片文件的文字识别功能
****************************************************************************/

#include "OcrSystem.h"
#include "Base/Utilities.h"
#include <exception>

bool OcrSystem::Init() {
	try {
		this->m_handle = ::OcrInit(DET_MODEL, CLS_MODEL, REC_MODEL, KEY_FILE, THREAD_NUM);
		return true;
	}
	catch (const std::exception& ex) {
		return false;
	}
}

OcrSystem::OcrResultList OcrSystem::Detect(OCR_HANDLE handle, const char* imagePath, const  char* imageName, OCR_PARAM* params) {
	OCR_BOOL bRet = ::OcrDetect(handle, imagePath, imageName, params);
	if (bRet) {
		int nLen = ::OcrGetLen(handle);
		if (nLen > 0) {
			char* szInfo = (char*)::malloc(nLen);
			if (szInfo) {
				if (::OcrGetResult(handle, szInfo, nLen)) {
					return Utilities::Split(szInfo, "\n");
				}
				::free(szInfo);
			}
		}
	}
	return OcrResultList();
}