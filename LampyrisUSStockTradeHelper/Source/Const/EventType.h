/****************************************************************************
**
** Copyright(C) 2024 HongJin Investment Cooperation
* *Contact: gameta@qq.com
* *
**File: EventType.h
* *Desc : 事件类型定义
* ***************************************************************************/

enum EventType {
	ResUSmartOp = 1, // 响应: USmart 请求买卖
	ResUSmartRevoke = 2, // 响应: USmart 请求撤单
	ResUSmartQueryAsset = 3, // 响应: USmart 持仓查询
	ResUsmartQueryMaxCount = 4, // 响应: USmart 查询最大可操作数量
	ResUsmartQueryTodayOrder = 5, // 响应: USmart 查询本日订单

	RefreshStockInfo = 6, // 刷新当前查看的股票信息
	RefreshAddress = 7, // 刷新当前定位股票简称的地址
	RefreshTempStopStock = 8, // 刷新当前熔断的股票

	ResUsmartQueryShortMaxCount = 9, // 响应: USmart 查询最大沽空可操作数量

};
