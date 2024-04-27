/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: Singleton.h
** Desc: �������ʵ��
****************************************************************************/

#pragma once

#include <type_traits>
#include <concepts>

template<class T>
class Singleton {
public:
	static T* GetInstance() {
		static T t;
		return &t;
	}
};