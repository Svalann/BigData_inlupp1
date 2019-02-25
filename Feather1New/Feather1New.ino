#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <AzureIoTHub.h>
#include <AzureIoTProtocol_MQTT.h>
#include <AzureIoTUtility.h>

#include "config.h"

static IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;


void setup()
{
	pinMode(LED_PIN, OUTPUT);

	initSerial();
	initWifi();
	initTime();
	iotHubClientHandle = initIotHub();
}

void loop() {
	currentMillis = millis();

	if (!messagePending && messageSending)
	{
		if ((currentMillis - prevMillis) >= interval) {
			prevMillis = currentMillis;

			char messagePayload[MESSAGE_MAX_LEN];
			readMessage(messagePayload);
			sendMessage(iotHubClientHandle, messagePayload);
		}
	}
	IoTHubClient_LL_DoWork(iotHubClientHandle);
	delay(10);
}