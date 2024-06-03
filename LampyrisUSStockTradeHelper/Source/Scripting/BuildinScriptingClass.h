/****************************************************************************
**
** Copyright(C) 2024 HongJin Investment Cooperation
* *Contact: gameta@qq.com
* *
** File: BuildinScriptngClass.h
** Desc : .Net¿ò¼ÜDLL¶¨Òå
* ***************************************************************************/
#pragma once
#include <string>
#include <unordered_map>

static std::unordered_map<std::string, std::string> g_buildInDllMap = {
	{"System","C:\\Program Files\\Mono\\lib\\mono\\4.5\\System.dll"},
	{"System.Net","C:\\Program Files\\Mono\\lib\\mono\\4.5\\System.Net.dll"},
	{"System.Net.Http","C:\\Program Files\\Mono\\lib\\mono\\4.5\\System.Net.Http.dll"},
	{"System.Core","C:\\Program Files\\Mono\\lib\\mono\\4.5\\System.Core.dll"},
	{"mscorlib","C:\\Program Files\\Mono\\lib\\mono\\4.5\\mscorlib.dll"},
};