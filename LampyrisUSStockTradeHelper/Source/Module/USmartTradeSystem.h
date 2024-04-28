/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: USmartTradeSystem.h
** Desc: Usmart盈立证券 交易接口
****************************************************************************/
#pragma once

#include <string>
#include <Base/Singleton.h>
#include <Scripting/MonoScriptingSystem.h>

class ScriptingStringContent :public IMonoObjectWrapper {
public:
	ScriptingStringContent(const std::string& content, const std::string& mediaType) {

	}

	virtual MonoObject* ToMonoObject() {
		auto type = MonoScriptingSystem::GetInstance()->GetType("System.Net.Http", "StringContent");
		type->New();
	}
};

class USmartTradeSystem:public Singleton<USmartTradeSystem> {
private:
	MonoScriptingClassPtr m_httpRequestClass;
public:
	         USmartTradeSystem();

	virtual ~USmartTradeSystem();

	void     ExecuteBuyByRatio(const std::string& code,int ratio);
		     
	void     ExecuteSellByRatio(const std::string& code, int ratio);
		     
	void     ExecuteRevoke(const std::string& orderId);

	void     ExecuteQueryPosition();
		     
	void     ExecuteQueryRevoke();

};