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
#include "Base/EventManager.h"
#include "Base/Utilities.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <cstdlib> // For std::stof

#pragma execution_character_set("utf-8")

CustomDelegate::CustomDelegate(QObject* parent)
	: QItemDelegate(parent), button(nullptr) {}

QWidget* CustomDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&/* option */,
	const QModelIndex&/* index */) {
	button = new QPushButton("操作", parent);
	connect(button, &QPushButton::clicked, this, &CustomDelegate::on_Button_Clicked);
	return button;
}

void CustomDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
	// 可以在这里设置按钮的文本等属性
	Q_UNUSED(editor);
	Q_UNUSED(index);
}

void CustomDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
	const QModelIndex& index) const {
	// 通常在这里从editor获取数据并设置到model里
	Q_UNUSED(editor);
	Q_UNUSED(model);
	Q_UNUSED(index);
}

void CustomDelegate::on_Button_Clicked() {
	// 按钮点击事件的处理
	if (button) {
		emit commitData(button);
	}
}
TradeOperationPanel::TradeOperationPanel(QWidget* parent) :QWidget(parent) {
	this->m_ui.setupUi(this);

	// 购买比例
	m_buttonGroupRatio.addButton(this->m_ui.RadioAll, 1);
	m_buttonGroupRatio.addButton(this->m_ui.Radio2, 2);
	m_buttonGroupRatio.addButton(this->m_ui.Radio3, 3);
	m_buttonGroupRatio.addButton(this->m_ui.Radio4, 4);

	QObject::connect(&m_buttonGroupRatio, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this, &TradeOperationPanel::OnClickRatioButton);

	// 自动止盈
	m_buttonGroupSell.addButton(this->m_ui.RadioSellNone, 1);
	m_buttonGroupSell.addButton(this->m_ui.RadioSellOneDotFive, 2);
	m_buttonGroupSell.addButton(this->m_ui.RadioSellTwo, 3);
	m_buttonGroupSell.addButton(this->m_ui.RadioSellThree, 4);
	m_buttonGroupSell.addButton(this->m_ui.RadioSellFive, 5);
	m_buttonGroupSell.addButton(this->m_ui.RadioSellSeven, 6);

	QObject::connect(&m_buttonGroupSell, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this, &TradeOperationPanel::OnClickAutoSellButton);

	// 按钮组
	QObject::connect(this->m_ui.ButtonBuy, &QPushButton::clicked, this, &TradeOperationPanel::OnClickButtonBuy);
	QObject::connect(this->m_ui.ButtonSell, &QPushButton::clicked, this, &TradeOperationPanel::OnClickButtonSell);
	QObject::connect(this->m_ui.ButtonBuyEmpty, &QPushButton::clicked, this, &TradeOperationPanel::OnClickButtonBuyEmpty);
	QObject::connect(this->m_ui.ButtonSellEmpty, &QPushButton::clicked, this, &TradeOperationPanel::OnClickButtonSellEmpty);
	QObject::connect(this->m_ui.ButtonRefresh, &QPushButton::clicked, this, &TradeOperationPanel::OnClickButtonRefresh);

	// 股票信息
	QObject::connect(&m_stockInfoCaptureTimer, &QTimer::timeout, this, &TradeOperationPanel::CaptureStockInfo);

	QObject::connect(this->m_ui.EditAddress, &QLineEdit::textChanged, [=](const QString& text) {
		std::string textStr = text.toStdString();
		void* param[] = { &textStr };
		EventManager::GetInstance()->RaiseEvent(EventType::RefreshAddress, param);
	});

	EventManager::GetInstance()->AddEventHandler(EventType::ResUsmartQueryMaxCount, [=](void** param) {
		QJsonDocument& document = *(QJsonDocument*)param[0];
		QJsonObject dataObject = document["data"].toObject();
		int maxCanBuy = dataObject["maxBuyQty"].toInt();
		int maxCashCanBuy = dataObject["maxCashBuyQty"].toInt();
		double maxPurchasePower = dataObject["maxPurchasePower"].toDouble();
		int mxaSellQty = dataObject["maxSellQty"].toInt();

		this->m_ui.LabelInfo1->setText(QString("融资可买:%0,金额:%1").arg(maxCanBuy).arg(maxPurchasePower));
		this->m_ui.LabelInfo2->setText(QString("可卖数量:%0").arg(mxaSellQty));

		this->m_ui.ButtonBuy->setEnabled(maxCanBuy > 0);
		this->m_ui.ButtonSell->setEnabled(mxaSellQty > 0);
		this->m_ui.ButtonRefresh->setEnabled(true);

		this->m_maxCanBuy = maxCanBuy;
		this->m_maxCashCanBuy = maxCashCanBuy;
		this->m_maxPurchasePower = maxPurchasePower;
		this->m_maxSellQty = mxaSellQty;
	});

	EventManager::GetInstance()->AddEventHandler(EventType::ResUsmartQueryShortMaxCount, [=](void** param) {
		QJsonDocument& document = *(QJsonDocument*)param[0];
		QJsonObject dataObject = document["data"].toObject();
		int maxCanBuy = dataObject["maxBuyQty"].toInt();
		int maxCashCanBuy = dataObject["maxCashBuyQty"].toInt();
		double maxPurchasePower = dataObject["maxPurchasePower"].toDouble();
		int mxaSellQty = dataObject["maxSellQty"].toInt();

		this->m_ui.LabelInfo3->setText(QString("可沽数量:%0,金额:%1").arg(mxaSellQty).arg(maxPurchasePower));
		this->m_ui.LabelInfo4->setText(QString("可平数量:%0").arg(maxCanBuy));

		// this->m_ui.ButtonBuy->setEnabled(maxCanBuy > 0);
		// this->m_ui.ButtonSell->setEnabled(mxaSellQty > 0);
		// this->m_ui.ButtonRefresh->setEnabled(true);
		// 
		// this->m_maxCanBuy = maxCanBuy;
		// this->m_maxCashCanBuy = maxCashCanBuy;
		// this->m_maxPurchasePower = maxPurchasePower;
		// this->m_maxSellQty = mxaSellQty;
	});

	EventManager::GetInstance()->AddEventHandler(EventType::RefreshStockInfo, [=](void** param) {
		this->m_stockInfo = *(StockTradeInfo*)(param[0]);
		this->RefreshStockTradeInfo();
	});

	EventManager::GetInstance()->AddEventHandler(EventType::RefreshTempStopStock, [=](void** param) {
		std::vector<StockTradeInfo>* pInfoList = (std::vector<StockTradeInfo>*)(param[0]);
		this->RefreshTempStopStockList(pInfoList);
	});

	this->RefreshStockTradeInfo();

	// 设置为置顶
	this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint); // 设置窗口标志为始终置顶

	this->InitTempStopStockTableView();
	this->InitPreMarketTableView();

	// 按钮默认值
	this->m_ui.Radio2->setChecked(true);
	this->m_ratio = 2;
	this->m_ui.RadioSellNone->setChecked(true);
	this->m_autoSellPercentage = 0;
}

