/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: SignatureMaker.h
** Desc: 通过变参模板参数构造函数签名字符串
****************************************************************************/

#include <string>
#include <exception>

template<typename T>
static const char* GetTypeName() {
    throw std::runtime_error("Unspported Type:" + typeid(T).name);
}

template<>
static const char* GetTypeName<int>() {
    return "int";
}

template<>
static const char* GetTypeName<bool>() {
    return "bool";
}

template<>
static const char* GetTypeName<float>() {
    return "float";
}

template<>
static const char* GetTypeName<double>() {
    return "double";
}

template<>
static const char* GetTypeName<const char*>() {
    return "string";
}

template<>
static const char* GetTypeName<std::string>() {
    return "string";
}

#define DELCARE_CPP_TO_MONO_TYPE(type) \
    template<> \
     static const char* GetTypeName<type>() { \
        return ##type; \
    } 


std::string GetTypeNames() {
    return "";
}

// 递归解包变参模板，并获取类型名称
template<typename T, typename... Args>
std::string GetTypeNames() {
    std::string typeName = GetTypeName<T>();
    if constexpr (sizeof...(Args) > 0) {
        return typeName + "," + GetTypeNames<Args...>();
    }
    else {
        return typeName;
    }
}

template<typename... Args>
std::string ExtractTypeName(Args... args) {
    std::string types = "(" + GetTypeNames<Args...>() + ")";
    return types;
}
