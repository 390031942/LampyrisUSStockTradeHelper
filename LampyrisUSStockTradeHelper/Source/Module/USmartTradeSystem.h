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
#include <unordered_map>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <zlib.h>
#include <Base/Singleton.h>
#include <QTimer>
#include <Module/WindowsCapture.h>
#include <Model/StockTradeInfo.h>
#include <QTimeZone>

enum TradeMarket {
	US = 1,
	HK = 2,
};

class USmartTradeSystem:public Singleton<USmartTradeSystem> {
	typedef std::vector<StockTradeInfo> StockInfoList;
	typedef std::unordered_map<std::string,StockTradeInfo> StockInfoMap;
private:
	QNetworkAccessManager m_manager;
	QNetworkRequest       m_request;
	QNetworkReply*        m_replyOp;
	QNetworkReply*        m_replyQueryAsset;
	QNetworkReply*        m_replyRevoke;
	QNetworkReply*        m_replyMaxCount;
	QNetworkReply*        m_replyShortMaxCount;
	QNetworkReply*        m_replyTodayOrder;
	QNetworkReply*        m_replyQueryServerTime;

	QNetworkAccessManager m_listGetManager;
	QNetworkRequest       m_listGetRequest;
	QNetworkReply*        m_replyListGet;

	WindowsCapture*       m_winCap;
	QTimer*               m_memoryReadTimer;
	HANDLE                m_processHandle;
	DWORD                 m_processId = 0;
	DWORD64               m_address = 0;
	StockInfoList         m_stockInfoList;
	StockInfoMap          m_stockInfoMap;

	QTimer*               m_stockRefreshTimer;
	std::string           m_lastViewingStockCode;
	qint64                m_serverTimestamp;
	qint64                m_timeStamp;

	QNetworkAccessManager m_preMarketManager;
	QNetworkRequest       m_preMarketRequest;

	QDateTime             m_lastestQuoteTime;

	// 龙一策略相关
	bool                  m_isDragonOneExecuted = false;
	QTimer                m_dragonOneTimer;
	int                   m_dragonOneBuyCount;
	bool                  m_dragonQueryQty = false;
	bool                  m_dragonQueryAfterBuyQty = false;
	QString               m_dragonOneCode;
	float                 m_dragonOnePrice;

	QString               extractDataFromJson(const QString& rawString);

	bool                  isMarketOpen();
public:
	                      USmartTradeSystem();

	void                  Initialize();
			             
	virtual              ~USmartTradeSystem();
			             
	void                  ExecuteBuy(const std::string& code, int count, bool isInMarket, float limitPrice = 0.0f);

	void                  ExecuteSell(const std::string& code, int count, bool isInMarket, float limitPrice = 0.0f);

	void                  ExecuteOperationEx(const std::string& code,float price,int count,bool isBuy,bool isInMarket);
		                  
	void                  ExecuteRevoke(const std::string& orderId);
			             
	void                  ExecuteQueryPosition();
		     
	void                  ExecuteQueryTodayOrder();

	void                  ExecuteQueryMaxQuantity(const QString& code, const QString& price);

	void                  ExecuteQueryShortMaxQuantity(const QString& code, const QString& price);

	void                  RefreshUSStockList();
};