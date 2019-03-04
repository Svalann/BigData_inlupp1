// Physical device information for board and sensor

// NACKA
#define DEVICE_ID             WiFi.macAddress()     // already configured
#define DEVICE_STUDENT        "Viktor"
#define DEVICE_TYPE           "TempSensor"
#define DEVICE_LOCATION_LAT   "59.295734"
#define DEVICE_LOCATION_LON   "18.086679"    
#define DEVICE_SEND_INTERVAL  620000              

//#define WIFI_SSID             "IoT"
//#define WIFI_PASS             "IoT2018!"
#define WIFI_SSID             "Rullen"
#define WIFI_PASS             "Banankaka17"

static int interval = DEVICE_SEND_INTERVAL;
char deviceStudent[16] = DEVICE_STUDENT;
char deviceType[32] = DEVICE_TYPE;

unsigned long currentMillis = 0;
unsigned long prevMillis = 0;
static bool messagePending = false;
static bool messageSending = true;


//DHT Temperature sensor configuration
#define DHT_PIN               13	
#define DHT_TYPE              DHT11


// Pin layout configuration
#define LED_PIN               15


// Address configuration, don't remove!
#define MESSAGE_MAX_LEN       512
#define EEPROM_SIZE           512
#define SSID_LEN              32
#define PASS_LEN              32
#define CONNECTIONSTRING_LEN  256
