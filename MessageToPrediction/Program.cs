using System;
using System.Collections.Generic;
using System.IO;
using System.Net.Http;
using System.Net.Http.Formatting;
using System.Net.Http.Headers;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

public class StringTable
{
    public string[] ColumnNames { get; set; }
    public string[,] Values { get; set; }
}

public class ResponseObject
{
    public Results Results { get; set; }
}

public class Results
{
    public Output1 output1 { get; set; }
}

public class Output1
{
    public string type { get; set; }
    public Value value { get; set; }
}

public class Value
{
    public string[] ColumnNames { get; set; }
    public string[] ColumnTypes { get; set; }
    public string[][] Values { get; set; }
}

public class DeviceMessage
{
    public string idOriginal { get; set; }
    public string type { get; set; }
    public string student { get; set; }
    public string[] position { get; set; }
    public float[] dht { get; set; }
}

public class PredictionData
{
    public string temperature { get; set; }
    public string humidity { get; set; }
    public string DoesItRain { get; set; }
    public string Probability { get; set; }
    public bool CalculationConfirmed { get; set; }
}


public static void Run(string myIoTHubMessage, out string PredictionCosmos, TraceWriter log)
{
    log.Info($"C# IoT Hub trigger function processed a message: {myIoTHubMessage}");

    var predictionData = new PredictionData();
    predictionData.CalculationConfirmed = false;

    InvokeRequestResponseService(myIoTHubMessage, predictionData).Wait();

    if (predictionData.CalculationConfirmed == true)
    {
        string predictedData = JsonConvert.SerializeObject(predictionData);
        PredictionCosmos = predictedData;
    }
    else
    {
        PredictionCosmos = null;
    }

}

static async Task InvokeRequestResponseService(string myIoTHubMessage, PredictionData predictionData)
{
    var messageFromDevice = JsonConvert.DeserializeObject<DeviceMessage>(myIoTHubMessage);

    using (var client = new HttpClient())
    {
        var scoreRequest = new
        {

            Inputs = new Dictionary<string, StringTable>() {
                {
                    "input1",
                    new StringTable()
                    {
                        ColumnNames = new string[] {"temperature", "humidity"},
                        Values = new string[,] { { messageFromDevice.dht[0].ToString(), messageFromDevice.dht[1].ToString() } }
                    }
                },
            },
            GlobalParameters = new Dictionary<string, string>()
            {
            }
        };

        const string apiKey = "d6PFL1k+OX0g89kAwZdQ66BGQPuUhf+DDn0yrcPoj0ZYB+5IbbNJyrQIh6C80+bUb7/LXHEhvrfhKtlQgnK5Kg=="; // Replace this with the API key for the web service
        client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", apiKey);

        client.BaseAddress = new Uri("https://ussouthcentral.services.azureml.net/workspaces/ff8d6dabce37408d9bc3ef48e357ad45/services/1bae496b0c874446916b3df10110ed48/execute?api-version=2.0&details=true");


        HttpResponseMessage response = await client.PostAsJsonAsync("", scoreRequest);


        if (response.IsSuccessStatusCode)
        {
            string result = await response.Content.ReadAsStringAsync();
            //Console.WriteLine("Result: {0}", result);
            var responseobj = JsonConvert.DeserializeObject<ResponseObject>(result);

            predictionData.temperature = responseobj.Results.output1.value.Values[0][0];
            predictionData.humidity = responseobj.Results.output1.value.Values[0][1];
            predictionData.DoesItRain = responseobj.Results.output1.value.Values[0][2];
            predictionData.Probability = responseobj.Results.output1.value.Values[0][3];
            predictionData.CalculationConfirmed = true;
        }
    }
}