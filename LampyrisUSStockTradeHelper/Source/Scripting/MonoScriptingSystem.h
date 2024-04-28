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

class MonoScriptingClass {
    typedef std::unordered_map<std::string, MonoMethod*> MethodHandleMap;
private:
    MonoDomain*     m_pMonoDomain;
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
        if (exception != nullptr) {
            mono_raise_std_exception(exception);
        }
        return result;
    }
    
    MonoObject* New() {
        MonoObject * obj = mono_object_new(this->m_pMonoDomain, this->m_hClassHandle);
        return obj;
    }

    template<typename ...Args>
    MonoObject* New(const std::string& signature,Args... args) {
        return this->Invoke(nullptr, signature,args);
    }

    MonoObject* GetProperty(MonoObject* self, const std::string& propertyName) {
        MonoProperty* property = mono_class_get_property_from_name(this->m_hClassHandle, propertyName.c_str());
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
        MonoProperty* property = mono_class_get_property_from_name(this->m_hClassHandle, propertyName.c_str());
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
        MonoClassField* field = mono_class_get_field_from_name(this->m_hClassHandle, fieldName.c_str());
        MonoObject* object;
        mono_field_set_value(self, field, &object);
    }

    template<typename T>
    void SetField(MonoObject* self, const std::string& propertyName, T* value) {
        MonoClassField* field = mono_class_get_field_from_name(this->m_hClassHandle, fieldName.c_str());
        T value;
        mono_field_get_value(obj_instance, field, &value);
        return value;
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

    MonoScriptingClassPtr GetType(const std::string& namespaceName,const std::string& typeName);

    MonoString*           GetString(const char* string);
};