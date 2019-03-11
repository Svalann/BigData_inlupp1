using System;
using System.Text;
using System.Collections.Generic;
using Newtonsoft.Json;
using System.Data.SqlClient;

namespace CosmosToSQLPrediction
{

    class Program
    {

        public class MessageFromCosmos
        {
            public string idOriginal { get; set; }
            public string type { get; set; }
            public string student { get; set; }
            public string[] position { get; set; }
            public float[] dht { get; set; }
            public string id { get; set; }
            public string _rid { get; set; }
            public string _self { get; set; }
            public string _etag { get; set; }
            public string _attachments { get; set; }
            public int _ts { get; set; }
        }

        public class GarbageFromCosmos
        {
            public string idOriginal { get; set; }
            public string type { get; set; }
            public string student { get; set; }
            public string[] position { get; set; }
            public float[] dht { get; set; }
        }
        static var dbConnectionstring = @"Server=tcp:viktorsiotdb18server.database.windows.net,1433;Initial Catalog=Iot18-BigData-DB;Persist Security Info=False;User ID=viktor;Password=Bananakaka17;MultipleActiveResultSets=False;Encrypt=True;TrustServerCertificate=False;Connection Timeout=30;";

        static void Main(string[] args)
        {
            IEnumerable <dynamic> MyCosmos;

            log.Info($"C# manually tranferring CosmosDB to SQL");
            foreach (var msg in MyCosmos)
            {
                msg._etag = 0;
                log.Info($"C# Transfering file {msg.id} to SQL-DB ");

                string cosmosMsg = msg.ToString();
                var messageFromCosmosDB = JsonConvert.DeserializeObject<MessageFromCosmos>(cosmosMsg);

                if (messageFromCosmosDB.idOriginal != null && messageFromCosmosDB.type != null && messageFromCosmosDB.student != null && messageFromCosmosDB.position != null && messageFromCosmosDB.dht != null && messageFromCosmosDB.dht[0] != 0 && messageFromCosmosDB.dht[1] != 0)
                {
                    using (SqlConnection conn = new SqlConnection(dbConnectionstring))
                    {
                        var locationId = 0;
                        var devicetypeId = 0;
                        var studentId = 0;
                        var deviceId = 0;
                        string temperatureAlert = (messageFromCosmosDB.dht[0] >= 25 ? "true" : "false");

                        var LocationsSql = "IF NOT EXISTS (SELECT 1 FROM [Locations] WHERE [Longitude] = @devicelongitude AND [Latitude] = @devicelatitude) INSERT INTO[Locations] ([Longitude], [Latitude]) output INSERTED.LocationId VALUES(@devicelongitude, @devicelatitude) ELSE SELECT LocationId FROM[Locations] WHERE[Longitude] = @devicelongitude AND[Latitude] = @devicelatitude";

                        var DeviceTypesSql = "IF NOT EXISTS (SELECT 1 FROM [DeviceTypes] WHERE [DeviceType] = @devicetype) INSERT INTO [DeviceTypes] ([DeviceType]) output INSERTED.DeviceTypeId VALUES(@devicetype) ELSE SELECT DeviceTypeId FROM [DeviceTypes] WHERE [DeviceType] = @devicetype";

                        var StudentsSql = "IF NOT EXISTS (SELECT 1 FROM [Students] WHERE [StudentName] = @student) INSERT INTO [Students] ([StudentName]) output INSERTED.StudentId VALUES(@student) ELSE SELECT StudentId FROM [Students] WHERE [StudentName] = @student";

                        var DevicesSql = "IF NOT EXISTS (SELECT 1 FROM [Devices] WHERE [MacAdress] = @macadress) INSERT INTO [Devices] ([MacAdress], [DeviceTypeId], [LocationId], [StudentId]) output INSERTED.DeviceId VALUES(@macadress, @devicetypeid, @devicelocationid, @studentid) ELSE SELECT DeviceId FROM [Devices] WHERE [MacAdress] = @macadress";

                        var MessagesSql = "INSERT INTO [Messages] ([DeviceId], [Temperature], [Humidity], [Created]) VALUES (@deviceid, @temperature, @humidity, @created)";

                        var PredictionMessagesSql = "INSERT INTO PredictionMessages ([Temperature], [Humidity], [Created], [DeviceId], [temperatureAlert], [averageTempLast5])VALUES (@temperature, @humidity, @created, @deviceId, @temperatureAlert, ISNULL(( SELECT AVG(temp.Temperature) FROM ( SELECT TOP (5) Temperature FROM PredictionMessages WHERE DeviceId = @deviceId ORDER BY [Created] DESC ) temp ), @temperature))";

                        conn.Open();

                        // Creating Location if not exists
                        using (SqlCommand cmd = new SqlCommand(LocationsSql, conn))
                        {
                            cmd.Parameters.AddWithValue("@devicelatitude", messageFromCosmosDB.position[0]);
                            cmd.Parameters.AddWithValue("@devicelongitude", messageFromCosmosDB.position[1]);
                            locationId = (int)cmd.ExecuteScalar();
                        }

                        // Creating Device Type if not exists
                        using (SqlCommand cmd = new SqlCommand(DeviceTypesSql, conn))
                        {
                            cmd.Parameters.AddWithValue("@devicetype", messageFromCosmosDB.type);
                            devicetypeId = (int)cmd.ExecuteScalar();
                        }

                        // Creating Student if not exists
                        using (SqlCommand cmd = new SqlCommand(StudentsSql, conn))
                        {
                            cmd.Parameters.AddWithValue("@student", messageFromCosmosDB.student);
                            studentId = (int)cmd.ExecuteScalar();
                        }

                        // Creating Device if not exists
                        using (SqlCommand cmd = new SqlCommand(DevicesSql, conn))
                        {
                            cmd.Parameters.AddWithValue("@macadress", messageFromCosmosDB.idOriginal);
                            cmd.Parameters.AddWithValue("@devicetypeid", devicetypeId);
                            cmd.Parameters.AddWithValue("@devicelocationid", locationId);
                            cmd.Parameters.AddWithValue("@studentid", studentId);
                            deviceId = (int)cmd.ExecuteScalar();
                        }

                        // Creating new Message
                        using (SqlCommand cmd = new SqlCommand(MessagesSql, conn))
                        {
                            cmd.Parameters.AddWithValue("@deviceid", deviceId);
                            cmd.Parameters.AddWithValue("@temperature", messageFromCosmosDB.dht[0]);
                            cmd.Parameters.AddWithValue("@humidity", messageFromCosmosDB.dht[1]);
                            cmd.Parameters.AddWithValue("@created", FromEpochTimeStamp(messageFromCosmosDB._ts));
                            cmd.ExecuteNonQuery();
                        }
                        // Creating new PredMessage
                        using (SqlCommand cmd = new SqlCommand(PredictionMessagesSql, conn))
                        {
                            cmd.Parameters.AddWithValue("@temperature", messageFromCosmosDB.dht[0]);
                            cmd.Parameters.AddWithValue("@humidity", messageFromCosmosDB.dht[1]);
                            cmd.Parameters.AddWithValue("@created", FromEpochTimeStamp(messageFromCosmosDB._ts));
                            cmd.Parameters.AddWithValue("@deviceid", deviceId);
                            cmd.Parameters.AddWithValue("@temperatureAlert", temperatureAlert);
                            cmd.ExecuteNonQuery();
                        }
                        conn.Close();
                    }
                }
            }
        }

        public static DateTime FromEpochTimeStamp(long epochTime)
        {
            var epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
            return epoch.AddSeconds(epochTime);
        }
    }
}
