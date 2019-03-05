

// Kopia av Azure Function - MyHubMessageRelay

using System;
using System.Text;
using Newtonsoft.Json;
using System.Data.SqlClient;

public class DeviceMessage
{
    public string idOriginal { get; set; }
    public string type { get; set; }
    public string student { get; set; }
    public string[] position { get; set; }
    public float[] dht { get; set; }
}

static var dbConnectionstring = @"Server=tcp:viktorsiotdb18server.database.windows.net,1433;Initial Catalog=Iot18-BigData-DB;Persist Security Info=False;User ID=viktor;Password=Bananakaka17;MultipleActiveResultSets=False;Encrypt=True;TrustServerCertificate=False;Connection Timeout=30;";

public static void Run(string myIoTHubMessage, out string AllMessagesFromClass, out string MessagesFromMe, out string CorruptedMessages, TraceWriter log)
{
    var newIoTHubMessage = myIoTHubMessage.Replace("id", "idOriginal");
    var messageFromDevice = JsonConvert.DeserializeObject<DeviceMessage>(newIoTHubMessage);

    if (MessageIsCorrect(messageFromDevice))
    {
        using (SqlConnection conn = new SqlConnection(dbConnectionstring))
        {
            var locationId = 0;
            var devicetypeId = 0;
            var studentId = 0;
            var deviceId = 0;

            var LocationsSql = "IF NOT EXISTS (SELECT 1 FROM [Locations] WHERE [Longitude] = @devicelongitude AND [Latitude] = @devicelatitude) INSERT INTO[Locations] ([Longitude], [Latitude]) output INSERTED.LocationId VALUES(@devicelongitude, @devicelatitude) ELSE SELECT LocationId FROM[Locations] WHERE[Longitude] = @devicelongitude AND[Latitude] = @devicelatitude";

            var DeviceTypesSql = "IF NOT EXISTS (SELECT 1 FROM [DeviceTypes] WHERE [DeviceType] = @devicetype) INSERT INTO [DeviceTypes] ([DeviceType]) output INSERTED.DeviceTypeId VALUES(@devicetype) ELSE SELECT DeviceTypeId FROM [DeviceTypes] WHERE [DeviceType] = @devicetype";

            var StudentsSql = "IF NOT EXISTS (SELECT 1 FROM [Students] WHERE [StudentName] = @student) INSERT INTO [Students] ([StudentName]) output INSERTED.StudentId VALUES(@student) ELSE SELECT StudentId FROM [Students] WHERE [StudentName] = @student";

            var DevicesSql = "IF NOT EXISTS (SELECT 1 FROM [Devices] WHERE [MacAdress] = @macadress) INSERT INTO [Devices] ([MacAdress], [DeviceTypeId], [LocationId], [StudentId]) output INSERTED.DeviceId VALUES(@macadress, @devicetypeid, @devicelocationid, @studentid) ELSE SELECT DeviceId FROM [Devices] WHERE [MacAdress] = @macadress";

            var MessagesSql = "INSERT INTO [Messages] ([DeviceId], [Temperature], [Humidity], [Created]) VALUES (@deviceid, @temperature, @humidity, @created)";

            conn.Open();

            // Creating Location if not exists
            using (SqlCommand cmd = new SqlCommand(LocationsSql, conn))
            {
                cmd.Parameters.AddWithValue("@devicelatitude", messageFromDevice.position[0]);
                cmd.Parameters.AddWithValue("@devicelongitude", messageFromDevice.position[1]);
                locationId = (int)cmd.ExecuteScalar();
            }

            // Creating Device Type if not exists
            using (SqlCommand cmd = new SqlCommand(DeviceTypesSql, conn))
            {
                cmd.Parameters.AddWithValue("@devicetype", messageFromDevice.type);
                devicetypeId = (int)cmd.ExecuteScalar();
            }

            // Creating Student if not exists
            using (SqlCommand cmd = new SqlCommand(StudentsSql, conn))
            {
                cmd.Parameters.AddWithValue("@student", messageFromDevice.student);
                studentId = (int)cmd.ExecuteScalar();
            }

            // Creating Device if not exists
            using (SqlCommand cmd = new SqlCommand(DevicesSql, conn))
            {
                cmd.Parameters.AddWithValue("@macadress", messageFromDevice.idOriginal);
                cmd.Parameters.AddWithValue("@devicetypeid", devicetypeId);
                cmd.Parameters.AddWithValue("@devicelocationid", locationId);
                cmd.Parameters.AddWithValue("@studentid", studentId);
                deviceId = (int)cmd.ExecuteScalar();
            }

            // Creating new Message
            using (SqlCommand cmd = new SqlCommand(MessagesSql, conn))
            {
                cmd.Parameters.AddWithValue("@deviceid", deviceId);
                cmd.Parameters.AddWithValue("@temperature", messageFromDevice.dht[0]);
                cmd.Parameters.AddWithValue("@humidity", messageFromDevice.dht[1]);
                cmd.Parameters.AddWithValue("@created", DateTime.UtcNow);
                cmd.ExecuteNonQuery();
            }
            conn.Close();
        }

        if (messageFromDevice.student == "Viktor")
        {
            log.Info($"C# \nSaving in MyMessages and ClassMessages \n");
            AllMessagesFromClass = newIoTHubMessage;
            MessagesFromMe = newIoTHubMessage;
            CorruptedMessages = null;
        }
        else
        {
            log.Info($"C# \nSaving in ClassMessages only\n");
            AllMessagesFromClass = newIoTHubMessage;
            MessagesFromMe = null;
            CorruptedMessages = null;
        }

    }
    else
    {
        log.Info($"C# \nSaving corrutped message in Garbage\n");
        CorruptedMessages = newIoTHubMessage;
        AllMessagesFromClass = null;
        MessagesFromMe = null;
    }


    log.Info($"C# IoT Hub trigger function processed a message: {myIoTHubMessage}");
}

static bool MessageIsCorrect(DeviceMessage msg)
{
    if (msg.idOriginal == null)
        return false;
    if (msg.type == null)
        return false;
    if (msg.student == null)
        return false;
    if (msg.position == null)
        return false;
    if (msg.dht == null)
        return false;
    if (msg.dht[0] == 0 || msg.dht[1] == 0)
        return false;
    return true;
}
