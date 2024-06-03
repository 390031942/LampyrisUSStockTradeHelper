/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: Program.cs
** Desc: 实现了C#程序集元数据扫描并输出为Mono框架可用的C++代码的功能。
****************************************************************************/

using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Mono.Cecil;

namespace CSharpBindingCodeGenerator
{
    public class NamespaceInfo
    {
        public List<ClassInfo> classInfoList = new List<ClassInfo>();

        public List<EnumInfo> enumInfoList = new List<EnumInfo>();
    }

    public class DLLInfo
    {
        public string name;
        
        public string identifyName;

        public string dllRelativePath;

        public Dictionary<string,NamespaceInfo> namespaces = new Dictionary<string, NamespaceInfo> ();
    }

    public class ClassInfo
    {
        public string Name { get; set; }
        public string NamespaceName { get; set; }

        public List<ConstructorInfo> Constructors { get; set; }
        public List<MethodInfo> Methods { get; set; }
        public List<PropertyInfo> Properties { get; set; }
        public List<FieldInfo> Fields { get; set; }

        public ClassInfo()
        {
            Constructors = new List<ConstructorInfo>();
            Methods = new List<MethodInfo>();
            Properties = new List<PropertyInfo>();
            Fields = new List<FieldInfo>();
        }
    }

    public class EnumInfo
    {
        public string Name { get; set; }
        public string NamespaceName { get; set; }

        public Dictionary<string, long> Values { get; set; }

        public EnumInfo()
        {
            Values = new Dictionary<string, long>();
        }
    }

    public class ConstructorInfo
    {
        public List<TypeReference> Parameters { get; set; }

        public ConstructorInfo()
        {
            Parameters = new List<TypeReference>();
        }
    }

    public class MethodInfo
    {
        public string Name { get; set; }
        public List<TypeReference> Parameters { get; set; }
        public TypeReference ReturnType;

        public MethodInfo()
        {
            Parameters = new List<TypeReference>();
        }
    }

    public class FieldInfo
    {
        public string Name { get; set; }
        public string Type { get; set; }
    }

    public class PropertyInfo
    {
        public string Name { get; set; }
        public string Type { get; set; }

        public bool hasPublicGetter;

        public bool hasPublicSetter;
    }

    public class DLLScaner
    {
        List<DLLInfo> m_dllInfoList = new List<DLLInfo> ();

        public void ScanSingleAssembly(string assemblyPath)
        { 
            var assembly = AssemblyDefinition.ReadAssembly(assemblyPath);
            var dllInfo = new DLLInfo()
            {
                name = assembly.Name.Name,
            };

            foreach (var type in assembly.MainModule.Types)
            {
                if (type.IsPublic)
                {
                    if (!dllInfo.namespaces.ContainsKey(type.Namespace))
                    {
                        dllInfo.namespaces[type.Namespace] = new NamespaceInfo();
                    }
                    if (type.IsClass && !type.IsEnum)
                    {
                        var classInfo = ExtractClassInfo(type);
                        if(classInfo != null)
                        {
                            dllInfo.namespaces[type.Namespace].classInfoList.Add(classInfo);
                        }
                    }
                    else if (type.IsEnum)
                    {
                        var enumInfo = ExtractEnumInfo(type);
                        dllInfo.namespaces[type.Namespace].enumInfoList.Add(enumInfo);
                    }
                }
            }

            m_dllInfoList.Add(dllInfo);
        }

        private ClassInfo ExtractClassInfo(TypeDefinition type)
        {
            var classInfo = new ClassInfo { Name = type.Name,NamespaceName = type.Namespace };

            foreach (var method in type.Methods)
            {
                if (method.IsPublic)
                {
                    if(method.Name != ".cctor")
                    {
                        if (method.Name == ".ctor")
                        {
                            var constructorInfo = new ConstructorInfo();
                            foreach (var parameter in method.Parameters)
                            {
                                constructorInfo.Parameters.Add(parameter.ParameterType);
                            }
                        }
                        else
                        {
                            var methodInfo = new MethodInfo { Name = method.Name };
                            foreach (var parameter in method.Parameters)
                            {
                                methodInfo.Parameters.Add(parameter.ParameterType);
                            }
                            methodInfo.ReturnType = method.ReturnType;
                        }
                    }
                }
            }

            // 提取public属性
            foreach (var property in type.Properties)
            {
                bool hasGetter = property.GetMethod != null && property.GetMethod.IsPublic;
                bool hasSetter = property.SetMethod != null && property.SetMethod.IsPublic;
                if (hasGetter || hasSetter)
                {
                    classInfo.Properties.Add(new PropertyInfo
                    {
                        Name = property.Name,
                        Type = property.PropertyType.FullName,
                        hasPublicGetter = hasGetter,
                        hasPublicSetter = hasSetter,
                    });
                }
            }

            // 提取public字段
            foreach (var field in type.Fields)
            {
                if (field.IsPublic)
                {
                    classInfo.Fields.Add(new FieldInfo
                    {
                        Name = field.Name,
                        Type = field.FieldType.FullName
                    });
                }
            }

            return classInfo;
        }

