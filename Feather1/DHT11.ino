#include <Adafruit_Sensor.h>
#include <DHT.h>

static DHT dht(DHT_PIN, DHT_TYPE);

void initSensor()
{
	dht.begin();
}

float readTemperature()
{
	return dht.readTemperature();
}

float readHumidity()
{
	return dht.readHumidity();
}

void MeasureTemperature()
{
	if (currentTime >= nextTempTime)
	{
		temperature = readTemperature(); 

		if (temperature >= temperatureAlertHigh && currentTime >= nextAlertTime)
		{
			alertStatus = true;
			strcpy(alertMessage, MESSAGE_TEMPERATURE_HIGH);
			if(valvePosition != VALVE_OPEN)
				MoveVentliation(VALVE_OPEN);
		}
		else if (temperature <= temperatureAlertLow && currentTime >= nextAlertTime)
		{
			alertStatus = true;
			strcpy(alertMessage, MESSAGE_TEMPERATURE_LOW);
			if (valvePosition != VALVE_CLOSED)
				MoveVentliation(VALVE_CLOSED);
		}
		else if (alertSent && temperature < temperatureAlertHigh && temperature > temperatureAlertLow) // warning for temp sent earlier, now normal temp.
		{
			alertIsOver = true;		// now approve send to restore to normal.
			alertStatus = false;
		}
		nextTempTime = currentTime + TEMPINTERVAL;
	}
}


