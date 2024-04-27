/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: TradeOperationPanel.cpp
** Desc: 实现了股票快速交易的窗口
****************************************************************************/
#include "TradeOperationPanel.h"
#include "Module/USmartTradeSystem.h"

TradeOperationPanel::TradeOperationPanel(QWidget* parent) :QWidget(parent) {
	this->m_ui.setupUi(this);

	QObject::connect(this->m_ui.ButtonBuy, &QPushButton::clicked, this, &TradeOperationPanel::OnClickButtonBuy);
	QObject::connect(this->m_ui.ButtonSell, &QPushButton::clicked, this, &TradeOperationPanel::OnClickButtonSell);
	QObject::connect(this->m_ui.ButtonBuyEmpty, &QPushButton::clicked, this, &TradeOperationPanel::OnClickButtonBuyEmpty);
	QObject::connect(this->m_ui.ButtonSellEmpty, &QPushButton::clicked, this, &TradeOperationPanel::OnClickButtonSellEmpty);

	m_buttonGroup.addButton(this->m_ui.RadioAll, 1);
	m_buttonGroup.addButton(this->m_ui.Radio2, 2);
	m_buttonGroup.addButton(this->m_ui.Radio3, 3);
	m_buttonGroup.addButton(this->m_ui.Radio4, 4);

	QObject::connect(&m_buttonGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this, &TradeOperationPanel::OnClickRatioButton);
	QObject::connect(&m_stockInfoCaptureTimer, &QTimer::timeout, this, &TradeOperationPanel::CaptureStockInfo);
}

TradeOperationPanel::~TradeOperationPanel() {

}

void TradeOperationPanel::OnClickButtonBuy() {
}

void TradeOperationPanel::OnClickButtonSell() {
}

void TradeOperationPanel::OnClickButtonBuyEmpty() {

}

void TradeOperationPanel::OnClickButtonSellEmpty() {

}

void TradeOperationPanel::OnClickRatioButton(int ratio) {

}

void TradeOperationPanel::CaptureStockInfo() {
}