        private EnumInfo ExtractEnumInfo(TypeDefinition type)
        {
            var enumInfo = new EnumInfo { Name = type.Name, NamespaceName = type.Namespace };

            foreach (var field in type.Fields)
            {
                if (field.IsPublic && field.IsStatic)
                {
                    var value = Convert.ToInt64(field.Constant);
                    enumInfo.Values.Add(field.Name, value);
                }
            }

            return enumInfo;
        }

        public void ScanAllDlls(List<string> dllFilePaths)
        {
            foreach (string dllFilePath in dllFilePaths)
            {
                ScanSingleAssembly(dllFilePath);
            };
        }
    }

    public class CodeGeneratorSimple
    {
        private StringBuilder m_sb = new StringBuilder();

        public void GenerateHeader()
        {
            m_sb.Append
            (
                "/****************************************************************************\n" +
                "**\n" +
                "** Copyright (C) 2024 HongJin Investment Cooperation\n" +
                "** \n" +
                "** File: csharp_bindings.gen.h\n" +
                "** Desc: THIS IS ALL AUTO-GENERATED, PLEASE DO NOT MODIFY IT\n" +
                "****************************************************************************/\n" +
                "#pragma once\n\n\n" +
                "class CSharpClassBase {\n" +
                "protected:\n" +
                "    MonoObject* m_monoObject;\n" +
                "public:\n" +
                "    MonoObject* PPtr() inline { return m_monoObject;}\n" +
                "};\n"
            );
        }
    }

    public class CodeGenerator
    {
        private StringBuilder m_sb = new StringBuilder();

        private StringBuilder m_tempSb = new StringBuilder();

        private static Dictionary<string, string> ms_csharp2cppTypeMap = new Dictionary<string, string>() {
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
            {"System.Object", "MonoObject*"} // 通用指针，具体类型应根据实际对象确定
        };

        public CodeGenerator(List<DLLInfo> dllInfos)
        {

        }

        public void GenerateHeader()
        {
            m_sb.Append
            (
                "/****************************************************************************\n" +
                "**\n" +
                "** Copyright (C) 2024 HongJin Investment Cooperation\n" +
                "** \n" +
                "** File: csharp_bindings.gen.h\n" +
                "** Desc: THIS IS ALL AUTO-GENERATED, PLEASE DO NOT MODIFY IT\n" +
                "****************************************************************************/\n" +
                "#pragma once\n\n\n" + 
                "class CSharpClassBase {\n" + 
                "protected:\n" + 
                "    MonoObject* m_monoObject;\n" + 
                "public:\n" + 
                "    MonoObject* PPtr() inline { return m_monoObject;}\n" + 
                "};\n"
            );
        }

        public void GenerateForwardDeclareForSingleAssembly(DLLInfo dllInfo)
        {
            m_sb.AppendLine
            (
                $"// Begin of Assembly \"{dllInfo.name}\"\n"
            );

            m_sb.Append(
               $"namespace Assembly::{dllInfo.identifyName} {{" + 
                "static MonoAssembly* GetAssembly() {\n" +
                "    static MonoAssembly* assembly = nullptr;\n" +
                "    if(assembly == null) {\n" +
                "        MonoDomain* domain = mono_domain_get();\n" +
               $"        assembly = mono_domain_assembly_open(domain, {dllInfo.name}.dll);\n" + 
                "    }\n" +
                "}\n\n" +
                "static MonoImage* GetImage() {\n" +
                "    static MonoImage* image = nullptr;\n" +
                "    if(image == null) {\n" +
                "        image = mono_assembly_get_image(GetAssembly());\n" +
                "    }\n" +
                "    return image;" + 
                "}\n" + 
                "}\n"
            );

            foreach (var kvp in dllInfo.namespaces)
            {
                var namespaceName = kvp.Key;
                var namespaceInfo = kvp.Value;

                namespaceName = namespaceName.Replace(".", "::");
                m_sb.AppendLine("namespace " + namespaceName + " {\n");
                {
                    // 类的前向声明
                    foreach (var classInfo in namespaceInfo.classInfoList)
                    {
                        m_sb.AppendLine("class " + classInfo.Name + ";");
                        m_sb.AppendLine("typedef std::shared_ptr<" + classInfo.Name + ">" + classInfo.Name + "Ptr");
                    }

                    // 枚举类定义
                    foreach (var enumInfo in namespaceInfo.enumInfoList)
                    {
                        GenerateEnum(namespaceName, enumInfo);
                    }
                }
                m_sb.AppendLine($"}}; // end of namespace {namespaceName}\n");

                m_sb.AppendLine("namespace " + namespaceName + " {\n");
                {
                    foreach (var classInfo in namespaceInfo.classInfoList)
                    {
                        GenerateClass(namespaceName, dllInfo.identifyName, classInfo);
                    }
                }
                m_sb.AppendLine($"}}; // end of namespace {namespaceName}\n");
            }

            m_sb.AppendLine
            (
                $"// End of Assembly \"{dllInfo.name}\""
            );
        }

