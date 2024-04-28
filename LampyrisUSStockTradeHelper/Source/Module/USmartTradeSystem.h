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
private:
	std::string m_content;
	std::string m_mediaType;
public:
	ScriptingStringContent(const std::string& content, const std::string& mediaType):
		m_content(content),m_mediaType(mediaType){

	}

	virtual MonoObject* ToMonoObject() {
		auto StringContentType = MonoScriptingSystem::GetInstance()->GetType("System.Net.Http", "StringContent");
		auto EncodingType = MonoScriptingSystem::GetInstance()->GetType("System.Text", "Encoding");
		auto utf8 = EncodingType->GetProperty(nullptr, "UTF8");

		return StringContentType->New("(string,System.Text.Encoding,string)", m_content.c_str(), utf8, m_mediaType.c_str());
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