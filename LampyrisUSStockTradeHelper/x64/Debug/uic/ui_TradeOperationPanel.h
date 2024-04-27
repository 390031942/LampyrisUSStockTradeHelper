/********************************************************************************
** Form generated from reading UI file 'TradeOperationPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRADEOPERATIONPANEL_H
#define UI_TRADEOPERATIONPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TradeOperationPanel
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *LabelStockCode;
    QLabel *LabelStockName;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *ButtonBuy;
    QPushButton *ButtonSell;
    QPushButton *ButtonBuyEmpty;
    QPushButton *ButtonSellEmpty;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_5;
    QRadioButton *Radio2;
    QRadioButton *RadioAll;
    QRadioButton *Radio3;
    QRadioButton *Radio4;
    QTabWidget *tabWidget;
    QWidget *TabRevoke;
    QHBoxLayout *horizontalLayout_3;
    QTableView *TableRevoke;
    QWidget *TabPosition;
    QHBoxLayout *horizontalLayout_4;
    QTableView *TablePosition;

    void setupUi(QWidget *TradeOperationPanel)
    {
        if (TradeOperationPanel->objectName().isEmpty())
            TradeOperationPanel->setObjectName(QString::fromUtf8("TradeOperationPanel"));
        TradeOperationPanel->resize(348, 248);
        verticalLayout = new QVBoxLayout(TradeOperationPanel);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QWidget(TradeOperationPanel);
        widget->setObjectName(QString::fromUtf8("widget"));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font.setPointSize(7);
        font.setBold(false);
        font.setWeight(50);
        widget->setFont(font);
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, 0);
        LabelStockCode = new QLabel(widget);
        LabelStockCode->setObjectName(QString::fromUtf8("LabelStockCode"));
        LabelStockCode->setFont(font);

        horizontalLayout->addWidget(LabelStockCode);

        LabelStockName = new QLabel(widget);
        LabelStockName->setObjectName(QString::fromUtf8("LabelStockName"));
        LabelStockName->setFont(font);

        horizontalLayout->addWidget(LabelStockName);


        verticalLayout->addWidget(widget);

        widget_2 = new QWidget(TradeOperationPanel);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setFont(font);
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, 0);
        ButtonBuy = new QPushButton(widget_2);
        ButtonBuy->setObjectName(QString::fromUtf8("ButtonBuy"));
        ButtonBuy->setMaximumSize(QSize(60, 16777215));
        ButtonBuy->setFont(font);

        horizontalLayout_2->addWidget(ButtonBuy);

        ButtonSell = new QPushButton(widget_2);
        ButtonSell->setObjectName(QString::fromUtf8("ButtonSell"));
        ButtonSell->setMaximumSize(QSize(60, 16777215));
        ButtonSell->setFont(font);

        horizontalLayout_2->addWidget(ButtonSell);

        ButtonBuyEmpty = new QPushButton(widget_2);
        ButtonBuyEmpty->setObjectName(QString::fromUtf8("ButtonBuyEmpty"));
        ButtonBuyEmpty->setMaximumSize(QSize(60, 16777215));
        ButtonBuyEmpty->setFont(font);

        horizontalLayout_2->addWidget(ButtonBuyEmpty);

        ButtonSellEmpty = new QPushButton(widget_2);
        ButtonSellEmpty->setObjectName(QString::fromUtf8("ButtonSellEmpty"));
        ButtonSellEmpty->setMaximumSize(QSize(60, 16777215));
        ButtonSellEmpty->setFont(font);

        horizontalLayout_2->addWidget(ButtonSellEmpty);


        verticalLayout->addWidget(widget_2);

        widget_3 = new QWidget(TradeOperationPanel);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setMinimumSize(QSize(0, 0));
        horizontalLayout_5 = new QHBoxLayout(widget_3);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(-1, 0, -1, 0);
        Radio2 = new QRadioButton(widget_3);
        Radio2->setObjectName(QString::fromUtf8("Radio2"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font1.setPointSize(7);
        Radio2->setFont(font1);

        horizontalLayout_5->addWidget(Radio2);

        RadioAll = new QRadioButton(widget_3);
        RadioAll->setObjectName(QString::fromUtf8("RadioAll"));
        RadioAll->setFont(font1);

        horizontalLayout_5->addWidget(RadioAll);

        Radio3 = new QRadioButton(widget_3);
        Radio3->setObjectName(QString::fromUtf8("Radio3"));
        Radio3->setFont(font1);

        horizontalLayout_5->addWidget(Radio3);

        Radio4 = new QRadioButton(widget_3);
        Radio4->setObjectName(QString::fromUtf8("Radio4"));
        Radio4->setFont(font1);

        horizontalLayout_5->addWidget(Radio4);


        verticalLayout->addWidget(widget_3);

        tabWidget = new QTabWidget(TradeOperationPanel);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setFont(font);
        TabRevoke = new QWidget();
        TabRevoke->setObjectName(QString::fromUtf8("TabRevoke"));
        horizontalLayout_3 = new QHBoxLayout(TabRevoke);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(3, 3, 3, 3);
        TableRevoke = new QTableView(TabRevoke);
        TableRevoke->setObjectName(QString::fromUtf8("TableRevoke"));

        horizontalLayout_3->addWidget(TableRevoke);

        tabWidget->addTab(TabRevoke, QString());
        TabPosition = new QWidget();
        TabPosition->setObjectName(QString::fromUtf8("TabPosition"));
        horizontalLayout_4 = new QHBoxLayout(TabPosition);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(3, 3, 3, 3);
        TablePosition = new QTableView(TabPosition);
        TablePosition->setObjectName(QString::fromUtf8("TablePosition"));

        horizontalLayout_4->addWidget(TablePosition);

        tabWidget->addTab(TabPosition, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(TradeOperationPanel);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(TradeOperationPanel);
    } // setupUi

    void retranslateUi(QWidget *TradeOperationPanel)
    {
        TradeOperationPanel->setWindowTitle(QCoreApplication::translate("TradeOperationPanel", "Form", nullptr));
        LabelStockCode->setText(QCoreApplication::translate("TradeOperationPanel", "\350\202\241\347\245\250\347\274\251\345\206\231", nullptr));
        LabelStockName->setText(QCoreApplication::translate("TradeOperationPanel", "\350\202\241\347\245\250\345\205\250\347\247\260", nullptr));
        ButtonBuy->setText(QCoreApplication::translate("TradeOperationPanel", "\344\271\260\345\205\245", nullptr));
        ButtonSell->setText(QCoreApplication::translate("TradeOperationPanel", "\345\215\226\345\207\272", nullptr));
        ButtonBuyEmpty->setText(QCoreApplication::translate("TradeOperationPanel", "\346\262\275\347\251\272", nullptr));
        ButtonSellEmpty->setText(QCoreApplication::translate("TradeOperationPanel", "\345\271\263\344\273\223", nullptr));
        Radio2->setText(QCoreApplication::translate("TradeOperationPanel", "\346\242\255\345\223\210", nullptr));
        RadioAll->setText(QCoreApplication::translate("TradeOperationPanel", "1/2\344\273\223", nullptr));
        Radio3->setText(QCoreApplication::translate("TradeOperationPanel", "1/3\344\273\223", nullptr));
        Radio4->setText(QCoreApplication::translate("TradeOperationPanel", "1/4\344\273\223", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(TabRevoke), QCoreApplication::translate("TradeOperationPanel", "\346\222\244\345\215\225", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(TabPosition), QCoreApplication::translate("TradeOperationPanel", "\346\214\201\344\273\223", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TradeOperationPanel: public Ui_TradeOperationPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRADEOPERATIONPANEL_H
