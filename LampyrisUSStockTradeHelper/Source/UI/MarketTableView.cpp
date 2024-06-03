/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: MarketTableView.cpp
** Desc: �г�ȫ��ͼ
****************************************************************************/
#include "MarketTableView.h"
#include <QHeaderView>
#include <QStandardItem>

MarketTableView::MarketTableView(QWidget* parent) : QTableView(parent) {
    model = new MarketTableViewModel();
    initializeModel();
    setModel(model);
}

void MarketTableView::initializeModel() {
    setModel(model);

    setItemDelegateForColumn(1, new MarketTableViewImageDelegate(this)); // ���õڶ��е�ί��
    setItemDelegateForColumn(2, new MarketTableViewImageDelegate(this)); // ���õ����е�ί��

    // ȷ����Ԫ���С��ȷ
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // ��������Ӧ�и�
    connect(horizontalHeader(), &QHeaderView::sectionResized, this, [this]() {
        resizeRowsToContents();
    });
}

QPixmap& MarketTableViewEmptyImage::GetEmptyDayImage() {
    static QPixmap pixmap(":/ImageRes/QuoteEmpty1.png");
    return pixmap;
}

QPixmap& MarketTableViewEmptyImage::GetEmptyKLineImage() {
    static QPixmap pixmap(":/ImageRes/QuoteEmpty2.png");
    return pixmap;
}
