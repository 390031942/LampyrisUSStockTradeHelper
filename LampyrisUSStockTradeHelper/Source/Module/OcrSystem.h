/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: OcrSystem.h
** Desc: 实现了对图片文件的文字识别功能
****************************************************************************/
#pragma once

#include <string>
#include <vector>
#include <OcrLib/OcrLiteCApi.h>

#define DET_MODEL   "models/ch_PP-OCRv3_det_infer.onnx"
#define CLS_MODEL   "models/ch_ppocr_mobile_v2.0_cls_infer.onnx"
#define REC_MODEL   "models/ch_PP-OCRv3_rec_infer.onnx"
#define KEY_FILE    "models/ppocr_keys_v1.txt"
#define THREAD_NUM  3

class OcrSystem {
public:
	typedef std::vector<std::string> OcrResultList;
private:
	static OCR_HANDLE    ms_handle;

	static OCR_HANDLE    Init();
public:
	static OcrResultList Detect(const char* imagePath, 
		                        const char* imageName, 
		                        OCR_PARAM*  params);

	static void          DetectNoAlloc(const char* imagePath, 
		                               const char* imageName, 
		                               OCR_PARAM*  params,
		                               OcrResultList& resultList);
};