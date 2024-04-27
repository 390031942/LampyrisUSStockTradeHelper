/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: OcrSystem.h
** Desc: ʵ���˶�ͼƬ�ļ�������ʶ����
****************************************************************************/
#pragma once
#include <vector>
#include <string>

namespace Utilities {
    // Split���������ַ���str���ݸ����ķָ���delimiter�ָ����һ�������ָ����ַ�����vector
    std::vector<std::string> Split(const std::string& str, const std::string& delimiter);
}