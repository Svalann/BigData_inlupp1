
void blinkRedLed_ON()
{
	digitalWrite(REDLED_PIN, HIGH);
	lastRedBlinkTime = millis();
}
void blinkGreenLed_ON()
{
	digitalWrite(GREENLED_PIN, HIGH);
	lastGreenBlinkTime = millis();
}

void blinkLeds_OFF(unsigned long currentTime)
{
	if (currentTime >= lastGreenBlinkTime + BLINKTIME*2)
		digitalWrite(GREENLED_PIN, LOW);
	if (currentTime >= lastRedBlinkTime + BLINKTIME)
		digitalWrite(REDLED_PIN, LOW);
}

void TriggerButton()
{
	Serial.print("\nButton pushed.");
	digitalWrite(REDLED_PIN, HIGH);
	alertStatus = true;
	buttonPushed = true;
	MoveVentliation();
	strcpy(alertMessage, MESSAGE_BUTTON_PUSHED);
	delay(300);
	digitalWrite(REDLED_PIN, LOW);
	delay(300);
}

bool ButtonPushed()
{
	if (digitalRead(BUTTON_PIN) == HIGH) return true;
	return false;
}


void initWiFi()
{
	Serial.print("WiFi communication initiated. Please wait...");
	WiFi.disconnect();
	WiFi.begin(WiFi_SSID, WiFi_Password);
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print(".");
		delay(500);
	}
	Serial.printf("Connected to WiFi Network %s \r\n", WiFi_SSID);
	Serial.print("IP-Adress: ");
	Serial.println(WiFi.localIP());
}

void initSerial()
{
	Serial.begin(115200);
	delay(2000);
	Serial.setDebugOutput(false);
	Serial.println("Serial successfully inited.");
}

void initPins()
{
	pinMode(REDLED_PIN, OUTPUT);
	pinMode(GREENLED_PIN, OUTPUT);
	digitalWrite(REDLED_PIN, HIGH);
	digitalWrite(GREENLED_PIN, HIGH);
	pinMode(BUTTON_PIN, INPUT);
}

void SetupComplete()
{
	Serial.println("Setup complete");
	digitalWrite(REDLED_PIN, LOW);
	digitalWrite(GREENLED_PIN, LOW);
}