        public void GenerateForwardDeclareForSingleClass(string namespaceName,ClassInfo classInfo)
        {
            m_sb.AppendLine("class " + classInfo.Name + ";");
        }

        public void GenerateEnum(string namespaceName, EnumInfo enumInfo)
        {
            m_sb.AppendLine("enum " + enumInfo.Name + "{");
            foreach(var kvp in enumInfo.Values)
            {
                m_sb.AppendLine($"    {kvp.Key} = {kvp.Value},");
            }
            m_sb.AppendLine("};\n");
        }

        public string GetSignatureString(List<TypeReference> parameterList)
        {
            m_tempSb.Clear();
            for (int i = 0; i < parameterList.Count; i++)
            {
                TypeReference type = parameterList[i];
                string typeName = "";
                if (type.HasGenericParameters || type.IsGenericInstance || type.IsArray)
                {
                    typeName = "MonoObject*";
                }
                else
                {
                    if (ms_csharp2cppTypeMap.ContainsKey(type.FullName))
                    {
                        typeName = ms_csharp2cppTypeMap[typeName];
                    }
                    else
                    {
                        typeName = type.FullName.Replace(".", "::") + "*";
                    }
                }
                
                m_tempSb.Append(typeName);
                if(i < parameterList.Count - 1)
                {
                    m_sb.Append(",");
                }
            }

            return m_tempSb.ToString();
        }

        public void GenerateClass(string namespaceName, string dllIdentifyName, ClassInfo classInfo)
        {
            m_sb.Append
            (
                "class " + classInfo.Name + ":public CSharpClassBase {\n" +
                "public:\n" +
                "    static MonoClass* Class() {\n" +
                "        static MonoClass* classObject = nullptr;" +
                "        if(!classObject) {\n" +
               $"            classObject = mono_class_from_name({namespaceName}, {classInfo}, typeName.c_str());;" +
                "        }\n" +
                "        return classObject;" +
                "    }\n"
            ); ;

            for (int i = 0; i < classInfo.Constructors.Count; i++)
            {
                if (classInfo.Constructors[i].Parameters.Count == 0)
                {
                    m_sb.Append
                    (
                        $"        static {classInfo.Name}Ptr New() {{\n" +
                        $"            {classInfo.Name}Ptr ptr = std::make_shared<{classInfo.Name}>()';\n" +
                        $"            ptr.m_monoObject = mono_object_new(Assembly::{dllIdentifyName}()::GetImage(),GetClass());\n" +
                         "        }"
                    );
                }
                else
                {
                     m_sb.Append
                    (
                        $"        static {classInfo.Name}Ptr New({GetSignatureString(classInfo.Constructors[i].Parameters)}) {{\n" +
                        $"            {classInfo.Name}Ptr ptr = std::make_shared<{classInfo.Name}>()';\n" +
                        $"            ptr.m_monoObject = mono_object_new(Assembly::{dllIdentifyName}()::GetImage(),GetClass());\n" +
                         "        }"
                    );
                }
            }
            for (int i = 0; i < classInfo.Properties.Count; i++)
            {
                m_sb.Append
                (
                    $"         {classInfo.Name}Ptr New(" + GetSignatureString(classInfo.Constructors[i].Parameters) + ") { \n" + 
                     "         }\n"
                ); ;
            }
            m_sb.AppendLine("}; // end of class {}");
        }
    }

    public class Program
    {
        static void Main(String[] args)
        {
            DLLScaner dLLScaner = new DLLScaner();
            dLLScaner.ScanSingleAssembly(@"D:\SourceCode\LampyrisUSStockTradeHelper\LampyrisUSStockTradeHelper\LampyrisUSStockTradeHelper.Managed.dll");
        }
    }
}