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

using namespace std;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    int returnCode;

    try {
        MonoScriptingSystem::GetInstance()->Initialize();
        MonoScriptingSystem::GetInstance()->LoadAssembly("LampyrisUSStockTradeHelper.Managed.dll");
        {
            returnCode = a.exec();
        }
        MonoScriptingSystem::GetInstance()->Finalize();
    }
    catch (std::exception& e) {
        returnCode = -1;
    }

    return returnCode;
}
