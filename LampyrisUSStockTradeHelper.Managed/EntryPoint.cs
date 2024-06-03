/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: EntryPoint.cs
** Desc: C# DLL的入口类
****************************************************************************/

using System;
using System.Collections.Generic;
using System.IO;

namespace LampyrisUSStockTradeHelper.Managed
{
    public class EntryPoint
    {
        public static void OnStart()
        {
            HttpRequest.Initialize();
        }

        public static void OnDestroy()
        {
            HttpRequest.Finalialize();
        }
    }
}
