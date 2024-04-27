#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LampyrisUSStockTradeHelperMainWindow.h"

class LampyrisUSStockTradeHelperMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    LampyrisUSStockTradeHelperMainWindow(QWidget *parent = nullptr);
    ~LampyrisUSStockTradeHelperMainWindow();

private:
    Ui::LampyrisUSStockTradeHelperMainWindowClass ui;
};
