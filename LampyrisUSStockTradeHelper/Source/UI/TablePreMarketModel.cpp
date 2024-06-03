/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: TablePreMarket.cpp
** Desc: 用于显示熔断股票的表格
****************************************************************************/
#pragma once
#include "TablePreMarketModel.h"
#include <QDebug>

TablePreMarketModel::TablePreMarketModel() {
    QObject::connect(&m_manager, &QNetworkAccessManager::finished, [=](QNetworkReply* reply) {
        m_lastReplyTimestmap = QDateTime::currentMSecsSinceEpoch();
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error:" << reply->errorString();
            reply->deleteLater();
            return;
        }
        QString result = reply->readAll();
        auto document = QJsonDocument::fromJson(result.toUtf8());
        QJsonArray array = document["data"].toObject()["answer"].toArray().at(0).toObject()["txt"].toArray().at(0).toObject()["content"].toObject()["components"].toArray().at(0).toObject()["data"].toObject()["datas"].toArray();

        int size = 0;
        for (int i = 0; i < array.size(); i++) {
            QJsonObject obj = array[i].toObject();
            QString     stockCode = obj["code"].toString();
            QDateTime   dateTime = QDateTime::currentDateTime();
            float       price = std::stof(obj["美股@最新价"].toString().toStdString());
            long        volumn = std::stol(obj["美股@成交量[20240517]"].toString().toStdString());
            float       perc = std::stof(obj["美股@盘前涨跌幅"].toString().toStdString());

            if (!m_stockMap.contains(stockCode)) {
                m_stockMap[stockCode] = { stockCode };
            }

            StockPreMarketInfo& stockInfo = m_stockMap[stockCode];
            stockInfo.price = price;
            stockInfo.percentage = perc;
            stockInfo.volumn = volumn;
            stockInfo.lastDate = dateTime;

            bool isNewMax = false;
            QDateTime previousMaxDate = stockInfo.maxDate;

            if (price > stockInfo.maxPrice) {
                stockInfo.maxDate = dateTime;
                stockInfo.maxPrice = price;
                stockInfo.maxPercentage = perc;
                isNewMax = true;
            }

            if (price < stockInfo.minPrice) {
                stockInfo.minPrice = price;
            }

            bool needShow = true;

            // 第一次出现拉升10%以上且 小于5秒之内的连续成交次数大于3次
            if ((stockInfo.flag & 1) == 0) {
                if (stockInfo.percentage > 10) {
                    if (!stockInfo.highPercShow) {
                        stockInfo.highPercShow = true;
                    }
                    else {
                        if (stockInfo.lastDate != dateTime && stockInfo.lastDate != QDateTime()) {
                            if (abs(dateTime.secsTo(stockInfo.lastDate)) < 5.0f) {
                                stockInfo.counter = stockInfo.counter + 1;
                                if (stockInfo.counter >= 3) {
                                    needShow = true;
                                    stockInfo.flag = stockInfo.flag & 1;
                                }
                            }
                            else {
                                stockInfo.counter = 0;
                            }
                        }
                    }
                }
                else {
                    stockInfo.counter = 0;
                }
            }

            // 第一次出现拉升10%以上,且中位数价格*成交量的金额大于10000美金
            if ((stockInfo.flag & 2) == 0) {
                if (stockInfo.percentage > 10 && (((stockInfo.minPrice + stockInfo.maxPrice) / 2.0f) * stockInfo.volumn) > 10000.0f) {
                    stockInfo.flag = stockInfo.flag & 2;
                }
            }

            // 间隔时间新高
            if ((stockInfo.flag & 4) == 0) {
                if (isNewMax) {
                    QTime timeA = previousMaxDate.time();
                    QTime timeB = stockInfo.maxDate.time();

                    // 计算时间差（分钟）
                    int minutesDifference = qAbs(timeA.minute() - timeB.minute());

                    if (dateTime.time() >= QTime(4, 1) && minutesDifference >= 5) {
                        stockInfo.flag = stockInfo.flag & 4;
                    }
                }
            }

            stockInfo.lastDate = dateTime;

            // if (stockInfo.flag <= 0) {
            //     continue;
            // }

            QStandardItem* item1 = this->item(size, 0);
            QStandardItem* item2 = this->item(size, 1);
            QStandardItem* item3 = this->item(size, 2);
            QStandardItem* item4 = this->item(size, 3);
            QStandardItem* item5 = this->item(size, 4);
            QStandardItem* item6 = this->item(size, 5);

            if (!item1) {
                item1 = new QStandardItem(stockCode);
                item2 = new QStandardItem(QString("%0").arg(price));
                item3 = new QStandardItem(QString("%0").arg(perc) + "%");
                item4 = new QStandardItem(QString("%0").arg(volumn));
                item5 = new QStandardItem(MakeReasonString(stockInfo.flag));
                item6 = new QStandardItem;

                this->setItem(size, 0, item1);
                this->setItem(size, 1, item2);
                this->setItem(size, 2, item3);
                this->setItem(size, 3, item4);
                this->setItem(size, 4, item5);
                this->setItem(size, 5, item6);
            }
            else {
                item1->setText(stockCode);
                item2->setText(QString("%0").arg(price));
                item3->setText(QString("%0").arg(perc) + "%");
                item4->setText(QString("%0").arg(volumn));
                item5->setText(MakeReasonString(stockInfo.flag));
            }

            size++;

            break;
        }

        this->setRowCount(size);
        reply->deleteLater();
    });

    this->Request();
    QObject::connect(&m_timer, &QTimer::timeout, [=]() {
        if (!isBetweenAfternoonAndNight()) {
            return;
        }
        if (QDateTime::currentMSecsSinceEpoch() - m_lastReplyTimestmap > 1000) {
            this->Request();
        }
    });

    m_timer.setInterval(100);
    // m_timer.start();
}

