/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: MonoScriptingSystem.cpp
** Desc: 实现了基于Mono框架的C#脚本子系统，支持加载C#程序集，
         调用程序集里定义的函数并获取返回值等功能
****************************************************************************/

#include "BuildinScriptingClass.h"
#include "MonoScriptingSystem.h"
#include <mono/metadata/debug-helpers.h>

void MonoScriptingSystem::Initialize() {
    mono_set_assemblies_path("C:\\Program Files\\Mono\\lib\\");
    // 初始化Mono运行时
    this->m_pMonoDomain = mono_jit_init("MainDomain");

    if (!this->m_pMonoDomain) {
        throw std::runtime_error("Fail to init mono domain");
        return;
    }

    this->SetupInternalCall();
}

void MonoScriptingSystem::Finalize() {
    return;
}

MonoScriptingAssemblyPtr MonoScriptingSystem::LoadAssembly(const std::string& assemblyName) {
    if (m_assemblyMap.contains(assemblyName)) {
        return m_assemblyMap[assemblyName];
    }

    return m_assemblyMap[assemblyName] = MonoScriptingAssembly::LoadFromFilePath(assemblyName);
}

MonoString* MonoScriptingSystem::GetString(const char* string) {
    return mono_string_new(this->m_pMonoDomain, string);
}

MonoScriptingAssemblyPtr MonoScriptingAssembly::LoadFromFilePath(const std::string& path) {
    MonoDomain* pMonoDomain = mono_domain_get();
    MonoAssembly* pMonoAssembly = nullptr;

    if (g_buildInDllMap.contains(path)) { // 如果是mono库中的C# DLL
        pMonoAssembly = mono_domain_assembly_open(pMonoDomain, g_buildInDllMap[path].c_str());
    }
    else {
        if (!std::filesystem::exists(path))
            return nullptr;
        pMonoAssembly = mono_domain_assembly_open(pMonoDomain, path.c_str());
    }

    if (pMonoAssembly == nullptr)
        return nullptr;

    MonoScriptingAssemblyPtr ptr = std::make_shared<MonoScriptingAssembly>();
    ptr->m_pMonoAssembly = pMonoAssembly;
    ptr->m_pMonoImage = mono_assembly_get_image(ptr->m_pMonoAssembly);
    MonoAssemblyName* assemblyName = mono_assembly_get_name(ptr->m_pMonoAssembly);
    ptr->m_assemblyName = mono_assembly_name_get_name(assemblyName);
    return ptr;
}

MonoScriptingClassPtr MonoScriptingAssembly::GetType(const std::string& typeName) {
    return this->GetType(this->m_assemblyName, typeName);
}

MonoScriptingClassPtr MonoScriptingAssembly::GetType(const std::string& namespaceName, const std::string& typeName) {
    if (m_classHandleMap.contains(typeName)) {
        return m_classHandleMap[typeName];
    }

    MonoClass* pMonoClass = mono_class_from_name(this->m_pMonoImage, namespaceName.c_str(), typeName.c_str());
    if (pMonoClass == nullptr) {
        return nullptr;
    }

    MonoScriptingClassPtr handle = std::make_shared<MonoScriptingClass>();
    handle->m_pMonoClass = pMonoClass;
    handle->m_className = typeName;

    m_classHandleMap[typeName] = handle;
    return handle;
}

MonoMethod* MonoScriptingClass::GetMethod(const std::string& methodSignature) {
    if (m_methodHandleMap.contains(methodSignature)) {
        return m_methodHandleMap[methodSignature];
    }
    
    std::string method_desc = this->m_className + ":" + methodSignature;
    MonoMethodDesc* method_desc_obj = mono_method_desc_new(method_desc.c_str(), NULL);
    MonoMethod* method = mono_method_desc_search_in_class(method_desc_obj, this->m_pMonoClass);
    mono_method_desc_free(method_desc_obj);

    m_methodHandleMap[methodSignature] = method;
    return method;
}
