/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: HttpRequest.cs
** Desc: 实现了异步网络请求的功能，方便C++调用并获取响应的结果
****************************************************************************/

using System;
using System.IO;
using System.Net.Http.Headers;
using System.Net.Http;
using System.Threading.Tasks;
using System.Collections.Generic;
using System.Text;
using System.Runtime.CompilerServices;

namespace LampyrisUSStockTradeHelper.Managed    
{
    public class TaskContainer<T> where T : class
    {
        private int m_counter = 1;

        private Dictionary<int, Task<T>> m_taskMap = new Dictionary<int, Task<T>>();

        public bool IsCompleted(int id)
        {
            return m_taskMap.ContainsKey(id) ? m_taskMap[id].IsCompleted : false;
        }

        public int Add(Task<T> task)
        {
            int counter = m_counter++;
            m_taskMap.Add(counter, task);
            return counter;
        }
    }
    
    public class DefaultRequestHeader
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern KeyValuePair<string, string>[] GetDefaultRequestHeaders();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern KeyValuePair<string, string> GetAuthorization();
    }

    public static class HttpRequest
    {
        private static HttpClient ms_client = null;

        private static TaskContainer<HttpResponseMessage> ms_taskContainer = new TaskContainer<HttpResponseMessage>();

        public static void Initialize()
        {
            ms_client = new HttpClient();

            // 设置请求的Headers  
            foreach(var pair in DefaultRequestHeader.GetDefaultRequestHeaders())
            {
                ms_client.DefaultRequestHeaders.Add(pair.Key, pair.Value);
            }

            // var authentication = DefaultRequestHeader.GetAuthorization();
            // ms_client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue(authentication.Key, authentication.Value); 
        }

        public static void Finalialize()
        {
            if(ms_client != null)
            {
                ms_client.Dispose();
                ms_client = null;
            }
        }

        private static string GZipDecompress(Stream webStream)
        {
            using (System.IO.Compression.GZipStream stream = new System.IO.Compression.GZipStream(webStream, System.IO.Compression.CompressionMode.Decompress))
            {
                stream.Flush();
                byte[] decompressBuffer = ToByteArray(stream);
                int nSizeIncept = decompressBuffer.Length;
                stream.Close();
                return System.Text.Encoding.UTF8.GetString(decompressBuffer, 0, nSizeIncept);
            }
        }

        private static byte[] ToByteArray(Stream stream)
        {
            byte[] buffer = new byte[32768];
            using (MemoryStream ms = new MemoryStream())
            {
                while (true)
                {
                    int read = stream.Read(buffer, 0, buffer.Length);
                    if (read <= 0)
                        return ms.ToArray();
                    ms.Write(buffer, 0, read);
                }
            }
        }

        public static int GetAsync(string url)
        {
            return ms_taskContainer.Add(ms_client.GetAsync(url));
        }

        public static int PostAsync(string url, string json)
        {
            var content = new StringContent(json, Encoding.UTF8, "application/json");
            return ms_taskContainer.Add(ms_client.PostAsync(url, content));
        }

        public static HttpResponseMessage GetSync(string url)
        {
            return ms_client.GetAsync(url).Result;
        }

        public static HttpResponseMessage PostSync(string url, string json)
        {
            var content = new StringContent(json, Encoding.UTF8, "application/json");
            return ms_client.PostAsync(url, content).Result;
        }

        static async Task Main(string[] args)
        {
            // 创建HttpClient实例
            using (HttpClient client = new HttpClient())
            {
                // 设置请求的URL
                string url = "https://hz.usmartsg.com/quotes-selfstock/api/v2/getgroup?version=0";
                
                // 发送GET请求
                HttpResponseMessage response = await client.GetAsync(url);

                // 检查响应状态码
                if (response.IsSuccessStatusCode)
                {
                    // 读取响应内容
                    Stream content = await response.Content.ReadAsStreamAsync();
                    string str = GZipDecompress(content);
                }
                else
                {
                    Console.WriteLine($"Error: {response.StatusCode}");
                }
            }
        }
    }
}
