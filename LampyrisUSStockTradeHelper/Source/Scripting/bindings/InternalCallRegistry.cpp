/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: InternalCallRegistry.cpp
** Desc: C++函数注册
****************************************************************************/
#pragma once
#include "InternalCallRegistry.h"
#include <Scripting/MonoScriptingSystem.h>
#include <Module/USmartTradeSystem.h>

// 需要注册的C++函数，与C#方法匹配
static MonoObject* GetDefaultRequestHeadersWrapper() {
    MonoDomain* domain = mono_domain_get();
    // 获取C++实现的函数结果
    std::unordered_map<std::string, std::string> headers = USmartTradeSystem::GetInstance()->GetHttpDefaultRequestHeader();

    auto genericHelperClass = MonoScriptingSystem::GetInstance()->LoadAssembly("LampyrisUSStockTradeHelper.Managed.dll")->GetType("GenericHelper");
    auto mscorlibAsm = MonoScriptingSystem::GetInstance()->LoadAssembly("mscorlib");
    auto stringKeyType = genericHelperClass->Invoke(NULL, "GetStringKeyValueType");
    auto stringKeyTypeClass = mono_object_get_class(stringKeyType);
    auto array = mono_array_new(domain, stringKeyTypeClass, headers.size());
   
    // 循环添加键值对到List
    int index = 0;
    for (const auto& header : headers) {
        MonoString* stringKey = mono_string_new(domain, header.first.c_str());
        MonoString* stringValue = mono_string_new(domain, header.second.c_str());

        mono_array_set(array, MonoObject*, index++, genericHelperClass->Invoke(NULL, "MakeStringKeyValuePair", stringKey, stringValue));
    }

    return (MonoObject*)array;
}

void MonoScriptingSystem::SetupInternalCall() {
    auto defaultRequestHeaderClass = LoadAssembly("LampyrisUSStockTradeHelper.Managed.dll")->GetType("DefaultRequestHeader");
    mono_add_internal_call("LampyrisUSStockTradeHelper.Managed.DefaultRequestHeader::GetDefaultRequestHeaders", (const void*)GetDefaultRequestHeadersWrapper);
}
