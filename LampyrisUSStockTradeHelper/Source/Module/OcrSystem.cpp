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

OCR_HANDLE OcrSystem::ms_handle = OcrSystem::Init();

OCR_HANDLE OcrSystem::Init() {
	try {
		OCR_HANDLE handle = ::OcrInit(DET_MODEL, CLS_MODEL, REC_MODEL, KEY_FILE, THREAD_NUM);
		return handle;
	}
	catch (const std::exception& ex) {	
		throw std::exception("OcrSystem::Init() Failed");
		return NULL;
	}
}

OcrSystem::OcrResultList OcrSystem::Detect(const char* imagePath, const  char* imageName, OCR_PARAM* params) {
	OCR_BOOL bRet = ::OcrDetect(ms_handle, imagePath, imageName, params);
	if (bRet) {
		int nLen = ::OcrGetLen(ms_handle);
		if (nLen > 0) {
			char* szInfo = (char*)::malloc(nLen);
			if (szInfo) {
				if (::OcrGetResult(ms_handle, szInfo, nLen)) {
					return Utilities::Split(szInfo, "\n");
				}
				::free(szInfo);
			}
		}
	}
	return OcrResultList();
}

void OcrSystem::DetectNoAlloc(const char* imagePath,const char* imageName,OCR_PARAM* params,OcrResultList& resultList) {
	OCR_BOOL bRet = ::OcrDetect(ms_handle, imagePath, imageName, params);
	if (bRet) {
		int nLen = ::OcrGetLen(ms_handle);
		if (nLen > 0) {
			char* szInfo = (char*)::malloc(nLen);
			if (szInfo) {
				if (::OcrGetResult(ms_handle, szInfo, nLen)) {
					Utilities::SplitNoAlloc(szInfo, "\n", resultList);
				}
				::free(szInfo);
			}
		}
	}
}