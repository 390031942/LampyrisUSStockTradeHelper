/****************************************************************************
**
** Copyright(C) 2024 HongJin Investment Cooperation
* *Contact: gameta@qq.com
* *
**File: BuildinScriptngClass.h
* *Desc : .Net¿ò¼ÜDLL¶¨Òå
* ***************************************************************************/
#pragma once
#include <string>
#include <unordered_map>

class BuildinScriptingClass {
private:
	static std::unordered_map<std::string, std::string> ms_buildInDllMap = {
		{"System","System.dll"}
	}
};