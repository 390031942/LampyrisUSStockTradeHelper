/****************************************************************************
**
** Copyright(C) 2024 HongJin Investment Cooperation
* *Contact: gameta@qq.com
* *
**File: EventType.h
* *Desc : �¼����Ͷ���
* ***************************************************************************/

enum EventType {
	ResUSmartOp = 1, // ��Ӧ: USmart ��������
	ResUSmartRevoke = 2, // ��Ӧ: USmart ���󳷵�
	ResUSmartQueryAsset = 3, // ��Ӧ: USmart �ֲֲ�ѯ
	ResUsmartQueryMaxCount = 4, // ��Ӧ: USmart ��ѯ���ɲ�������
	ResUsmartQueryTodayOrder = 5, // ��Ӧ: USmart ��ѯ���ն���

	RefreshStockInfo = 6, // ˢ�µ�ǰ�鿴�Ĺ�Ʊ��Ϣ
	RefreshAddress = 7, // ˢ�µ�ǰ��λ��Ʊ��Ƶĵ�ַ
	RefreshTempStopStock = 8, // ˢ�µ�ǰ�۶ϵĹ�Ʊ

	ResUsmartQueryShortMaxCount = 9, // ��Ӧ: USmart ��ѯ�����տɲ�������

};
