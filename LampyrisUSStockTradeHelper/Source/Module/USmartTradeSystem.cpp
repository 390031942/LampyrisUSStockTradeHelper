/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: USmartTradeSystem.cpp
** Desc: Usmartӯ��֤ȯ ���׽ӿ�
****************************************************************************/

#include "USmartTradeSystem.h"
#include <format>
#include <Base/Utilities.h>
#include <Base/EventManager.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <Windows.h>
#include <QInputDialog>
#include <QDatetime>
#include <QNetworkCookieJar>
#include <Module/SystemTrayIcon.h>

QString USmartTradeSystem::extractDataFromJson(const QString& rawString) {
    // ���� JSON ��ʼ�ͽ�����λ��
    int beginIndex = rawString.indexOf('{');
    int endIndex = rawString.lastIndexOf('}');

    // ����Ҳ�����ʼ�����λ�ã����ؿ��ַ���
    if (beginIndex == -1 || endIndex == -1) {
        return QString();
    }

    // ��ȡ JSON �ַ���
    QString jsonString = rawString.mid(beginIndex, endIndex - beginIndex + 1);

    // �� JSON �ַ�������Ϊ QJsonDocument
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8(), &parseError);

    // �������Ƿ�ɹ�
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON Parse Error:" << parseError.errorString();
        return QString();
    }

    // �� QJsonDocument �л�ȡ QJsonObject
    QJsonObject jsonObj = jsonDoc.object();

    // �� QJsonObject ����ȡ "data" ��ֵ
    QJsonValue dataValue = jsonObj.value("data");

    // ��� "data" �Ƿ���ڲ�ת��Ϊ QString
    if (!dataValue.isString()) {
        qDebug() << "Data value is missing or not a string.";
        return QString();
    }

    return dataValue.toString();
}

bool USmartTradeSystem::isMarketOpen() {
    return true;
    // ���ɿ���ʱ�䣺ET 9:30 AM
    // ��������ʱ�䣺ET 4:00 PM
    const int marketOpenHourET = 9;
    const int marketOpenMinuteET = 30;
    const int marketCloseHourET = 16; // 24-hour format for 4 PM

    // ��ȡ��ǰ�ı���ʱ��
    QDateTime nowBeijing = QDateTime::currentDateTime();
    nowBeijing.setTimeSpec(Qt::UTC); // ת��ΪUTCʱ��

    // �ֶ�������������ʱ���뱱��ʱ���ʱ��
    // ͨ������£�����ʱ�����������ʱ���12��13Сʱ��ȡ�����Ƿ�Ϊ����ʱ
    int timeDifference = 12; // Ĭ��Ϊ������ʱ��ʱ��

    // �ֶ���鵱ǰ�����Ƿ�����������ʱ�������ʱ�ڼ�
    // ��������ʱͨ����3�µĵڶ��������տ�ʼ��11�µĵ�һ�������ս���
    // ע�⣺������Ҫ����ʵ�����ڽ����жϣ����½�Ϊʾ��
    QDate currentDate = nowBeijing.date();
    if (currentDate.month() == 3 && currentDate.day() >= 8 ||
        currentDate.month() == 4 ||
        currentDate.month() == 5 ||
        currentDate.month() == 6 ||
        currentDate.month() == 7 ||
        currentDate.month() == 8 ||
        (currentDate.month() == 11 && currentDate.day() <= 7)) {
        timeDifference = 12; // ����ʱ�ڼ�
    }
    else {
        timeDifference = 13; // ����ʱ�ڼ�
    }

    // ������������ʱ��
    QDateTime nowEastern = nowBeijing.addSecs(timeDifference * 3600);

    // ��ȡ����ʱ���Сʱ�ͷ���
    int currentHourBeijing = nowBeijing.time().hour();
    int currentMinuteBeijing = nowBeijing.time().minute();

    // �жϱ���ʱ���Ƿ������ɿ���ʱ����
    bool isMarketOpen = (currentHourBeijing >= 21 + timeDifference ||
        (currentHourBeijing == 21 + timeDifference && currentMinuteBeijing >= 30)) &&
        (currentHourBeijing < 5 + timeDifference ||
            (currentHourBeijing == 5 + timeDifference && currentMinuteBeijing < 60));

    return isMarketOpen;
}