TradeOperationPanel::~TradeOperationPanel() {

}

void TradeOperationPanel::OnClickButtonBuy() {
	if (this->m_stockInfo.code != "") {
		float price = std::stof(this->m_stockInfo.price.toStdString()) + 0.01;
		price = std::round(price * 100) / 100.0f; // 乘以100进行四舍五入，然后除以100.0f保留两位小数

		int count = (int)std::floor(this->m_maxCanBuy / this->m_ratio);
		USmartTradeSystem::GetInstance()->ExecuteBuy(this->m_stockInfo.code.toStdString(),count, true, this->m_stockInfo.statusCode == 2);

		// 止盈单逻辑
		if (m_autoSellPercentage != 0.0f) {
			price = std::stof(this->m_stockInfo.price.toStdString()) * (1 + m_autoSellPercentage * 0.01);
			price = std::round(price * 100) / 100.0f; // 乘以100进行四舍五入，然后除以100.0f保留两位小数
			USmartTradeSystem::GetInstance()->ExecuteSell(this->m_stockInfo.code.toStdString(), price, count, this->m_stockInfo.statusCode == 2);
		}
	}
}

void TradeOperationPanel::OnClickButtonSell() {
	if (this->m_stockInfo.code != "") {
		float price = std::stof(this->m_stockInfo.price.toStdString()) - 0.01;
		price = std::round(price * 100) / 100.0f; // 乘以100进行四舍五入，然后除以100.0f保留两位小数

		int count = this->m_maxSellQty;
		// USmartTradeSystem::GetInstance()->ExecuteAutoSell(this->m_stockInfo.code.toStdString(), 999.00, count, false, this->m_stockInfo.statusCode == 2);
	}
}

