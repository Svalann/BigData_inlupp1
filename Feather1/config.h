/*
Gemensamt IOT-HUB (Event Hub) - CUSTOM

KEY:
iot18-iothub

VALUE:
Endpoint=sb://iothub-ns-iot18-ioth-1314863-9e54412716.servicebus.windows.net/;SharedAccessKeyName=iot18;SharedAccessKey=CV55i4YEhjbMBEXonSceYUxLAySMIxd1ILf5VhSE7ko=;EntityPath=iot18-iothub

Consumer Group: 6
HostName=iot18-iothub.azure-devices.net;DeviceId=41;SharedAccessKey=v1aXUI6cL6b9cwPhRjW4YxgS/ZC0n0MRXMZH5KZp890=
HostName=iot18-iothub.azure-devices.net;DeviceId=42;SharedAccessKey=eiO4wEyHXrf9MYal85Voe0Htz/yd//C5Ix0/hbJhqw4=
*/

#define NACKADEMIN 1
#define NACKA 2
#define PELARGATAN 3

#define LOCATION NACKA //write where you are/ which device you are using

#if LOCATION == NACKA
#define DEVICE_ID "41"
#define DEVICE_NAME "ViktorS Device1"
static char *connectionString = "HostName=iot18-iothub.azure-devices.net;DeviceId=41;SharedAccessKey=v1aXUI6cL6b9cwPhRjW4YxgS/ZC0n0MRXMZH5KZp890=";
static char *WiFi_SSID = "Rullen";
static char *WiFi_Password = "Banankaka17";
#define LATITUDE "59.259504"
#define LONGITUDE "18.195617"
#endif 

#if LOCATION == PELARGATAN
#define DEVICE_ID "42"
#define DEVICE_NAME "ViktorS Device2"
static char *connectionString = "HostName=iot18-iothub.azure-devices.net;DeviceId=42;SharedAccessKey=eiO4wEyHXrf9MYal85Voe0Htz/yd//C5Ix0/hbJhqw4=";
static char *WiFi_SSID = "Rullen";
static char *WiFi_Password = "Banankaka17";
#define LATITUDE "59.295734""
#define LONGITUDE "18.086679""
#endif 

#if LOCATION == NACKADEMIN
#define DEVICE_ID "Feather-TempSensor2"
static char *connectionString = "HostName=Inlupp3-cloud.azure-devices.net;DeviceId=Feather-TempSensor2;SharedAccessKey=bJatC58iLMxuqTtjNxmmCadvt95Zqenj5U/MueehOuM=";
static char *WiFi_SSID = "IoT";
static char *WiFi_Password = "IoT2018!";
#define LATITUDE "59.345279"
#define LONGITUDE "18.023274"
#endif 



#define MYNAME "Viktor S"
#define SENDER_ID "UWP-VentReg"
#define CLOUD_PROVIDER "Azure"
#define MESSAGE_TEMPERATURE_HIGH "High Temperature!"
#define MESSAGE_TEMPERATURE_LOW "Low Temperature!"
#define MESSAGE_BUTTON_PUSHED "Button pushed"
#define MESSAGE_VALVE_OPEN "Valve is now open"
#define MESSAGE_VALVE_CLOSED "Valve is now closed"
#define DHT_TYPE DHT11

#define SERVO_PIN 16
#define REDLED_PIN 15
#define GREENLED_PIN 14
#define DHT_PIN 13
#define BUTTON_PIN 12


#define TEMPERATURE_ALERT_HIGH 25
#define TEMPERATURE_ALERT_LOW 20
#define TEMPINTERVAL 1000*15		
#define SENDINTERVAL 1000*60*10		// 10 min		
#define WARNINGINTERVAL 1000*60*3	// 3 min	
#define BLINKTIME 500

#define SENSORTYPE "Ventilation Regulator"

#define MESSAGE_MAX_LEN 512

static bool messagePending = false;
static bool messageSending = true;
static bool alertStatus = false;
static bool alertSent = false;
static bool alertIsOver = false;
static bool buttonPushed = false;
static bool alertMessageConfirmedByReciever = false;


typedef enum
{
	VALVE_CLOSED,
	VALVE_OPEN
} VALVE;

static VALVE valvePosition;

unsigned long currentTime;
unsigned long nextSendTime;
unsigned long nextAlertTime;
unsigned long nextTempTime;	
unsigned long lastRedBlinkTime;
unsigned long lastGreenBlinkTime;
static int sendInterval = SENDINTERVAL;
static int tempInterval = TEMPINTERVAL;

char deviceName[32] = DEVICE_NAME;			// nya för TwinProp
char myName[32] = MYNAME;
char sensorType[32] = SENSORTYPE;

static int temperatureAlertHigh = TEMPERATURE_ALERT_HIGH;
static int temperatureAlertLow = TEMPERATURE_ALERT_LOW;
static float temperature;
char alertMessage[50];
