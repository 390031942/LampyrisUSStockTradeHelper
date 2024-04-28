/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: main.cpp
** Desc: 主程序入口
****************************************************************************/

#include "Scripting/MonoScriptingSystem.h"
#include <iostream>
#include <windows.h>
#include <QtWidgets/QApplication>
#include <QFileInfo>
#include <QDir>

using namespace std;

int main(int argc, char *argv[]) {
    QString appPath = argv[0];
    QString appDirPath = QFileInfo(appPath).dir().path();
    std::string asmPath = appDirPath.toStdString() + "\\LampyrisUSStockTradeHelper.Managed.dll";

    QApplication a(argc, argv);
    int returnCode;

    try {
        MonoScriptingSystem::GetInstance()->Initialize();
        MonoScriptingSystem::GetInstance()->LoadAssembly("C:\\Users\\Administrator\\source\\repos\\LampyrisUSStockTradeHelper\\x64\\Debug\\EntryPoint.dll");
        {
            // 初始化C# 
            auto type = MonoScriptingSystem::GetInstance()->GetType("LampyrisUIStockTradeHelper.Managed","EntryPoint");
            MonoObject* o = type->Invoke(nullptr, "Main(int)", 3);
            int val = mono_to_native<int>(o);
            returnCode = a.exec();
        }
        MonoScriptingSystem::GetInstance()->Finalize();
    }
    catch (std::exception& e) {
        returnCode = -1;
    }

    return returnCode;
}
