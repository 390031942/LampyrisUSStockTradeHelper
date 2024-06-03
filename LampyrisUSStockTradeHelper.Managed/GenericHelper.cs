/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: GenericHelper.cs
** Desc: 事件管理器
****************************************************************************/

using System;
using System.Collections.Generic;

namespace LampyrisUSStockTradeHelper.Managed
{
    public static class GenericHelper
    {
        public static object MakeStringKeyValuePair(string key,string value)
        {
            return new KeyValuePair<string, string>(key, value);
        }

        public static object GetStringKeyValueType()
        {
            return typeof(KeyValuePair<string, string>);
        }

        public static object CreateArray(Type type,int count)
        {
            return Array.CreateInstance(type, count);
        }
    }
}
