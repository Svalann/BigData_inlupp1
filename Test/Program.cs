using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Test
{
    class Program
    {
        static void Main(string[] args)
        {
            IDictionary<string, IList<int>> deviceIdDict = new Dictionary<string, IList<int>>();
            IList<int> intList = new List<int>() { 1, 20, 20, 20 };

            string deviceId = messageFromCosmosDB.idOriginal;
            if (!deviceIdDict.ContainsKey(deviceId))
                deviceIdDict.Add(deviceId, intList);

            int ListPosition = deviceIdDict[deviceId][0];

            if (ListPosition == 4)
                ListPosition = 1;

            deviceIdDict[deviceId][ListPosition] = messageFromCosmosDB.dht[0];
            ListPosition++;
        }
    }
}
