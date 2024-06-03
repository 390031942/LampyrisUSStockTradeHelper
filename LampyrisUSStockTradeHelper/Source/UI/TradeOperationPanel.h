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
#include "Model/StockTradeInfo.h"
#include <QStandardItemModel>
#include <QItemDelegate>
#include <UI/TablePreMarketModel.h>

class CustomDelegate : public QItemDelegate {
	Q_OBJECT

public:
	explicit CustomDelegate(QObject* parent = nullptr);

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
		const QModelIndex& index);

	void setEditorData(QWidget* editor, const QModelIndex& index) const override;

	void setModelData(QWidget* editor, QAbstractItemModel* model,
		const QModelIndex& index) const override;

private slots:
	void on_Button_Clicked();

private:
	QPushButton* button;
};

class TradeOperationPanel :public QWidget {
	Q_OBJECT
private:
	Ui::TradeOperationPanel m_ui;

	QButtonGroup            m_buttonGroupRatio;

	QButtonGroup            m_buttonGroupSell;

	QTimer                  m_stockInfoCaptureTimer;

	StockTradeInfo          m_stockInfo;

	QStandardItemModel*     m_tempStopStockModel;

	QStandardItemModel*     m_preMarketModel;

	int                     m_ratio;

	int                     m_autoSellPercentage;

	int                     m_maxCanBuy{0};

	int                     m_maxCashCanBuy{0};

	double                  m_maxPurchasePower{0.0};

	int                     m_maxSellQty{0};

	void                    OnClickButtonBuy();
		                    
	void                    OnClickButtonSell();
		                    
	void                    OnClickButtonBuyEmpty();
		                    
	void                    OnClickButtonSellEmpty();

	void                    OnClickButtonRefresh();
		                    
	void                    OnClickRatioButton(int ratio);

	void                    OnClickAutoSellButton(int mode);

	void                    CaptureStockInfo();

	void                    RefreshStockTradeInfo();

	void                    RefreshTempStopStockList(std::vector<StockTradeInfo>* list);

	void                    InitTempStopStockTableView();

	void                    InitPreMarketTableView();
public:
	explicit                TradeOperationPanel(QWidget* parent = NULL);

	virtual                ~TradeOperationPanel();
};
