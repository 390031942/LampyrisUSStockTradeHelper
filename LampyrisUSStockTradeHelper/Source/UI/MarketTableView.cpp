/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: MarketTableView.cpp
** Desc: 市场全景图
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

    setItemDelegateForColumn(1, new MarketTableViewImageDelegate(this)); // 设置第二列的委托
    setItemDelegateForColumn(2, new MarketTableViewImageDelegate(this)); // 设置第三列的委托

    // 确保单元格大小正确
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // 设置自适应行高
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
