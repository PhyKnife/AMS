#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"tool.h"
#include"card_file.h"


void timeToString(time_t t, char* pBuf)
{
	struct tm* timeinfo;

	timeinfo = localtime(&t);
	strftime(pBuf, 20, "%Y-%m-%d %H:%M", timeinfo);
}
time_t stringToTime(char* pTime)
{
	struct tm tm1;
	time_t time1;

	sscanf(pTime, "%d-%d-%d %d:%d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday, &tm1.tm_hour, &tm1.tm_min);
	tm1.tm_year -= 1900;
	tm1.tm_mon -= 1;
	tm1.tm_sec = 0;
	tm1.tm_isdst = -1;

	time1 = mktime(&tm1);

	return time1;
}
