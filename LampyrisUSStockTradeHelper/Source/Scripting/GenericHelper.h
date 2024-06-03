/****************************************************************************
**
** Copyright(C) 2024 HongJin Investment Cooperation
* *Contact: gameta@qq.com
* *
** File: GenericHelper.h
** Desc : C#泛型实用工具类
* ***************************************************************************/
#pragma once
#include <string>
#include <unordered_map>
#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/class.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/exception.h>
#include <mono/metadata/reflection.h>
#include <Scripting/MonoScriptingSystem.h>

class GenericHelper
{
public:
    static MonoObject* MakeStringKeyValuePair(const char* key,const char* value)
    {
        auto domain = mono_domain_get();
        auto genericHelperType = MonoScriptingSystem::GetInstance()->LoadAssembly("LampyrisUSStockTradeHelper.Managed.dll")->GetType("GenericHelper");
        if (genericHelperType) {
            MonoString* stringKey = mono_string_new(domain, key);
            MonoString* stringValue = mono_string_new(domain, value);
            return genericHelperType->Invoke(NULL, "MakeStringKeyValuePair", stringKey, stringValue);
        }

        return nullptr;
    }
};