TablePreMarketModel::~TablePreMarketModel() {

}

void TablePreMarketModel::Request() {
    QNetworkRequest request;
    request.setUrl(QUrl("https://www.iwencai.com/unifiedwap/unified-wap/v2/result/get-robot-data"));
    QString data = R"({"source":"Ths_iwencai_Xuangu","version":"2.0","query_area":"","block_list":"","add_info":"{\"urp\":{\"scene\":1,\"company\":1,\"business\":1},\"contentType\":\"json\",\"searchInfo\":true}","question":"盘前涨幅榜前1，成交金额大于1万","perpage":100,"page":1,"secondary_intent":"usstock","log_info":"{\"input_type\":\"typewrite\"}","rsh":"Ths_iwencai_Xuangu_cqkgja940i8c99rh7bxgtqfgg4h8y2nt"})";
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("hexin-v", "A5HVW-W9k-jf9P_8IQdWisFdoJYu_gZur3OphHM1jOx3xLvCu04VQD_CuUUA");
    request.setRawHeader("accept", "application/json, text/plain, */*");
    request.setRawHeader("accept-language", "zh-CN,zh;q=0.9");
    request.setRawHeader("cache-control", "no-cache");
    request.setRawHeader("pragma", "no-cache");
    request.setRawHeader("sec-ch-ua", "\";Not A Brand\";v=\"99\", \"Chromium\";v=\"94\"");
    request.setRawHeader("sec-ch-ua-mobile", "?0");
    request.setRawHeader("sec-ch-ua-platform", "\"Windows\"");
    request.setRawHeader("sec-fetch-dest", "empty");
    request.setRawHeader("sec-fetch-mode", "cors");
    request.setRawHeader("sec-fetch-site", "same-origin");
    m_manager.post(request,data.toUtf8());
}

void TablePreMarketModel::RemoveStockRow(int row) {
    QString text = this->item(row, 0)->text();
    m_stockMap[text].flag = 0;
}

bool TablePreMarketModel::isBetweenAfternoonAndNight() {
    QTime currentTime = QTime::currentTime();
    QTime afternoonStart(16, 0);
    QTime nightEnd(21, 30);
    return currentTime >= afternoonStart && currentTime <= nightEnd;
}

QString TablePreMarketModel::MakeReasonString(int flag) {
    QString result = "";
    if ((flag & 1) != 0) {
        result = result + "10%(成交频次)+";
    }
    if ((flag & 2) != 0) {
        result = result + "10%(高成交金额)+";
    }
    if ((flag & 4) != 0) {
        result = result + "间隔时间新高+";
    }

    return result.mid(0, result.length() - 1);
}

QWidget* TablePreMarketModelButtonDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QPushButton* button = new QPushButton(parent);
    button->setText("忽略");
    return button;
}

void TablePreMarketModelButtonDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (index.column() == 4) { // 假设第一列是按钮列
        QPushButton* editor = qobject_cast<QPushButton*>(sender());
        if (editor) {
            QApplication::style()->drawControl(QStyle::CE_PushButton, &option, painter, editor);
        }
        else {
            QStyleOptionButton buttonOption;
            buttonOption.rect = option.rect;
            buttonOption.text = "忽略";
            QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter, nullptr);
        }
    }
    else {
        QItemDelegate::paint(painter, option, index);
    }
}

bool TablePreMarketModelButtonDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) {
    if (index.column() == 4 && event->type() == QEvent::MouseButtonRelease) {
        ((TablePreMarketModel*)model)->RemoveStockRow(index.row());
        return true; // 表示事件已被处理
    }
    return false;
}
