/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: SerializationManager.cs
** Desc: 序列化工具类，负责将App设置等保存到本地磁盘
****************************************************************************/
using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;

namespace LampyrisUIStockTradeHelper.Managed
{
    public class SerializationManager
    {
        private static string ms_SerializedDataSavePath
        {
            get
            {
                string path = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "LampyrisUSStockTradeHelper");
                if (!Directory.Exists(path))
                {
                    Directory.CreateDirectory(path);
                }
                return path;
            }
        }

        private List<object> m_serializableObjectList = new List<object>();

        public void OnDestroy()
        {
            BinaryFormatter bin = new BinaryFormatter();
            foreach (object serializableObject in m_serializableObjectList)
            {
                string filePath = Path.Combine(ms_SerializedDataSavePath, serializableObject.GetType().Name + ".bin");
                using (Stream stream = File.Open(filePath, FileMode.OpenOrCreate))
                {
                    bin.Serialize(stream, serializableObject);
                }
            }
        }

        public void OnStart()
        {

        }

        // Unused
        public void OnUpdate(float dTime)
        {

        }

        public void Register(object serializableObject)
        {
            if (serializableObject != null)
            {
                m_serializableObjectList.Add(serializableObject);
            }
        }

        public T TryDeserializeObjectFromFile<T>()
        {
            string filePath = Path.Combine(ms_SerializedDataSavePath, typeof(T).Name + ".bin");

            if (File.Exists(filePath))
            {
                using (Stream stream = File.Open(filePath, FileMode.Open))
                {
                    try
                    {
                        BinaryFormatter bin = new BinaryFormatter();
                        return (T)bin.Deserialize(stream);
                    }
                    catch (SerializationException ex)
                    {
                        return default(T);
                    }
                }
            }

            return default(T);
        }
    }
}