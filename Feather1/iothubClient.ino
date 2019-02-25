static WiFiClientSecure sslClient; // for ESP8266

const char *onSuccess = "\"Successfully invoke device method\"";
const char *notFound = "\"No method found\"";
const char *gitLink = "https://github.com/Svalann/BigData_inlupp1.git";


static void sendCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void *userContextCallback)
{
    if (IOTHUB_CLIENT_CONFIRMATION_OK == result)
    {
        Serial.println("Message sent to Azure IoT Hub");
		blinkRedLed_ON();

		// HighTemp triggered and warning has been sent.
		if (alertStatus && !alertSent && !buttonPushed) 
		{
			alertSent = true;
			nextAlertTime = currentTime + WARNINGINTERVAL;
		}

		// alertMsg sent earlier, now possible to send new HighTemp warning again
		if (alertSent && currentTime >= nextAlertTime || alertIsOver)	
		{
			alertSent = false;
		}

		// back to normal sent.
		if (alertIsOver)	
		{
			alertIsOver = false;
		}
		alertStatus = false;
	}
    else
    {
        Serial.println("Failed to send message to Azure IoT Hub");
    }
    messagePending = false;	
}

static void sendMessage(IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle, char *buffer)
{
    IOTHUB_MESSAGE_HANDLE messageHandle = IoTHubMessage_CreateFromByteArray((const unsigned char *)buffer, strlen(buffer));
    if (messageHandle == NULL)
        Serial.println("Unable to create a new IoTHubMessage.");
    else
    {
        MAP_HANDLE properties = IoTHubMessage_Properties(messageHandle);
        Map_Add(properties, "alertStatus", alertStatus ? "true" : "false");		
        Serial.printf("\nSending message: %s.\r\n", buffer);
        if (IoTHubClient_LL_SendEventAsync(iotHubClientHandle, messageHandle, sendCallback, NULL) != IOTHUB_CLIENT_OK)
            Serial.println("Failed to hand over the message to IoTHubClient.");
        else
        {
            messagePending = true;
            Serial.println("IoTHubClient accepted the message for delivery.");
        }

        IoTHubMessage_Destroy(messageHandle);
    }
}

void Start()
{
    Serial.println("Start sending temperature and humidity data.");
    messageSending = true;
}

void Stop()
{
    Serial.println("Stop sending temperature and humidity data.");
    messageSending = false;
}

IOTHUBMESSAGE_DISPOSITION_RESULT receiveMessageCallback(IOTHUB_MESSAGE_HANDLE message, void *userContextCallback)
{
    IOTHUBMESSAGE_DISPOSITION_RESULT result;
    const unsigned char *buffer;
    size_t size;
    if (IoTHubMessage_GetByteArray(message, &buffer, &size) != IOTHUB_MESSAGE_OK)
    {
        Serial.println("Unable to IoTHubMessage_GetByteArray.");
        result = IOTHUBMESSAGE_REJECTED;
    }
    else
    {
        char *temp = (char *)malloc(size + 1);

        if (temp == NULL)
            return IOTHUBMESSAGE_ABANDONED;

        strncpy(temp, (const char *)buffer, size);
        temp[size] = '\0';

		if (!parseC2DMessage(temp)) // if not Json then stringmessage
			Serial.printf("Receive C2D message: %s.\r\n", temp);
        free(temp);
		blinkGreenLed_ON();
    }
    return IOTHUBMESSAGE_ACCEPTED;
}


int deviceMethodCallback(
    const char *methodName,
    const unsigned char *payload,
    size_t size,
    unsigned char **response,
    size_t *response_size,
    void *userContextCallback)
{
    Serial.printf("Try to invoke method %s.\r\n", methodName);
    const char *responseMessage = onSuccess;
    int result = 200;

    if (strcmp(methodName, "start") == 0 || strcmp(methodName, "Start") == 0)
        Start();

    else if (strcmp(methodName, "stop") == 0 || strcmp(methodName, "Stop") == 0)
        Stop();

	else if (strcmp(methodName, "git") == 0 || strcmp(methodName, "github") == 0 || strcmp(methodName, "gitlink") == 0)
		responseMessage = gitLink;

    else
    {
        Serial.printf("No method %s found.\r\n", methodName);
        responseMessage = notFound;
        result = 404;
    }

    *response_size = strlen(responseMessage);
    *response = (unsigned char *)malloc(*response_size);
    strncpy((char *)(*response), responseMessage, *response_size);

    return result;
}

void twinCallback(
    DEVICE_TWIN_UPDATE_STATE updateState,
    const unsigned char *payLoad,
    size_t size,
    void *userContextCallback)
{
    char *temp = (char *)malloc(size + 1);
    for (int i = 0; i < size; i++)
        temp[i] = (char)(payLoad[i]);
    temp[size] = '\0';
    parseTwinMessage(temp);
    free(temp);
}	

char* GetConnectionString()
{
	HTTPClient http;
	String url = "http://iot18-fa-v2.azurewebsites.net/api/RegisterDevice?code=avXVx943iIavuDYIEbDe3u0JcvJO8VJjSexCzKiydgy5LA8I0HtSMw==&deviceid=" + DEVICE_ID;
	char _connectionString[CONNECTIONSTRING_LEN];
	Serial.println(url);
	http.begin(url); //HTTP
	int httpCode = http.GET();

	if (httpCode > 0) {
		if (httpCode == HTTP_CODE_OK) {
			String payload = http.getString();
			payload.toCharArray(_connectionString, CONNECTIONSTRING_LEN);
		}
	}
	else {
		Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
		while (1);
	}

	http.end();
	return _connectionString;
}

IOTHUB_CLIENT_LL_HANDLE initIotHub()
{
	Serial.println("Connecting to Azure IOT Hub. Please wait...");
	IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;

	iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(GetConnectionString(), MQTT_Protocol);
	if (iotHubClientHandle == NULL)
	{
		Serial.println("ERROR: Failed on IoTHubClient_CreateFromConnectionString.");
		while (1);
	}

	IoTHubClient_LL_SetDeviceMethodCallback(iotHubClientHandle, deviceMethodCallback, NULL);
	IoTHubClient_LL_SetDeviceTwinCallback(iotHubClientHandle, twinCallback, NULL);

	Serial.print("DeviceId: ");
	Serial.println(DEVICE_ID);
	Serial.print("Status: Connected");
	Serial.println("");
	Serial.println("");
	Serial.println("MESSAGE INFORMATION");
	Serial.println("----------------------------------------------------");
	return iotHubClientHandle;
}