USmartTradeSystem::USmartTradeSystem() {
    
}

void USmartTradeSystem::Initialize() {
    // ��������ͷ
    m_request.setRawHeader("Connection", "keep-alive");
    m_request.setRawHeader("X-Dev-Id", "31d2874b09ba7c6004c42b516fcd9b2351b4ae4b95147c022ca58c0cc9b7b74a");
    m_request.setRawHeader("Authorization", "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzZXNzaW9uIjoiZWQwNmQ1NjQxOWYxNGRmYThhZjMxYWYzMTEyY2UyNmQiLCJzb3VyY2UiOiJwYyIsInV1aWQiOjk3NTQxNjEzOTg1MzEyMzU4NH0.d-B_wnQrcCgfsslyPqwGxUnqWTqUAtkJSWF60RCewaQ");
    m_request.setRawHeader("X-Ver", "3.27.4");
    m_request.setRawHeader("X-Token", "06c0a56d9dd7669c6ef5894063372063");
    m_request.setRawHeader("X-Uid", "975416139853123584");
    m_request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) uSmart/3.27.4 Chrome/100.0.4896.160 Electron/18.3.5 Safari/537.36");
    m_request.setRawHeader("X-Dt", "win");
    m_request.setRawHeader("Accept", "application/json, text/plain, */*");
    m_request.setRawHeader("X-Net-Type", "");
    m_request.setRawHeader("X-Dev-Info", "electron-app");
    m_request.setRawHeader("X-Type", "13");
    m_request.setRawHeader("X-Lang", "1");
    m_request.setRawHeader("X-Time", "1713368128474");
    m_request.setRawHeader("X-Trans-Id", "e73ea9e6-be54-43c6-bc44-99fc93c077b5");
    m_request.setRawHeader("X-Request-Id", "86ccea8c-5389-4849-8e3e-01b46237bc63");
    m_request.setRawHeader("Sec-Fetch-Site", "cross-site");
    m_request.setRawHeader("Sec-Fetch-Mode", "cors");
    m_request.setRawHeader("Sec-Fetch-Dest", "empty");
    m_request.setRawHeader("Accept-Encoding", "gzip, deflate, br");
    m_request.setRawHeader("Accept-Language", "zh-CN");
    m_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    m_request.setAttribute(QNetworkRequest::CookieLoadControlAttribute, false);
    m_request.setHeader(QNetworkRequest::CookieHeader, QByteArray());

    QObject::connect(&m_manager, &QNetworkAccessManager::finished, [=](QNetworkReply* reply) {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error:" << reply->errorString();
            reply->deleteLater();
            return;
        }

        QByteArray result = reply->readAll(); //  Utilities::GZipUnCompress(reply->readAll());
        QString fromByteArray = QString::fromUtf8(result);

        // ��ѯ������ʱ������⴦��
        if (reply == m_replyQueryServerTime) {
            this->m_serverTimestamp = extractDataFromJson(fromByteArray).toULongLong();
            qint64 currentLocalTimestamp = QDateTime::currentMSecsSinceEpoch();
            this->m_timeStamp = m_serverTimestamp - currentLocalTimestamp;
            return;
        }

        auto document = QJsonDocument::fromJson(result);
        void* params[] = { &document };

        if (reply == m_replyOp) {
            EventManager::GetInstance()->RaiseEvent(EventType::ResUSmartOp, params);
            SystemTrayIcon::GetInstance()->ShowMessage("�����������", fromByteArray);
        }
        else if (reply == m_replyQueryAsset) {
            EventManager::GetInstance()->RaiseEvent(EventType::ResUSmartQueryAsset, params);
        }
        else if (reply == m_replyRevoke) {
            EventManager::GetInstance()->RaiseEvent(EventType::ResUSmartRevoke, params);
        }
        else if (reply == m_replyMaxCount) {
            // ��һ���Բ�ѯ�������������ִ�й���
            if (this->m_dragonQueryQty) {
                QJsonObject dataObject = document["data"].toObject();
                int maxCanBuy = dataObject["maxBuyQty"].toInt();
                this->m_dragonOneBuyCount = maxCanBuy;
                this->ExecuteBuy(m_dragonOneCode.toStdString(), maxCanBuy, true, m_dragonOnePrice);

                this->m_dragonOneTimer.start();
                this->m_dragonQueryQty = false;
            }
            // ��һ���Լ���ǲ�������ɽ����
            if (this->m_dragonQueryAfterBuyQty) {
                QJsonObject dataObject = document["data"].toObject();
                int maxSellQty = dataObject["maxSellQty"].toInt();
                
                if (this->m_dragonOneBuyCount == maxSellQty) {
                    // ��������
                    float correntRate = 0.0f;
                    if (this->m_dragonOnePrice < 0.5f) {
                        correntRate = 0.02f;
                    }
                    else if (this->m_dragonOnePrice >= 0.5f && this->m_dragonOnePrice <= 2.0f) {
                        correntRate = 0.01f;
                    }

                    // ����۸�����10% + ����������(����
                    // )
                    float ceilPrice = m_dragonOnePrice * (1.1f);

                    // ��ȡ����0.01����С��λv
                    this->m_dragonOnePrice = std::ceil(ceilPrice * 100) / 100;

                    this->ExecuteSell(m_dragonOneCode.toStdString(), ceilPrice, true, this->m_dragonOnePrice);

                    // ֹͣ��ʱ��
                    this->m_dragonOneTimer.stop();
                }
                this->m_dragonQueryAfterBuyQty = false;
            }
            EventManager::GetInstance()->RaiseEvent(EventType::ResUsmartQueryMaxCount, params);
        }
        else if (reply == m_replyShortMaxCount) {
            EventManager::GetInstance()->RaiseEvent(EventType::ResUsmartQueryShortMaxCount, params);
        }
        else if (reply == m_replyTodayOrder) {
            EventManager::GetInstance()->RaiseEvent(EventType::ResUsmartQueryTodayOrder, params);
        }
        reply->deleteLater();
        });

    QObject::connect(&m_listGetManager, &QNetworkAccessManager::finished, [=](QNetworkReply* reply) {
        this->m_replyListGet = nullptr;

        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error:" << reply->errorString();
            reply->deleteLater();
            return;
        }
        QByteArray result = reply->readAll();
        auto document = QJsonDocument::fromJson(result);
        QJsonArray array = document["data"].toObject()["diff"].toArray();

        m_stockInfoList.clear();
        m_stockInfoMap.clear();

        QDateTime nowDateTime = QDateTime::currentDateTime();
        int maxTimeStamp = 0;

        for (int i = 0; i < array.size(); i++) {
            StockTradeInfo stockInfo;
            auto value = array[i].toObject();
            stockInfo.code = value["f12"].toString();
            stockInfo.name = value["f14"].toString();
            stockInfo.price = QString("%0").arg(value["f2"].toDouble());
            stockInfo.openPrice = QString("%0").arg(value["f17"].toDouble());
            stockInfo.percentage = value["f3"].toDouble();
            stockInfo.turnOverRate = value["f8"].toDouble();
            stockInfo.money = value["f6"].toDouble();
            stockInfo.statusCode = value["f292"].toInt();
            int timeStmap = value["f124"].toInt();
            stockInfo.timeStamp = QDateTime::fromMSecsSinceEpoch(timeStmap);
            stockInfo.percentage = std::round(stockInfo.percentage * 100) / 100.0;
            stockInfo.turnOverRate = std::round(stockInfo.turnOverRate * 100) / 100.0;

            m_stockInfoList.push_back(stockInfo);
            m_stockInfoMap[stockInfo.code.toLocal8Bit().toStdString()] = stockInfo;

            if (timeStmap > maxTimeStamp) {
                maxTimeStamp = timeStmap;
            }
        }

        std::sort(m_stockInfoList.begin(), m_stockInfoList.end(), [](const StockTradeInfo& a, const StockTradeInfo& b) {
            return b.percentage > a.percentage;
        });

        if (!m_isDragonOneExecuted) {
            QDateTime maxDateTime = QDateTime::fromMSecsSinceEpoch(maxTimeStamp);
            QTime maxTime = maxDateTime.time();
            if (maxTime.hour() >= 0 && maxTime.minute() >= 0 && maxTime.second() >= 0) {
                m_isDragonOneExecuted = true; 
                for (int i = 0; i < this->m_stockInfoList.size(); i++) {
                    const StockTradeInfo& info = this->m_stockInfoList[0];
                    if (info.code.size() >= 5 || info.name.contains("Wt") || info.code.contains("_")) {
                        continue;
                    }

                    this->m_dragonOneCode = info.code;

                    // ����۸��µ�2%
                    float droppedPrice = std::stof(info.openPrice.toStdString()) * 0.98f;

                    // ��ȡ����0.01����С��λ
                    this->m_dragonOnePrice = std::floor(droppedPrice * 100) / 100;

                    this->m_dragonQueryQty = true;
                    this->ExecuteQueryMaxQuantity(info.code, QString("%1").arg(this->m_dragonOnePrice));
                    break;
                }
            }
        }

        void* param[] = { &this->m_stockInfoList };
        EventManager::GetInstance()->RaiseEvent(EventType::RefreshTempStopStock, param);
    });

    this->m_memoryReadTimer = new QTimer;
    this->m_memoryReadTimer->setInterval(10);

    QObject::connect(this->m_memoryReadTimer, &QTimer::timeout, [=]() {
        if (!this->m_winCap->CheckWindowValid()) {
            this->m_winCap->Init();
        }
        DWORD pid = this->m_winCap->GetWindowProcessID();
        if (pid != m_processId) {
            HANDLE process_handle = OpenProcess(PROCESS_VM_READ, FALSE, pid);
            if (process_handle == NULL) {
                return;
            }
            m_processId = pid;
            this->m_processHandle = process_handle;
        }

        char buffer[256]{'\0'}; // �洢��ȡ�����ڴ�����
        SIZE_T bytes_read = 0; // ʵ�ʶ�ȡ���ֽ���

        BOOL success = ReadProcessMemory(
            this->m_processHandle,
            (LPVOID)this->m_address,
            buffer,
            sizeof(buffer),
            &bytes_read
        );

        if (success) {
            std::string stockCode = buffer;
            if (m_stockInfoMap.contains(stockCode)) {
                StockTradeInfo info = m_stockInfoMap[stockCode];
                void* param[] = {&info};
                EventManager::GetInstance()->RaiseEvent(EventType::RefreshStockInfo, param);

                if (m_lastViewingStockCode != stockCode) {
                    m_lastViewingStockCode = stockCode;
                    this->ExecuteQueryMaxQuantity(info.code, info.price);
                    this->ExecuteQueryShortMaxQuantity(info.code, info.price);
                }
            }
        }
    });
    this->m_winCap = new WindowsCapture("CHQPDFixedHeader", "CHQObjWndBase");
    this->m_winCap->Init();
    this->m_memoryReadTimer->start();

    EventManager::GetInstance()->AddEventHandler(EventType::RefreshAddress, [=](void** param) {
        std::string addressText = *(std::string*)(param[0]);
        try {
            DWORD64 address = std::stoi(addressText, nullptr, 16); // Ҫ��ȡ���ڴ��ַ
            this->m_address = address;
        }
        catch(std::exception& ex) {}
    });

    this->RefreshUSStockList();
    this->ExecuteQueryPosition();

    // ��ʼ��ˢ�¹�Ʊ���鶨ʱ��
    this->m_stockRefreshTimer = new QTimer;
    this->m_stockRefreshTimer->setInterval(1000);
    this->m_stockRefreshTimer->start();

    QObject::connect(this->m_stockRefreshTimer, &QTimer::timeout, [=]() {
        if(this->isMarketOpen()) {
            this->RefreshUSStockList();
        }
    });

    // ������ѯ�ʷ�����ʱ��
    m_request.setUrl(QUrl("https://jy.usmartsg.com/user-server-sg/api/get-background-system-server-time/v1"));
    m_replyQueryServerTime = m_manager.post(m_request, QString("").toUtf8());

    // ����Url
    m_preMarketRequest.setUrl(QUrl("https://stock.finance.sina.com.cn/usstock/api/jsonp.php//US_DataCenterService.getPreMarket?page=1&num=200&sort=newchg&asc=0&market=&id="));
    QObject::connect(&m_preMarketManager, &QNetworkAccessManager::finished, [=](QNetworkReply* reply) {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error:" << reply->errorString();
            reply->deleteLater();
            return;
        }
        QByteArray result = reply->readAll();
        QString input(result);
        QString jsonPart;

        int firstPos = input.indexOf("(");
        int lastPos = input.lastIndexOf(")");

        if (firstPos != -1 && lastPos != -1 && firstPos < lastPos) {
            jsonPart = input.mid(firstPos + 1, lastPos - firstPos - 1);
            jsonPart = jsonPart.trimmed();
            if (jsonPart.endsWith(';')) {
                jsonPart = jsonPart.left(jsonPart.size() - 1);
            }

            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonPart.toUtf8());
            if (!jsonDoc.isNull()) {
                QJsonObject jsonObj = jsonDoc.object();
                QJsonArray array = jsonObj["data"].toArray();
                
            }
            else {
                qWarning("Couldn't parse JSON.");
            }
        }
        else {
            qWarning("JSON brackets not found or not balanced.");
        }
    });

    // ��һ���Լ��ɽ�
    m_dragonOneTimer.setInterval(500);
    QObject::connect(&this->m_dragonOneTimer, &QTimer::timeout, [=]() {
        this->m_dragonQueryAfterBuyQty = true;
        this->ExecuteQueryMaxQuantity(m_dragonOneCode, QString("%1").arg(this->m_dragonOnePrice));
    });
}

