// Physical device information for board and sensor

// NACKA
#define DEVICE_ID             WiFi.macAddress()     // already configured
#define DEVICE_STUDENT        "Viktor"
#define DEVICE_TYPE           "TempSensor"
#define DEVICE_LOCATION_LAT   "59.259504"
#define DEVICE_LOCATION_LON   "18.195617"    
#define DEVICE_SEND_INTERVAL  620000       

#define WIFI_SSID             "Rullen"
#define WIFI_PASS             "Banankaka17"
//#define WIFI_SSID             "IoT"
//#define WIFI_PASS             "IoT2018!"

static int interval = DEVICE_SEND_INTERVAL;
char deviceStudent[16] = DEVICE_STUDENT;
char deviceType[32] = DEVICE_TYPE;
#define TEMPERATURE_ALERT_HIGH 25

unsigned long currentMillis = 0;
unsigned long prevMillis = 0;
static bool messagePending = false;
static bool messageSending = true;
static bool temperatureAlert = false;

//DHT Temperature sensor configuration
#define DHT_PIN               13	
#define DHT_TYPE              DHT11


// Pin layout configuration
#define LED_PIN					15
#define SERVO_PIN				16


// Address configuration, don't remove!
#define MESSAGE_MAX_LEN       512
#define EEPROM_SIZE           512
#define SSID_LEN              32
#define PASS_LEN              32
#define CONNECTIONSTRING_LEN  256

typedef enum
{
	VALVE_CLOSED,
	VALVE_OPEN
} VALVE;

static VALVE valvePosition;