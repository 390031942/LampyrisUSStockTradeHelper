/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: MarketTableView.h
** Desc: 市场全景图
****************************************************************************/
#pragma once

#include <QTableView>
#include <QAbstractTableModel>
#include <QPixmap>
#include <QItemDelegate>
#include <QPainter>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class MarketTableView: public QTableView {
	Q_OBJECT

public:
	explicit MarketTableView(QWidget* parent = nullptr);
	void initializeModel();

private:
	QAbstractTableModel* model;
};

class MarketTableViewEmptyImage {
public:
    static QPixmap& GetEmptyDayImage();

    static QPixmap& GetEmptyKLineImage();
};

class MarketTableViewStockInfoWidget : public QWidget {
public:
    explicit MarketTableViewStockInfoWidget(QWidget* parent = nullptr)
        : QWidget(parent) {}

};

class MarketTableViewStockInfoDelegate : public QItemDelegate {
    Q_OBJECT

public:
    explicit MarketTableViewStockInfoDelegate(QObject* parent = nullptr)
        : QItemDelegate(parent) {}

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        // 创建并返回一个自定义的 widget
        return new MarketTableViewStockInfoWidget(parent);
    }
};

class MarketTableViewImageDelegate : public QItemDelegate {
private:
    void OnDownloadFinished(QNetworkReply* reply) {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error:" << reply->errorString();   
            reply->deleteLater();
            return;
        }
        QByteArray result = reply->readAll();
        this->m_pixMap = QPixmap(result);
        this->m_ready = true;
    }
public:
    MarketTableViewImageDelegate(QObject* parent = nullptr):QItemDelegate(parent), m_ready(false){
        QObject::connect(&m_manager, &QNetworkAccessManager::finished, this, &MarketTableViewImageDelegate::OnDownloadFinished);
    }

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
        const QModelIndex& index) const override {
        int col = index.column();
        if (col == 1 || col == 2) {
            if (this->m_ready) {
                painter->drawPixmap(option.rect, this->m_pixMap);
            }
            else {
                painter->drawPixmap(option.rect, col == 1 ? MarketTableViewEmptyImage::GetEmptyDayImage() : MarketTableViewEmptyImage::GetEmptyKLineImage());
            }
        }
        else {
            QItemDelegate::paint(painter, option, index);
        }
    }

    // 可以选择重写 sizeHint() 方法以自定义项的大小
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        // 根据需要返回合适的尺寸
        return QSize(470,270);
    }
private:
    QNetworkAccessManager m_manager;
    QPixmap               m_pixMap;
    bool                  m_ready;
};

class MarketTableViewModel : public QAbstractTableModel {
public:
    int rowCount(const QModelIndex& parent) const override {
        return 10; // 假设有10行数据
    }

    int columnCount(const QModelIndex& parent) const override {
        return 4; // 四列数据
    }

    QVariant data(const QModelIndex& index, int role) const override {
        if (role == Qt::DisplayRole) {
            return tr("Data");
        }
        else if (role == Qt::UserRole) {
            if (index.column() == 0 || index.column() == 3) {
                return QVariant();
            }
        }
        return QVariant();
    }

    QWidget* createDelegateForColumn(int column, QWidget* parent) {
        if (column == 0 || column == 3) {
            return new QWidget(parent);
        }
        return nullptr;
    }
};
