/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: USmartTradeSystem.h
** Desc: Usmartӯ��֤ȯ ���׽ӿ�
****************************************************************************/
#pragma once

#include <string>
#include <Base/Singleton.h>

class USmartTradeSystem:public Singleton<USmartTradeSystem> {
public:
	         USmartTradeSystem();

	virtual ~USmartTradeSystem();

	void     ExecuteBuyByRatio(const std::string& code,int ratio);
		     
	void     ExecuteSellByRatio(const std::string& code, int ratio);
		     
	void     ExecuteRevoke(const std::string& orderId);

	void     ExecuteQueryPosition();
		     
	void     ExecuteQueryRevoke();

};