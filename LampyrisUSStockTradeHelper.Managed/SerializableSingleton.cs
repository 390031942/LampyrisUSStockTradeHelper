/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: SerializableSingleton.cs
** Desc: 可序列化的单例类，比如App设置等
****************************************************************************/
using System;
using System.Reflection;

namespace LampyrisUIStockTradeHelper.Managed
{
    public interface IPostSerializationHandler
    {
        void PostSerialization();
    }

    [Serializable]
    public class SerializableSingleton<T> where T : class, new()
    {
        private static T ms_instance;

        public static T Instance
        {
            get
            {
                if (ms_instance == null)
                {
                    ms_instance = new T();
                }
                else
                {
                    // 检查类型T是否实现了IABC接口
                    if (typeof(IPostSerializationHandler).IsAssignableFrom(typeof(T)))
                    {
                        MethodInfo method = typeof(T).GetMethod("PostSerialization");
                        if (method != null)
                        {
                            method.Invoke(ms_instance, null); // 通过反射调用PostSerialization方法
                        }
                    }
                }
                return ms_instance;
            }
        }

        public virtual void PostSerialization() { }
    }
}