void TradeOperationPanel::OnClickButtonBuyEmpty() {

}

void TradeOperationPanel::OnClickButtonSellEmpty() {

}

void TradeOperationPanel::OnClickButtonRefresh() {
	if (this->m_stockInfo.code != "") {
		USmartTradeSystem::GetInstance()->ExecuteQueryMaxQuantity(this->m_stockInfo.code, this->m_stockInfo.price);
		USmartTradeSystem::GetInstance()->ExecuteQueryShortMaxQuantity(this->m_stockInfo.code, this->m_stockInfo.price);
	}
}

void TradeOperationPanel::OnClickRatioButton(int ratio) {
	this->m_ratio = ratio;
}

void TradeOperationPanel::OnClickAutoSellButton(int mode) {
	switch (mode) {
		case 1:this->m_autoSellPercentage = 0;break;
		case 2:this->m_autoSellPercentage = 1.5;break;
		case 3:this->m_autoSellPercentage = 2;break;
		case 4:this->m_autoSellPercentage = 3;break;
		case 5:this->m_autoSellPercentage = 5;break;
		case 6:this->m_autoSellPercentage = 7;break;
	}
}

void TradeOperationPanel::CaptureStockInfo() {
}

void TradeOperationPanel::RefreshStockTradeInfo() {
	if (this->m_stockInfo.code == "") {
		this->m_ui.LabelStockCode->setText("暂无股票信息");
		return;
	}
	if (this->m_stockInfo.name.size() > 20) {
		this->m_stockInfo.name = this->m_stockInfo.name.mid(0,20) + "...";
	}
	this->m_ui.LabelStockCode->setText(this->m_stockInfo.code + "(" + this->m_stockInfo.name + ")");
}

void TradeOperationPanel::RefreshTempStopStockList(std::vector<StockTradeInfo>* list) {
	int size = 0;
	// 生成随机数据填充表格
	for (int i = 0; i < list->size(); ++i) {
		const auto& data = list->at(i);
		if (data.statusCode == 6 && data.money > 0 ){ //&& Utilities::IsAfterTodayUSMarketOpenTime(data.timeStamp)) {
			QStandardItem* item1 = this->m_tempStopStockModel->item(size, 0);
			QStandardItem* item2 = this->m_tempStopStockModel->item(size, 1);
			QStandardItem* item3 = this->m_tempStopStockModel->item(size, 2);
			QStandardItem* item4 = this->m_tempStopStockModel->item(size, 3);

			if (!item1) {
				 item1 = new QStandardItem(data.code);
				 item2 = new QStandardItem(QString("%0").arg(data.percentage) + "%");
				 item3 = new QStandardItem(Utilities::GetMoneyString(data.money));
				 item4 = new QStandardItem(QString("%0").arg(data.turnOverRate) + "%");

				 this->m_tempStopStockModel->setItem(size, 0, item1);
				 this->m_tempStopStockModel->setItem(size, 1, item2);
				 this->m_tempStopStockModel->setItem(size, 2, item3);
				 this->m_tempStopStockModel->setItem(size, 3, item4);
			}
			else {
				item1->setText(data.code);
				item2->setText(QString("%0").arg(data.percentage) + "%");
				item3->setText(Utilities::GetMoneyString(data.money));
				item4->setText(QString("%0").arg(data.turnOverRate) + "%");
			}

			size++;
		}
	}
	this->m_tempStopStockModel->setRowCount(size);
}

void TradeOperationPanel::InitTempStopStockTableView() {

	// 创建表格模型
	QStandardItemModel* model = this->m_tempStopStockModel = new QStandardItemModel;
	model->setColumnCount(4);

	// 设置列的标题
	model->setHorizontalHeaderLabels(QStringList() << "代码" <<"涨幅" << "金额" << "换手");

	this->m_ui.TableTempStop->setModel(model); // 给视图设置模型
}

void TradeOperationPanel::InitPreMarketTableView() {
	// 创建表格模型
	QStandardItemModel* model = this->m_preMarketModel = new TablePreMarketModel;
	model->setColumnCount(5);
	// 设置列的标题
	model->setHorizontalHeaderLabels(QStringList() << "代码" << "盘前价" << "盘前涨幅" << "盘前量" << "原因" <<  "");
	this->m_ui.TablePreMarket->setItemDelegate(new TablePreMarketModelButtonDelegate());
	this->m_ui.TablePreMarket->setModel(model); // 给视图设置模型
}
