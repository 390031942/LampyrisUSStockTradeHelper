#include <QApplication>
#include <UI/TradeOperationPanel.h>
#include <Module/USmartTradeSystem.h>
#include <UI/MarketTableView.h>
#include <Module/SystemTrayIcon.h>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    USmartTradeSystem::GetInstance()->Initialize();
    TradeOperationPanel w;
    SystemTrayIcon::GetInstance()->Show();
    w.show();
    return a.exec();
}