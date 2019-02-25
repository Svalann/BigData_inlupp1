static WiFiClientSecure sslClient; // for ESP8266

const char *onSuccess = "\"Successfully invoke device method\"";
const char *notFound = "\"No method found\"";
const char *gitLink = "\"https://github.com/Svalann/BigData_inlupp1.git\"";

static void sendCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void *userContextCallback)
{
    if (IOTHUB_CLIENT_CONFIRMATION_OK == result)
    {
        Serial.println("Message::  Successfully delivered message to Azure IOT Hub.");
        Serial.println("");
        blinkLED();
    }
    else
    {
        Serial.println("Error::  Failed to send message to Azure IoT Hub");
        Serial.println("");
    }
    messagePending = false;
}

static void sendMessage(IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle, char *buffer)
{
    IOTHUB_MESSAGE_HANDLE messageHandle = IoTHubMessage_CreateFromByteArray((const unsigned char *)buffer, strlen(buffer));
    if (messageHandle == NULL)
    {
        Serial.println("Error::  Unable to create a new IoTHubMessage.");
        Serial.println("");
    }
    else
    {
        Serial.printf("Message:: %s \r\n", buffer);
        if (IoTHubClient_LL_SendEventAsync(iotHubClientHandle, messageHandle, sendCallback, NULL) != IOTHUB_CLIENT_OK)
        {
            Serial.println("Error::  Failed to hand over the message to IoTHubClient.");
            Serial.println("");
        }
        else
        {
            messagePending = true;
            Serial.println("Message::  Sending message to Azure IOT Hub.");
        }

        IoTHubMessage_Destroy(messageHandle);
    }
}

void Start()
{
    Serial.println("DirectMethod::  Started sending data.");
    Serial.println("");
    messageSending = true;
}

void Stop()
{
    Serial.println("DirectMethod::  Stopped sending data.");
    Serial.println("");
    messageSending = false;
}

IOTHUBMESSAGE_DISPOSITION_RESULT receiveMessageCallback(IOTHUB_MESSAGE_HANDLE message, void *userContextCallback)
{
    IOTHUBMESSAGE_DISPOSITION_RESULT result;
    const unsigned char *buffer;
    size_t size;
    if (IoTHubMessage_GetByteArray(message, &buffer, &size) != IOTHUB_MESSAGE_OK)
    {
        Serial.println("Error::  Unable to IoTHubMessage_GetByteArray.");
        result = IOTHUBMESSAGE_REJECTED;
    }
    else
    {
        /*buffer is not zero terminated*/
        char *temp = (char *)malloc(size + 1);

        if (temp == NULL)
        {
            return IOTHUBMESSAGE_ABANDONED;
        }

        strncpy(temp, (const char *)buffer, size);
        temp[size] = '\0';
        Serial.printf("Message:: Successfully received %s.\r\n", temp);
        Serial.println("");
        free(temp);
        blinkLED();
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
	Serial.printf("DirectMethod::  Invoking method %s.\r\n", methodName);
	const char *responseMessage = onSuccess;
	int result = 200;

	if (strcmp(methodName, "start") == 0 || strcmp(methodName, "Start") == 0) 
	{
		Start();
	}
	else if (strcmp(methodName, "stop") == 0 || strcmp(methodName, "Stop") == 0) 
	{
		Stop();
	}
	else if (strcmp(methodName, "git") == 0 || strcmp(methodName, "github") == 0 || strcmp(methodName, "gitlink") == 0) 
	{
		responseMessage = gitLink;
		result = 200;
	}
	else
    {
        Serial.printf("DirectMethod::  No method %s found.\r\n", methodName);
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
    {
        temp[i] = (char)(payLoad[i]);
    }
    temp[size] = '\0';
    parseTwinMessage(temp);
    free(temp);
}
