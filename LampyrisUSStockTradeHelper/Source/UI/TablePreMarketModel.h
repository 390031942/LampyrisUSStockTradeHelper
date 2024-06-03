/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: TablePreMarket.h
** Desc: 用于显示熔断股票的表格
****************************************************************************/
#pragma once
#include <QStandardItemModel>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QDateTime>
#include <QPushButton>
#include <unordered_map>
#include <unordered_set>
#include <Base/Utilities.h>

struct StockPreMarketInfo {
    QString      stockCode;
    float        percentage = 0.0f;
    float        price = 0.0f;
    float        maxPrice;
    float        minPrice = 99999999.0f;
    long         volumn;
    float        maxPercentage = 0.0f;
    QDateTime    maxDate;
    QDateTime    lastDate;
    int          counter;
    bool         highPercShow = false;
    int          flag = 0;
};

class TablePreMarketModelButtonDelegate : public QItemDelegate {
public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    void     paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    bool     editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;
};

class TablePreMarketModel:public QStandardItemModel {
    typedef std::unordered_map<QString, StockPreMarketInfo> PreMarketInfoMap;
private:
    bool                        isBetweenAfternoonAndNight();
                                
    QString                     MakeReasonString(int flag);
public:
	                            TablePreMarketModel();
	                           
	virtual                    ~TablePreMarketModel();

    void                        Request();
                               
    void                        RemoveStockRow(int row);
private:
    QNetworkAccessManager       m_manager;
    QTimer                      m_timer;
    PreMarketInfoMap            m_stockMap;
    qint64                      m_lastReplyTimestmap;
    std::unordered_set<QString> m_showSet;
};