USmartTradeSystem::~USmartTradeSystem() {
    delete this->m_winCap;
    delete this->m_memoryReadTimer;
    delete this->m_stockRefreshTimer;

    this->m_winCap = nullptr;
    this->m_memoryReadTimer = nullptr;
    this->m_stockRefreshTimer = nullptr;
}

void USmartTradeSystem::ExecuteBuy(const std::string& code,int count, bool isInMarket, float limitPrice) {
    if (!this->m_stockInfoMap.contains(code)) {
        return;
    }
    StockTradeInfo info = this->m_stockInfoMap[code];
    QString rawjson;
    QString json;
    if (isInMarket) {
        if (limitPrice > 0) {
            // �޼۵�
            rawjson = R"({"amountIncrease":"","businessType":"S","conditionPrice":"0.01","conditionType":1,"entrustAmount":%1,"entrustPrice":"%2","entrustProp":"LMT","market":"US","marketPrice":"%3","stockCode":"%4","stockName":"%5","strategyEnddateDesc":12,"tradePeriod":"N"})";
            json = QString(rawjson).arg(count).arg(limitPrice).arg(info.price).arg(info.code).arg(info.name);
        }
        else {
            // �м۵�
            rawjson = R"({"amountIncrease":"","businessType":"S","conditionPrice":"0.01","conditionType":1,"entrustAmount":%1,"entrustGear":5,"entrustPrice":0,"entrustProp":"MKT","market":"US","marketPrice":"%2","stockCode":"%3","stockName":"%4","strategyEnddateDesc":12,"tradePeriod":"N"})";
            json = QString(rawjson).arg(count).arg(info.price).arg(info.code).arg(info.name);
        }
    }
    else {
        // �޼۵�,��һ��
        rawjson = R"({"amountIncrease":"","businessType":"S","conditionPrice":"0.01","conditionType":1,"entrustAmount":%1,"entrustGear":3,"entrustPrice":0,"entrustProp":"LMT","market":"US","marketPrice":"%2","stockCode":"%3","stockName":"%4","strategyEnddateDesc":12,"tradePeriod":"AB"})";
        json = QString(rawjson).arg(count).arg(info.price).arg(info.code).arg(info.name);
    }

    QString time = QString("%0").arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
    m_request.setRawHeader("X-Time", time.toUtf8());
    
    m_request.setUrl(QUrl("https://jy.usmartsg.com/condition-center-sg/api/add-condition/v1"));
    m_replyOp = m_manager.post(m_request, json.toUtf8());
}

