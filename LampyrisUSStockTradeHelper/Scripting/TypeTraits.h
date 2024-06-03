/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: SignatureMaker.h
** Desc: ͨ�����ģ��������캯��ǩ���ַ���
****************************************************************************/
#pragma once
#include<string>
#include<unordered_map>

namespace Scripting {
    static std::unordered_map<std::string, std::string> g_typeMapping = {
        {"System.Boolean", "bool"},
        {"System.Byte", "unsigned char"},
        {"System.SByte", "signed char"},
        {"System.Char", "char"},
        {"System.Decimal", "long double"},
        {"System.Double", "double"},
        {"System.Single", "float"},
        {"System.Int32", "int"},
        {"System.UInt32", "unsigned int"},
        {"System.Int64", "long long"},
        {"System.UInt64", "unsigned long long"},
        {"System.Int16", "short"},
        {"System.UInt16", "unsigned short"},
        {"System.String", "const char*"},
        {"System.IntPtr", "intptr_t"},
        {"System.UIntPtr", "uintptr_t"},
        {"System.Object", "MonoObject*"} // ͨ��ָ�룬��������Ӧ����ʵ�ʶ���ȷ��
    };
}