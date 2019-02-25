#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
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

void readMessage(char *payload)
{
    float temperature = readTemperature();
    float humidity = readHumidity();

    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;  
    JsonObject &root = jsonBuffer.createObject();
    root["id"] = DEVICE_ID;
    root["type"] = DEVICE_TYPE;
    root["student"] = DEVICE_STUDENT;
    
    JsonArray& positiondata = root.createNestedArray("position");
      positiondata.add(DEVICE_LOCATION_LAT);
      positiondata.add(DEVICE_LOCATION_LON);  

    JsonArray& dhtdata = root.createNestedArray("dht");
      if (std::isnan(temperature)){ dhtdata.add(NULL); } else { dhtdata.add(temperature); }
      if (std::isnan(humidity)){ dhtdata.add(NULL); } else { dhtdata.add(humidity); }
  
    root.printTo(payload, MESSAGE_MAX_LEN);
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

    if (root["desired"]["interval"].success())
    {
        interval = root["desired"]["interval"];
    }
    else if (root.containsKey("interval"))
    {
        interval = root["interval"];
    }
}