void USmartTradeSystem::ExecuteSell(const std::string& code, int count, bool isInMarket, float limitPrice)  {
    if (!this->m_stockInfoMap.contains(code)) {
        return;
    }
    StockTradeInfo info = this->m_stockInfoMap[code];
    QString rawjson;
    QString json;
    if (isInMarket) {
        if (limitPrice > 0) {
            // �޼۵�
            rawjson = R"({"amountIncrease":"","businessType":"S","conditionPrice":"0.01","conditionType":3,"entrustAmount":%1,"entrustPrice":"%2","entrustProp":"LMT","market":"US","marketPrice":"%3","stockCode":"%4","stockName":"%5","strategyEnddateDesc":12,"tradePeriod":"N"})";
            json = QString(rawjson).arg(count).arg(limitPrice).arg(info.price).arg(info.code).arg(info.name);
        }
        else {
            // �м۵�
            rawjson = R"({"amountIncrease":"","businessType":"S","conditionPrice":"0.01","conditionType":3,"entrustAmount":%1,"entrustGear":5,"entrustPrice":0,"entrustProp":"MKT","market":"US","marketPrice":"%2","stockCode":"%3","stockName":"%4","strategyEnddateDesc":12,"tradePeriod":"N"})";
            json = QString(rawjson).arg(count).arg(info.price).arg(info.code).arg(info.name);
        }
    }
    else {
        // �޼۵�,��һ��
        rawjson = R"({"amountIncrease":"","businessType":"S","conditionPrice":"0.01","conditionType":3,"entrustAmount":%1,"entrustGear":3,"entrustPrice":0,"entrustProp":"LMT","market":"US","marketPrice":"%2","stockCode":"%3","stockName":"%4","strategyEnddateDesc":12,"tradePeriod":"AB"})";
        json = QString(rawjson).arg(count).arg(info.price).arg(info.code).arg(info.name);
    }

    QString time = QString("%0").arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
    m_request.setRawHeader("X-Time", time.toUtf8());

    m_request.setUrl(QUrl("https://jy.usmartsg.com/condition-center-sg/api/add-condition/v1"));
    m_replyOp = m_manager.post(m_request, json.toUtf8());
}

