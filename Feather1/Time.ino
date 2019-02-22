#define NTP_OFFSET   60 * 60   
#define NTP_INTERVAL 60 * 1000    
#define NTP_ADDRESS  "europe.pool.ntp.org"

void initTime()
{
	time_t epochTime;
	configTime(NTP_OFFSET, 0, NTP_ADDRESS);
	delay(2000);
	while (true)
	{
		epochTime = time(NULL);

		if (epochTime == 0)
		{
			Serial.println("Fetching NTP epoch time failed! Waiting 2 seconds to retry.");
			delay(2000);
		}
		else
		{
			Serial.printf("DELAY Fetched NTP epoch time is: %lu.\r\n", epochTime);
			break;
		}
	}
}

unsigned long UpdateCurrentTime()
{
	return currentTime = millis();
}



