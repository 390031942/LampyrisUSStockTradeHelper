/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: USmartTradeSystem.cpp
** Desc: Usmart盈立证券 交易接口
****************************************************************************/

#include "USmartTradeSystem.h"

USmartTradeSystem::USmartTradeSystem() {
	m_httpRequestClass = MonoScriptingSystem::GetInstance()->GetType("LampyrisUIStockTradeHelper.Managed", "HttpRequest");
}

USmartTradeSystem::~USmartTradeSystem() {

}

void USmartTradeSystem::ExecuteBuyByRatio(const std::string& code, int ratio) {
	m_httpRequestClass->Invoke(nullptr, "PostAsync", "/asset-center-sg/api/app-stockHoldAsset/v5");
}

void USmartTradeSystem::ExecuteSellByRatio(const std::string& code, int ratio) {
}

void USmartTradeSystem::ExecuteQueryPosition() {
}

void USmartTradeSystem::ExecuteQueryRevoke() {
}
