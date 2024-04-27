/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: MonoScriptingSystem.h
** Desc: ʵ���˻���Mono��ܵ�C#�ű���ϵͳ��֧�ּ���C#���򼯣�
         ���ó����ﶨ��ĺ�������ȡ����ֵ�ȹ��� 
****************************************************************************/

#pragma once

#include <format>
#include <string>
#include <memory>
#include <unordered_map>
#include <Base/Singleton.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

template<typename T>
T mono_extract_value(MonoObject* obj) {
    throw std::runtime_error("Unsupported type");
}

template<>
std::string mono_extract_value(MonoObject* monoObject) {
    if (!monoObject) {
        throw std::runtime_error("Null MonoObject* provided");
    }

    MonoString* monoString = mono_object_to_string(monoObject, nullptr);
    if (!monoString) {
        throw std::runtime_error("Conversion to string failed");
    }

    return mono_string_to_utf8(monoString);
}

template<typename T>
T mono_to_native(MonoObject* monoObject) {
    if (!monoObject) {
        throw std::runtime_error("Null MonoObject* provided");
    }

    // ��ȡMonoObject������
    MonoClass* monoClass = mono_object_get_class(monoObject);
    MonoType* monoType = mono_class_get_type(monoClass);
    MonoTypeEnum monoTypeCode = mono_type_get_type(monoType);

    // ����T�����ͣ�ȷ��MonoTypeEnumֵ
    MonoTypeEnum expectedTypeCode;
    if constexpr (std::is_same_v<T, int>) {
        expectedTypeCode = MONO_TYPE_I4;
    }
    else if constexpr (std::is_same_v<T, bool>) {
        expectedTypeCode = MONO_TYPE_BOOLEAN;
    }
    else if constexpr (std::is_same_v<T, float>) {
        expectedTypeCode = MONO_TYPE_R4;
    }
    else if constexpr (std::is_same_v<T, double>) {
        expectedTypeCode = MONO_TYPE_R8;
    }
    else if constexpr (std::is_same_v<T, char>) {
        expectedTypeCode = MONO_TYPE_CHAR;
    }
    else if constexpr (std::is_same_v<T, std::string>) {
        expectedTypeCode = MONO_TYPE_STRING;
    }
    else {
        throw std::runtime_error("Unsupported conversion type");
    }

    // ��������Ƿ�ƥ��
    if (monoTypeCode != expectedTypeCode) {
        throw std::runtime_error("Type mismatch between MonoObject and native type");
    }

    // ��MonoObject��ȡֵ
    return mono_extract_value<T>(monoObject);
}

class MonoScriptingClass {
    typedef std::unordered_map<std::string, MonoMethod*> MethodHandleMap;
private:
    MonoClass*      m_hClassHandle;
    std::string     m_className;
    MethodHandleMap m_methodHandleMap;

    MonoMethod* GetMethod(const std::string& methodSignature) {
        if (m_methodHandleMap.contains(methodSignature)) {
            return m_methodHandleMap[methodSignature];
        }

        std::string method_desc = this->m_className + ":" + methodSignature;
        MonoMethodDesc* method_desc_obj = mono_method_desc_new(method_desc.c_str(), NULL);
        MonoMethod* method = mono_method_desc_search_in_class(method_desc_obj, this->m_hClassHandle);
        mono_method_desc_free(method_desc_obj);

        m_methodHandleMap[methodSignature] = method; 
        return method;
    }
public:
    template <typename... Args>
    MonoObject* Invoke(MonoObject* self,const std::string& methodSignature,Args... args) {
        MonoMethod* method = GetMethod(methodSignature);

        void* params[] = { &args... };
        MonoObject* exception = nullptr;
        MonoObject* result = mono_runtime_invoke(method, self, params, &exception);
        if (exception) {
            MonoString* exStr = mono_object_to_string(exception, nullptr);
            const char* exCStr = mono_string_to_utf8(exStr);
            std::string errorMsg = "Exception thrown: ";
            errorMsg += exCStr;
            mono_free(exCStr);
            throw std::runtime_error(errorMsg);
        }
        return result;
    }
    
    friend class MonoScriptingSystem;
};

typedef std::shared_ptr<MonoScriptingClass> MonoScriptingClassPtr;

class MonoScriptingSystem:public Singleton<MonoScriptingSystem> {
    typedef std::unordered_map<std::string, MonoScriptingClassPtr> ClassHandleMap;
private:
    MonoDomain*           m_pMonoDomain;
    MonoAssembly*         m_pMonoAsm;
    MonoImage*            m_pMonoImage;
    ClassHandleMap        m_classHandleMap;
public:                   
    void                  Initialize();

    void                  Finalize();
                          
    bool                  LoadAssembly(const std::string& assemblyName);

    MonoScriptingClassPtr GetType(const std::string& typeName);
};