/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: MonoScriptingSystem.cpp
** Desc: ʵ���˻���Mono��ܵ�C#�ű���ϵͳ��֧�ּ���C#���򼯣�
         ���ó����ﶨ��ĺ�������ȡ����ֵ�ȹ���
****************************************************************************/

#include "MonoScriptingSystem.h"
#include <mono/metadata/debug-helpers.h>

void MonoScriptingSystem::Initialize() {
    mono_set_assemblies_path("C:\\Program Files\\Mono\\lib\\");
    // ��ʼ��Mono����ʱ
    this->m_pMonoDomain = mono_jit_init("domain");

    if (!this->m_pMonoDomain) {
        throw std::runtime_error("Fail to init mono domain");
        return;
    }
}

void MonoScriptingSystem::Finalize() {
    return;
}

bool MonoScriptingSystem::LoadAssembly(const std::string& assemblyName) {
    // ����C# DLL
    m_pMonoAsm = mono_domain_assembly_open(this->m_pMonoDomain, assemblyName.c_str());

    if (!this->m_pMonoAsm) {
        return false;
    }

    m_pMonoImage = mono_assembly_get_image(m_pMonoAsm);

    return true;
}

MonoScriptingClassPtr MonoScriptingSystem::GetType(const std::string& namespaceName,const std::string& typeName) {
    if (m_classHandleMap.contains(typeName)) {
        return m_classHandleMap[typeName];
    }

    MonoClass* hClass = mono_class_from_name(this->m_pMonoImage, namespaceName.c_str(), typeName.c_str());
    if (hClass == nullptr) {
        return nullptr;
    }

    MonoScriptingClassPtr handle = std::make_shared<MonoScriptingClass>();
    handle->m_hClassHandle = hClass;
    handle->m_className = typeName;
    handle->m_pMonoDomain = m_pMonoDomain;

    m_classHandleMap[typeName] = handle;
    return handle;
}

MonoString* MonoScriptingSystem::GetString(const char* string) {
    return mono_string_new(this->m_pMonoDomain, string);
}