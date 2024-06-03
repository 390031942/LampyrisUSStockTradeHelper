/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: MonoScriptingSystem.h
** Desc: 实现了基于Mono框架的C#脚本子系统，支持加载C#程序集，
         调用程序集里定义的函数并获取返回值等功能 
****************************************************************************/
#pragma once

#include <format>
#include <string>
#include <memory>
#include <unordered_map>
#include <filesystem>
#include <Base/Singleton.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

static void mono_raise_std_exception(MonoObject* exception) {
    MonoString* exStr = mono_object_to_string(exception, nullptr);
    const char* exCStr = mono_string_to_utf8(exStr);
    std::string errorMsg = "Exception thrown: ";
    errorMsg += exCStr;
    mono_free(exception); 
    throw std::runtime_error(errorMsg);
}

class IMonoObjectWrapper {
    virtual MonoObject* ToMonoObject() = 0;
};

template<typename T>
static T mono_extract_value(MonoObject* monoObject) {
    if (!monoObject) throw std::runtime_error("Null MonoObject* provided");
    return *(T*)mono_object_unbox(monoObject);
}

template<>
static std::string mono_extract_value(MonoObject* monoObject) {
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
static T mono_to_native(MonoObject* monoObject) {
    if (!monoObject) {
        throw std::runtime_error("Null MonoObject* provided");
    }

    // 获取MonoObject的类型
    MonoClass* monoClass = mono_object_get_class(monoObject);
    MonoType* monoType = mono_class_get_type(monoClass);
    MonoTypeEnum monoTypeCode = (MonoTypeEnum)mono_type_get_type(monoType);

    // 根据T的类型，确定MonoTypeEnum值
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

    // 检查类型是否匹配
    if (monoTypeCode != expectedTypeCode) {
        throw std::runtime_error("Type mismatch between MonoObject and native type");
    }

    // 从MonoObject提取值
    return mono_extract_value<T>(monoObject);
}

class MonoScriptingClass;
typedef std::shared_ptr<MonoScriptingClass> MonoScriptingClassPtr;

class MonoScriptingAssembly;
typedef std::shared_ptr<MonoScriptingAssembly> MonoScriptingAssemblyPtr;

class MonoScriptingAssembly {
    typedef std::unordered_map<std::string, MonoScriptingClassPtr> ClassHandleMap;
private:
    MonoAssembly*  m_pMonoAssembly;
    MonoImage*     m_pMonoImage;
    ClassHandleMap m_classHandleMap;
    std::string    m_assemblyName;
public:
    static MonoScriptingAssemblyPtr LoadFromFilePath(const std::string& path);

    MonoScriptingClassPtr GetType(const std::string& typeName);

    MonoScriptingClassPtr GetType(const std::string& namespaceName, const std::string& typeName);
};

class MonoScriptingClass {
    typedef std::unordered_map<std::string, MonoMethod*> MethodHandleMap;
private:
    MonoClass*      m_pMonoClass;
    std::string     m_className;
    MethodHandleMap m_methodHandleMap;

public:
    MonoMethod* GetMethod(const std::string& methodSignature);

    MonoClass* GetMonoClass() inline const { return this->m_pMonoClass; }
    
    MonoObject* Invoke(MonoObject* self, const std::string& methodSignature) {
        MonoMethod* method = GetMethod(methodSignature);

        MonoObject* exception = nullptr;
        MonoObject* result = mono_runtime_invoke(method, self, NULL, &exception);
        if (exception != nullptr) {
            mono_raise_std_exception(exception);
        }
        return result;
    }

    template <typename... Args>
    MonoObject* Invoke(MonoObject* self,const std::string& methodSignature,Args... args) {
        MonoMethod* method = GetMethod(methodSignature);

        void* params[] = { &args... };
        MonoObject* exception = nullptr;
        MonoObject* result = mono_runtime_invoke(method, self, params, &exception);
        if (exception != nullptr) {
            mono_raise_std_exception(exception);
        }
        return result;
    }
    
    MonoObject* New() {
        MonoObject * obj = mono_object_new(mono_domain_get(), this->m_pMonoClass);
        return obj;
    }

    template<typename ...Args>
    MonoObject* New(const std::string& signature,Args... args) {
        return this->Invoke(nullptr, signature,args...);
    }

    MonoObject* GetProperty(MonoObject* self, const std::string& propertyName) {
        MonoProperty* property = mono_class_get_property_from_name(this->m_pMonoClass, propertyName.c_str());
        if (property == nullptr) {
            return nullptr;
        }

        // 获取属性的get方法
        MonoMethod* getter = mono_property_get_get_method(property);
        if (getter == nullptr) {
            return nullptr;
        }

        // 调用get方法获取属性值
        MonoObject* exception = nullptr;
        MonoObject* result = mono_runtime_invoke(getter, self, nullptr, &exception);
        if (exception != nullptr) {
            mono_raise_std_exception(exception);
        }
        return result;
    }

    template<typename T>
    void SetProperty(MonoObject* self, const std::string& propertyName, T* value) {
        MonoProperty* property = mono_class_get_property_from_name(this->m_pMonoClass, propertyName.c_str());
        if (property == nullptr) {
            return nullptr;
        }

        // 获取属性的set方法
        MonoMethod* setter = mono_property_get_set_method(property);
        if (setter == nullptr) {
            return;
        }

        void* args[1];
        args[0] = mono_value_box(mono_domain_get(), mono_get_int32_class(), value);

        // 调用set方法设置属性值
        MonoObject* exception = nullptr;
        mono_runtime_invoke(setter, self, args, &exception);

        if (exception != nullptr) {
            mono_raise_std_exception(exception);
        }
    }

    MonoObject* GetField(MonoObject* self, const std::string& fieldName) {
        MonoClassField* field = mono_class_get_field_from_name(this->m_pMonoClass, fieldName.c_str());
        MonoObject* object;
        mono_field_set_value(self, field, &object);
    }

    template<typename T>
    void SetField(MonoObject* self, const std::string& fieldName, T* value) {
        MonoClassField* field = mono_class_get_field_from_name(this->m_pMonoClass, fieldName.c_str());
        T value;
        mono_field_get_value(self, field, &value);
        return value;
    }

    friend class MonoScriptingAssembly;
};

class MonoScriptingSystem:public Singleton<MonoScriptingSystem> {
    typedef std::unordered_map<std::string, MonoScriptingAssemblyPtr> AssemblyMap;
private:
    MonoDomain*              m_pMonoDomain;
    MonoAssembly*            m_pMonoAsm;
    AssemblyMap              m_assemblyMap;

    void                     SetupInternalCall();
public:                      
    void                     Initialize();
                             
    void                     Finalize();
                          
    MonoScriptingAssemblyPtr LoadAssembly(const std::string& assemblyName);
                             
    MonoString*              GetString(const char* string);
};