void USmartTradeSystem::ExecuteOperationEx(const std::string& code, float price, int count, bool isBuy, bool isInMarket) {
    QString time = QString("%0").arg(QDateTime::currentMSecsSinceEpoch() + this->m_timeStamp);
    m_request.setRawHeader("X-Time", time.toUtf8());
    m_request.setUrl(QUrl("https://jy.usmartsg.com/order-center-sg/api/order/stock-order-create/v1"));
    m_replyOp = m_manager.post(m_request, QString(R"({"entrustPrice":"2.9","entrustProp":"LMT","entrustQty":1,"entrustSide":"B","entrustWay":"NET","forceEntrustFlag":false,"market":"US","symbol":"REBN","tradePeriod":"AB","exchangeCode":"SMART","entrustMode":""})").toUtf8());
}

void USmartTradeSystem::ExecuteQueryPosition() {
    std::string json = "{ \"accountBusinessType\":100,\"moneyType\":\"USD\"}";
    QString time = QString("%0").arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
    m_request.setRawHeader("X-Time", time.toUtf8());
    m_request.setHeader(QNetworkRequest::CookieHeader, QByteArray());

    m_request.setUrl(QUrl("https://jy.usmartsg.com/asset-center-sg/api/app-stockHoldAsset/v5"));
    m_replyQueryAsset = m_manager.post(m_request,QString::fromStdString(json).toUtf8());
}

