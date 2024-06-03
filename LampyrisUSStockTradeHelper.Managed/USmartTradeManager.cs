using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LampyrisUSStockTradeHelper.Managed
{
    public class USmartTradeManager
    {
        private static string ms_orderCreateUrl = "/order-center-sg/api/order/stock-order-create/v1";

        private static string ms_orderCancelUrl = "/order-center-sg/api/order/stock-order-cancel/v1";

        private static string ms_stockHoldAssetUrl = "/asset-center-sg/api/app-stockHoldAsset/v5";

        private static string ms_queryMaxCountUrl = "/order-center-sg/api/order/stock-order-max-qty-get/v1";

        public static string RequestOperationByCountUS(string market,string code,float price,int count,bool isBuy,bool isInMarket)
        {
            string tradePeriod = isInMarket ? "N" : "AB";
            string opCode = isBuy ? "B" : "S";
            string json = "{\n  " +
                         $"\"entrustPrice\": \"{price}\",\n  " +
                          "\"entrustProp\": \"LMT\",\n  " +
                         $"\"entrustQty\": {count},\n  " +
                         $"\"entrustSide\": \"{opCode}\",\n  " +
                          "\"entrustWay\": \"NET\",\n  " +
                          "\"forceEntrustFlag\": false,\n  " +
                          "\"market\": \"US\",\n  " +
                          "\"symbol\": \"BENF\",\n  " +
                         $"\"tradePeriod\": \"{tradePeriod}\",\n  " +
                          "\"exchangeCode\": \"SMART\",\n  " +
                          "\"entrustMode\": \"\"\n}";

            var response = HttpRequest.PostSync(ms_orderCreateUrl,json);
            return response.Content.ReadAsStringAsync().Result;
        }

        public static string RequestQueryPosition(string code, int ratio)
        {
            string json = "{ \"accountBusinessType\":100,\"moneyType\":\"USD\"}";
            var response = HttpRequest.PostSync(ms_stockHoldAssetUrl, json);
            return response.Content.ReadAsStringAsync().Result;
        }

        public static string RequestQueryRevoke(long orderId)
        {
            string json = $"{{\"orderId\":\"{orderId}\"}}";
            var response = HttpRequest.PostSync(ms_orderCancelUrl, json);
            return response.Content.ReadAsStringAsync().Result;
        }

        public static string RequestMaxCanBuySellCount(string market, string code, float price)
        {
            string json = $"{{\"handQty\":1,\"entrustPrice\":\"{price}\",\"symbol\":\"{code}\",\"market\":\"{market}\"}}";
            var response = HttpRequest.PostSync(ms_queryMaxCountUrl, json);
            return response.Content.ReadAsStringAsync().Result;
        }
    }
}
