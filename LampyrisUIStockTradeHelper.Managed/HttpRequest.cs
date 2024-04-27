/****************************************************************************
**
** Copyright (C) 2024 HongJin Investment Cooperation
** Contact: gameta@qq.com
**
** File: MonoScriptingSystem.h
** Desc: 实现了基于Mono框架的C#脚本子系统，支持加载C#程序集，
         调用程序集里定义的函数并获取返回值等功能 
****************************************************************************/

using System;
using System.IO;
using System.Net.Http.Headers;
using System.Net.Http;
using System.Threading.Tasks;
using System.Collections.Generic;

namespace LampyrisUIStockTradeHelper.Managed    
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

    public static class HttpRequest
    {
        private static HttpClient ms_client = null;

        private static TaskContainer<HttpResponseMessage> ms_taskContainer = new TaskContainer<HttpResponseMessage>();

        public static void Initialize(Dictionary<string,string> defaultRequestHeaderMap, 
                                      string authenticationScheme, 
                                      string authenticationParameter)
        {
            ms_client = new HttpClient();

            // 设置请求的Headers  
            foreach(var pair in defaultRequestHeaderMap)
            {
                ms_client.DefaultRequestHeaders.Add(pair.Key, pair.Value);
            }
            ms_client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue(authenticationScheme, authenticationParameter); 
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

        public static int PostAsync(string url,HttpContent httpContent)
        {
            return ms_taskContainer.Add(ms_client.PostAsync(url, httpContent));
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
