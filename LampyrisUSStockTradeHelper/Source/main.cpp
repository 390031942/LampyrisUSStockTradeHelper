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
#include <Module/USmartTradeSystem.h>

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
            ScriptingStringContent s("abc","asv");
            auto mono = s.ToMonoObject();
            // 初始化C# 
            returnCode = a.exec();
        }
        MonoScriptingSystem::GetInstance()->Finalize();
    }
    catch (std::exception& e) {
        returnCode = -1;
    }

    return returnCode;
}
