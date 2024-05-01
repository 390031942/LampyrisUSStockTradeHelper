/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: MonoScriptingSystem.h
** Desc: 实现了基于Mono框架的C#脚本子系统，支持加载C#程序集，
         调用程序集里定义的函数并获取返回值等功能 
****************************************************************************/

using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;
using Mono.Cecil;

namespace CSharpBindingCodeGenerator
{
    public class DLLInfo
    {
        public string DLLPath;
        public string DLLName { get; set; }
        public List<ClassInfo> Classes { get; set; } = new List<ClassInfo>();

        public HashSet<string> Dependencies = new HashSet<string>();
    }

    public class ClassInfo
    {
        public string ClassName { get; set; }
        public List<string> ConstructorSignatures { get; set; } = new List<string>();
    }

    public class DLLScaner
    {
        List<DLLInfo> m_dllInfoList = new List<DLLInfo> ();

        public void Scan(string dllPath)
        {
            Assembly assembly = Assembly.ReflectionOnlyLoadFrom(dllPath);

            DLLInfo dllInfo = new DLLInfo
            {
                DLLName = System.IO.Path.GetFileNameWithoutExtension(dllPath),
                DLLPath = dllPath,
            };

            m_dllInfoList.Add(dllInfo);

            // 获取依赖项
            AssemblyName[] referencedAssemblies = assembly.GetReferencedAssemblies();

            foreach(AssemblyName assemblyName in referencedAssemblies)
            {
                dllInfo.Dependencies.Add(assemblyName.Name);
            }
        }

        public void Rescan()
        {
            foreach (DLLInfo dllInfo in m_dllInfoList)
            {
                if (dllInfo.DLLName != "System.Net.Http")
                    continue;

                // 使用Mono.Cecil加载DLL
                var assemblyDefinition = AssemblyDefinition.ReadAssembly(dllInfo.DLLPath);

                // 遍历DLL中的所有类型
                foreach (var typeDefinition in assemblyDefinition.MainModule.Types)
                {
                    Console.WriteLine($"Class: {typeDefinition.FullName}");

                    // 遍历类型中的所有属性
                    foreach (var property in typeDefinition.Properties)
                    {
                        Console.WriteLine($"Property: {property.Name} Type: {property.PropertyType.Name}");
                    }

                    // 遍历类型中的所有构造函数
                    foreach (var method in typeDefinition.Methods)
                    {
                        if (method.IsConstructor)
                        {
                            Console.Write($"Constructor: {method.Name}(");
                            // 获取并打印构造函数的参数类型
                            for (int i = 0; i < method.Parameters.Count; i++)
                            {
                                var param = method.Parameters[i];
                                Console.Write(param.ParameterType.FullName);
                                if (i < method.Parameters.Count - 1)
                                {
                                    Console.Write(", ");
                                }
                            }
                            Console.WriteLine(")");
                        }
                    }
                }
            }
        }

        public void Sort()
        {
            m_dllInfoList.Sort((a, b) =>
            {
                if (a.Dependencies.Contains(b.DLLName))
                    return 1;

                if (b.Dependencies.Contains(a.DLLName))
                    return -1;

                return 0;
            });
        }
    }

    public class Program
    {
        static void Main(string[] args)
        {
            // 指定需要扫描的文件夹路径
            string folderPath = @"C:\Program Files\Mono\lib\mono\4.7.2-api";

            // 获取所有.dll文件的绝对路径
            List<string> dllFilePaths = ScanFolderForDlls(folderPath);

            DLLScaner scanner = new DLLScaner();

            // 打印所有找到的.dll文件路径
            foreach (string filePath in dllFilePaths)
            {
                scanner.Scan(filePath);
            }

            scanner.Sort();
            scanner.Rescan();
            return;
        }

        // 递归扫描指定文件夹及其子文件夹中的所有.dll文件，并返回包含所有.dll文件路径的列表
        static List<string> ScanFolderForDlls(string folderPath)
        {
            List<string> dllFilePaths = new List<string>();

            if (!Directory.Exists(folderPath))
            {
                Console.WriteLine("指定的文件夹不存在！");
                return dllFilePaths; // 返回空列表
            }

            try
            {
                // 使用SearchOption.AllDirectories选项递归查找所有子文件夹
                string[] files = Directory.GetFiles(folderPath, "*.dll", SearchOption.AllDirectories);

                // 将找到的文件路径添加到列表中
                dllFilePaths.AddRange(files);
            }
            catch (UnauthorizedAccessException ex)
            {
                Console.WriteLine("没有权限访问某些文件夹: " + ex.Message);
            }
            catch (IOException ex)
            {
                Console.WriteLine("发生IO异常: " + ex.Message);
            }

            return dllFilePaths;
        }
    }
}
