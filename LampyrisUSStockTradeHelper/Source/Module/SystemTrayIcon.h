/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: SystemTrayIcon.h
** Desc: ����ͼ��
****************************************************************************/
#pragma once
#include <Base/Singleton.h>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QApplication>

class SystemTrayIcon :public Singleton<SystemTrayIcon> {
private:
	QSystemTrayIcon* m_trayIcon;
public:
	SystemTrayIcon():m_trayIcon(new QSystemTrayIcon){
		QObject::connect(m_trayIcon, &QSystemTrayIcon::activated, [=]() {	

		});
		QMenu* trayIconMenu = new QMenu();
		trayIconMenu->addAction("�˳�", [=]() {
			QApplication::quit();
		});

		m_trayIcon->setIcon(QIcon(":/ImageRes/Icon.png"));
	}

	void ShowMessage(const QString& title,const QString& content) {
		m_trayIcon->showMessage(title, content);
	}

	void Show() {
		m_trayIcon->show();
	}
};