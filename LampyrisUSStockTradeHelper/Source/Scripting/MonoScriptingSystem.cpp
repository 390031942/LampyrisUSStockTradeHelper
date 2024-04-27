/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: MonoScriptingSystem.cpp
** Desc: 实现了基于Mono框架的C#脚本子系统，支持加载C#程序集，
         调用程序集里定义的函数并获取返回值等功能
****************************************************************************/

#include "MonoScriptingSystem.h"

void MonoScriptingSystem::Initialize() {
    // 初始化Mono运行时
    this->m_pMonoDomain = mono_jit_init("MainDomain");

    if (!this->m_pMonoDomain) {
        throw std::runtime_error("Fail to init mono domain");
        return;
    }
}

void MonoScriptingSystem::Finalize() {
    return;
}

bool MonoScriptingSystem::LoadAssembly(const std::string& assemblyName) {
    // 加载C# DLL
    m_pMonoAsm = mono_domain_assembly_open(this->m_pMonoDomain, assemblyName.c_str());

    if (!this->m_pMonoAsm) {
        return false;
    }
    return false;
}

MonoScriptingClassPtr MonoScriptingSystem::GetType(const std::string& typeName) {
    if (m_classHandleMap.contains(typeName)) {
        return m_classHandleMap[typeName];
    }

    MonoClass* hClass = mono_class_from_name(this->m_pMonoImage, "", typeName.c_str());
    if (hClass != nullptr) {
        return nullptr;
    }

    MonoScriptingClassPtr handle = std::make_shared<MonoScriptingClass>();
    handle->m_hClassHandle = hClass;
    handle->m_className = typeName;

    m_classHandleMap[typeName] = handle;
    return handle;
}
