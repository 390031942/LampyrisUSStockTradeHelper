/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: EventManager.h
** Desc: 事件管理器
****************************************************************************/
#pragma once
#include <Base/Singleton.h>
#include <Const/EventType.h>
#include <set>
#include <unordered_map>

typedef std::function<void(void**)> PFN_EventCallback;

struct FunctionEqual {
	bool operator()(const std::function<void(void**)>& lhs,
		const std::function<void(void**)>& rhs) const {
		// 比较函数指针是否相等
		return lhs.target<void(void**)>() == rhs.target<void(void**)>();
	}
};

class EventManager:public Singleton<EventManager> {
private:
	std::unordered_map <EventType, std::set<PFN_EventCallback, FunctionEqual>> m_eventMap;
public:
	void AddEventHandler(EventType type, PFN_EventCallback callback) {
		if (callback == nullptr)
			return;
		PFN_EventCallback a;
		PFN_EventCallback b;
		auto& set = m_eventMap[type];
		if (!set.contains(callback)) {
			set.insert(callback);
		}
	}

	void RemoveEventHandler(EventType type, PFN_EventCallback callback) {
		if (callback == nullptr)
			return;

		auto& set = m_eventMap[type];
		if (!set.contains(callback)) {
			set.insert(callback);
		}
	}

	void RaiseEvent(EventType type, void* params[]) {
		auto& set = m_eventMap[type];
		for (PFN_EventCallback callback : set) {
			callback(params);
		}
	}
};