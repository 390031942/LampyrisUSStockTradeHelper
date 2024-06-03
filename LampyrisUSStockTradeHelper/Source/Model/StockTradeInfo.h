/****************************************************************************
**
** Copyright(C) 2024 HongJin Investment Cooperation
* *Contact: gameta@qq.com
* *
** File: StockTradeInfo.h
** Desc : 股票的交易信息，包括名称，可买数量，可沽空等
* ***************************************************************************/
#pragma once
#include <QString>
#include <QDatetime>

class StockTradeInfo {
public:		  
	QString   code; // f12
	QString   name; // f14
			  
	int       maxCanBuy;
	float     maxCanBuyMoney;
	int       maxCanBuyEmpty;
	float     maxCanBuyEmptyMoney;
			  
	QString   openPrice; // f17
	QString   price; // f2
	double    turnOverRate; // f8
	double    money; // f6
	double    percentage; // f3
			  
	int       statusCode; // f3
	QDateTime timeStamp; // f124
};