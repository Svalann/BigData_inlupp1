#include <ArduinoJson.h>

void prepareMessage(float temperature, char *payload)
{
    float humidity = readHumidity();
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["deviceId"] = DEVICE_ID;
	root["sendTo"] = SENDER_ID; // om UWP ska användas
	root["epochTime"] = (long)time(NULL);
	root["myName"] = MYNAME;

	root["latitude"] = LATITUDE;
	root["longitude"] = LONGITUDE;
	root["sensorType"] = SENSORTYPE;
	root["valvePosition"] = valvePosition? "Open" : "Closed";

    if (std::isnan(temperature))
        root["temperature"] = NULL;
    else
        root["temperature"] = temperature;

    if (std::isnan(humidity))
        root["humidity"] = NULL;
    else
        root["humidity"] = (int)humidity;

	if (alertStatus)
	{
		root["alertStatus"] = alertStatus;
		root["alertMessage"] = alertMessage;
	}
    root.printTo(payload, MESSAGE_MAX_LEN);
}

bool parseC2DMessage(char *message)
{	
	StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
	JsonObject &root = jsonBuffer.parseObject(message);
	if (!root.success())
		return false;
	else
	{		
		if (root["deviceId"] == SENDER_ID)
		{
			Serial.printf("\nReceived JSON message from device %s.\r\n", SENDER_ID);
			if (root.containsKey("tempOn"))
			{
				if (root["tempOn"] == true)
					Start();
				else
					Stop();
			}
			else if (root.containsKey("msgConfirmed"))
			{
				if (root["msgConfirmed"] == true)
				{
					int delayAlert = WARNINGINTERVAL * 10;
					alertMessageConfirmedByReciever = root["msgConfirmed"]; // not yet used
					nextAlertTime += currentTime + delayAlert; //warning confirmed by personal, delay warningmessage (default 30 min)
					Serial.printf("Alertmessage confirmed by UWP-user, pausing warningsmessages for %d minutes.\r\n", delayAlert / 1000 / 60);
				}
			}
			else if (root.containsKey("sendInterval"))
			{
				sendInterval = root["sendInterval"];
				nextSendTime = currentTime + sendInterval;
				Serial.printf("Send interval changed by UWP-user, now sending every %d second.\r\n", sendInterval / 1000);
			}
			else if (root.containsKey("alertLevel"))
			{
				temperatureAlertHigh = root["alertLevel"];
				Serial.printf("Temperature alert level changed by UWP-user, now alerts at %d degrees.\r\n", temperatureAlertHigh);
			}	
			else if (root.containsKey("valvePosition"))
			{
				root["valvePosition"] ? valvePosition = VALVE_OPEN : valvePosition = VALVE_CLOSED;
				MoveVentliation(valvePosition);
				Serial.printf("Valve position changed by UWP-user, valve is now %s.\r\n", valvePosition? "Open" : "Closed");
			}
		}
		else
			Serial.printf("\nReceived C2D JSON message from unexpected device: %s.\r\n", message);		
	}
	return true;
}


void parseTwinMessage(char *message)
{
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(message);
    if (!root.success())
    {
        Serial.printf("Parse %s failed.\r\n", message);
        return;
    }

    if (root["desired"]["sendInterval"].success())
    {
		sendInterval = root["desired"]["sendInterval"];
		Serial.printf("\nSendInterval is set to: %d\n", sendInterval);
    }
    else if (root.containsKey("sendInterval"))
    {
		sendInterval = root["sendInterval"];
		Serial.printf("\nTwinmessage recieved. SendInterval changed to: %d\n", sendInterval);
    }

	if (root["desired"]["tempInterval"].success())
	{
		sendInterval = root["desired"]["tempInterval"];
		Serial.printf("\nTempInterval is set to: %d\n", tempInterval);
	}
	else if (root.containsKey("tempInterval"))
	{
		sendInterval = root["tempInterval"];
		Serial.printf("\nTwinmessage recieved. TempInterval changed to: %d\n", tempInterval);
	}
}


bool SendMessageApproved ()
{
	if (!messagePending && messageSending && currentTime >= nextSendTime)	return true;	//coldpath - normal
	else if (alertStatus && !alertSent && !messagePending)	return true;					//hotpath  - high temp
	else if (alertStatus && buttonPushed && !messagePending)	return true;				//hotpath  - button
	else if (alertIsOver && !messagePending)	return true;								//coldpath - after high temp
	return false;
}
