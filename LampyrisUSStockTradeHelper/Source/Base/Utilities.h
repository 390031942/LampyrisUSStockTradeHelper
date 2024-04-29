/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: Utilities.h
** Desc: 实用工具类
****************************************************************************/
#pragma once
#include <vector>
#include <string>

namespace Utilities {
    // Split函数，将字符串str根据给定的分隔符delimiter分割，返回一个包含分割后的字符串的vector
    std::vector<std::string> Split(const std::string& str, const std::string& delimiter);
}