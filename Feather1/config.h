#define NACKADEMIN 1
#define HOME 2

#define LOCATION NACKADEMIN //write where you are/ which device you are using

#if LOCATION == HOME
#define DEVICE_ID "Feather-TempSensor"
static char *connectionString = "HostName=Inlupp3-cloud.azure-devices.net;DeviceId=Feather-TempSensor;SharedAccessKey=bc0Y11SyviA+ZEjbstekKjtfJ4o+TuYYlRXe6MRwQAo=";
static char *WiFi_SSID = "Rullen";
static char *WiFi_Password = "Banankaka17";
#define LATITUDE 59259504
#define LONGITUDE 18195617
#endif 

#if LOCATION == NACKADEMIN
#define DEVICE_ID "Feather-TempSensor2"
static char *connectionString = "HostName=Inlupp3-cloud.azure-devices.net;DeviceId=Feather-TempSensor2;SharedAccessKey=bJatC58iLMxuqTtjNxmmCadvt95Zqenj5U/MueehOuM=";
static char *WiFi_SSID = "IoT";
static char *WiFi_Password = "IoT2018!";
#define LATITUDE 59345279
#define LONGITUDE 18023274
#endif 

#define MYNAME "Viktor Svahlstedt"
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
static int temperatureAlertHigh = TEMPERATURE_ALERT_HIGH;
static int temperatureAlertLow = TEMPERATURE_ALERT_LOW;
static float temperature;
char alertMessage[50];
