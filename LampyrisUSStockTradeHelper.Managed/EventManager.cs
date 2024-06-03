/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: EventManager.cs
** Desc: 事件管理器
****************************************************************************/

using System.Runtime.CompilerServices;

namespace LampyrisUSStockTradeHelper.Managed
{
    public static class EventManager
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void RaiseEvent(int eventCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AddEventHandlerInternal(int eventCode,int callbackId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void RemoveEventHandlerInternal(int eventCode,int callbackId);
    }
}
