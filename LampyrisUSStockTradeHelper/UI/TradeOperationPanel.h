/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: TradeOperationPanel.h
** Desc: 实现了股票快速交易的窗口
****************************************************************************/
#pragma once

#include <QButtonGroup>
#include <QWidget>
#include <QTimer>
#include <Module/WindowsCapture.h>
#include <ui_TradeOperationPanel.h>

class TradeOperationPanel :public QWidget {
	Q_OBJECT
private:
	Ui::TradeOperationPanel m_ui;

	QButtonGroup            m_buttonGroup;

	QTimer                  m_stockInfoCaptureTimer;

	explicit                TradeOperationPanel(QWidget* parent);

	virtual                ~TradeOperationPanel();

	void                    OnClickButtonBuy();
		                    
	void                    OnClickButtonSell();
		                    
	void                    OnClickButtonBuyEmpty();
		                    
	void                    OnClickButtonSellEmpty();
		                    
	void                    OnClickRatioButton(int ratio);

	void                    CaptureStockInfo();
};