void USmartTradeSystem::ExecuteQueryTodayOrder() {
    std::string json = "{ \"market\":\"US\" }";
    QString time = QString("%0").arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
    m_request.setRawHeader("X-Time", time.toUtf8());
    m_request.setUrl(QUrl("https://jy.usmartsg.com/order-center-sg/api/order/stock-order-today-list/v1"));
    m_request.setHeader(QNetworkRequest::CookieHeader, QByteArray());

    m_replyQueryAsset = m_manager.post(m_request, QString::fromStdString(json).toUtf8());
}

void USmartTradeSystem::ExecuteQueryMaxQuantity(const QString& code, const QString& price) {
    QString json("{\"handQty\":1,\"entrustPrice\":\"%0\",\"symbol\":\"%1\",\"market\":\"US\"}");
    QString time = QString("%0").arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
    m_request.setRawHeader("X-Time", time.toUtf8());
    json = json.arg(price).arg(code);
    m_request.setUrl(QUrl("https://jy.usmartsg.com/order-center-sg/api/order/stock-order-max-qty-get/v1"));
    m_request.setHeader(QNetworkRequest::CookieHeader, QByteArray());

    m_replyMaxCount = m_manager.post(m_request, json.toUtf8());
}

void USmartTradeSystem::ExecuteQueryShortMaxQuantity(const QString& code, const QString& price) {
    QString json("{\"handQty\":1,\"entrustPrice\":\"%0\",\"symbol\":\"%1\",\"market\":\"US\"}");
    QString time = QString("%0").arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
    m_request.setRawHeader("X-Time", time.toUtf8());
    json = json.arg(price).arg(code);
    m_request.setUrl(QUrl("https://jy.usmartsg.com/order-center-sg/api/order/short-order-max-qty-get/v1"));
    m_request.setHeader(QNetworkRequest::CookieHeader, QByteArray());

    m_replyShortMaxCount = m_manager.post(m_request, json.toUtf8());
}

void USmartTradeSystem::RefreshUSStockList() {
    if (m_replyListGet != nullptr) {
        return;
    }
    QString url = "http://72.push2.eastmoney.com/api/qt/clist/get?cb=&pn=1&pz=200000&po=1&np=1&ut=bd1d9ddb04089700cf9c27f6f7426281&fltt=2&invt=2&wbp2u=2710255628112086|0|1|0|web&fid=f3&fs=m:105,m:106,m:107&fields=f2,f3,f6,f8,f12,f14,f124,f292&_=1715054538673";
    m_listGetRequest.setUrl(QUrl(url));
    m_replyListGet = m_listGetManager.get(m_listGetRequest);
}

void USmartTradeSystem::ExecuteRevoke(const std::string& orderId) {
    std::string json = std::format("{\"orderId\":\"{}\"}",orderId);
    QString time = QString("%0").arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
    m_request.setRawHeader("X-Time", time.toUtf8());
    m_request.setUrl(QUrl("/order-center-sg/api/order/stock-order-cancel/v1"));
    m_request.setHeader(QNetworkRequest::CookieHeader, QByteArray());

    m_replyQueryAsset = m_manager.post(m_request, QString::fromStdString(json).toUtf8());
}
