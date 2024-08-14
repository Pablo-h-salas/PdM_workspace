#ifndef __API_DS1307_H
#define __API_DS1307_H

#include<stdint.h>
#include<stdbool.h>
#include "stm32f4xx_hal.h"
/*----------------------------------------------------------------------------*/
/*												DECLARACION DE VARIABLES Y TIPOS 						  			*/
/*----------------------------------------------------------------------------*/
typedef enum{
	COMMAND_NONE,
	COMMAND_SET_TIME,
	COMMAND_SET_DATE,
	COMMAND_SET_ALARM
	}commandType;

typedef struct{
	uint8_t 		second;		//0-59
	uint8_t 		minute;		//0-59
	uint8_t 		hour;			//0-23
	uint8_t 		day;			//1-31
	uint8_t 		weekDay;	//1-7
	uint8_t 		month;		//1-12
	uint8_t 		year;			//>=24
	commandType currentCommand;
	}timeDate_t;

typedef struct{
	uint8_t			minute;
	uint8_t			hour;
	uint8_t			day;
	uint8_t			month;
	uint8_t			year;
	bool 				alarmEnabled;
	}alarm_t;

extern timeDate_t timeParameters;
/*----------------------------------------------------------------------------*/
/*														PROTOTIPOS DE FUNCIONES 						  					*/
/*----------------------------------------------------------------------------*/
void rtcFSM_init();
void rtcFSM_update();
void readData(timeDate_t * rtc);
void validateCommand(uint8_t * commandBuffer, timeDate_t * timeSettings);
void writeTime (timeDate_t* rtc,uint8_t hour, uint8_t minute); 
bool setTime();
bool setDate();
bool setAlarm(alarm_t * alarm);
bool checkAlarm(alarm_t* alarm, timeDate_t *rtcTime);
bool triggerAlarm();
void showInfo(timeDate_t *rtc);
void commandErrorHandler();
void settingsErrorHandler();
uint8_t toBCD (uint8_t value);



#endif /*__API_DS1307_H*/